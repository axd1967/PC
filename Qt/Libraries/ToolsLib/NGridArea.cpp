#include "NGridArea.h"
#include "NGrid.h"

using namespace Tools;


Tools::NGridArea::NGridArea() : _parentGrid(0)
{
}

Tools::NGridArea::NGridArea(const QRectF &rect, Tools::NGrid *grid) : _rect(rect), _parentGrid(grid)
{
    update();
}

bool NGridArea::isValid() const
{
    return _parentGrid != 0 && _rect.isValid();
}

void Tools::NGridArea::update()
{
    _nodes.clear();
    foreach(NGridNode* node, _parentGrid->getNodes())
    {
        if (_rect.contains(node->getPosition()))
            _nodes << node;
    }
}

const QList<Tools::NGridNode *>& Tools::NGridArea::getNodes() const
{
    return _nodes;
}

Tools::NGridNode *Tools::NGridArea::getNearestNode(const QPointF &position) const
{
    NGridNode* nearestNode = 0;
    double smallerDistance = -1.0;

    if (!_nodes.isEmpty())
    {
        smallerDistance = (_nodes.first()->getPosition() - position).manhattanLength();
        nearestNode = _nodes.first();
    }

    foreach(NGridNode* node, _nodes)
    {
        QPointF diff(node->getPosition());
        diff -= position;

        if (diff.isNull())
            return node;

        double d = diff.manhattanLength();
        if (d < smallerDistance)
        {
            smallerDistance = d;
            nearestNode = node;
        }
    }

    return nearestNode;
}

void Tools::NGridArea::setInternalCost(int cost)
{
    foreach(NGridNode* node, _nodes)
    {
        for(QHash<NGridNode*, int>::const_iterator it = node->getReachableNodes().constBegin(); it != node->getReachableNodes().constEnd(); ++it)
        {
            NGridNode* subNode = it.key();
            if (_nodes.contains(subNode))
                node->setReachableNodeCostToGo(subNode, cost);
        }
    }
}

void Tools::NGridArea::setCostToGoInside(int cost)
{
    foreach(NGridNode* node, _nodes)
    {
        foreach(NGridNode* subNode, node->getRevertedConnections())
        {
            if (!_nodes.contains(subNode))
                subNode->setReachableNodeCostToGo(node, cost);
        }
    }
}

void Tools::NGridArea::setCostToGoOutside(int cost)
{
    foreach(NGridNode* node, _nodes)
    {
        for(QHash<NGridNode*, int>::const_iterator it = node->getReachableNodes().constBegin(); it != node->getReachableNodes().constEnd(); ++it)
        {
            NGridNode* subNode = it.key();
            if (!_nodes.contains(subNode))
                node->setReachableNodeCostToGo(subNode, cost);
        }
    }
}


void Tools::NGridArea::setRect(const QRectF &rect)
{
    _rect = rect;
    update();
}

const QRectF &Tools::NGridArea::getRect() const
{
    return _rect;
}

bool Tools::NGridArea::contains(Tools::NGridNode *node) const
{
    return _nodes.contains(node);
}

bool NGridArea::contains(const QPointF &position) const
{
    return _rect.contains(position);
}

NGridNode *NGridArea::getCentralNode() const
{
    return getNearestNode(_rect.center());
}

//-------------------------------------------------------

void AreaLocker::lockArea(const QString &area, int costToGoInside, int costToGoOutside, int internalCost)
{
	AreaLockingValues values;
	values.costToGoInside = costToGoInside;
	values.costToGoOutside = costToGoOutside;
	values.internalCost = internalCost;

	_lockedAreas[area] = values;
	_areaList << area;

	setAreaValues(area);
}

void AreaLocker::setAreaValues(const QString &area)
{
	Tools::NGridArea* a = _grid->getArea(area);
	if (a && _lockedAreas.contains(area))
	{
		const AreaLockingValues& values = _lockedAreas[area];
		if (values.costToGoInside >= 0)
			a->setCostToGoInside(values.costToGoInside);

		if (values.costToGoInside >= 0)
			a->setCostToGoOutside(values.costToGoOutside);

		if (values.internalCost >= 0)
			a->setInternalCost(values.internalCost);
	}
}

void AreaLocker::unlockArea(const QString &area)
{
	Tools::NGridArea* a = _grid->getArea(area);
	if (a && _lockedAreas.contains(area))
	{
		const AreaLockingValues& values = _lockedAreas[area];

		if (values.costToGoInside >= 0)
			a->setCostToGoInside(1);

		if (values.costToGoInside >= 0)
			a->setCostToGoOutside(1);

		if (values.internalCost >= 0)
			a->setInternalCost(1);

		_lockedAreas.remove(area);
		_areaList.removeAll(area);

		foreach(const QString& area, _areaList)
			setAreaValues(area);
	}
}
