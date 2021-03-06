#ifndef WHILEACTION_H
#define WHILEACTION_H

#include "ConditionalActions.h"

class AbstractWhileBase : public AbstractAction
{
	Q_OBJECT

public:
	AbstractWhileBase(QObject* parent = 0);

protected:
	void connectSubAction(AbstractAction* action);
	void disconnectSubAction(AbstractAction* action);

protected:
	virtual void actionExecutionFinished() = 0;

private slots:
	void actionFinished();
};

template <class TL, class TR>
class AbstractWhile : public AbstractWhileBase
{
public:
	AbstractWhile(ConditionTest<TL, TR>* test, bool neg, TR testedRefValue, AbstractAction* loopedAction, QObject* parent = 0)
		: AbstractWhileBase(parent), _action(loopedAction), _test(test), _neg(neg), _stopped(false)
	{
		if (test)
			_test->setRightOperand(testedRefValue);
	}

	virtual ~AbstractWhile()
	{
		delete _action;
	}

	void execute()
	{
		if (!_test)
		{
			failed();
			return;
		}

		if (_action)
			connectSubAction(_action);

		actionExecutionFinished();
	}

	void stop()
	{
		_stopped = true;
		if (_action)
			_action->stop();
	}

	void end()
	{
		_stopped = false;
		if (_action)
		{
			_action->end();
			disconnectSubAction(_action);
		}
	}

	void setTestType(int type)
	{
		if (_test)
			_test->setType(type);
	}


protected:
	void setTest(ConditionTest<TL, TR>* test) {_test = test;}

	TR _testedValue;

private:
	AbstractAction* _action;
	ConditionTest<TL, TR>* _test;
	bool _neg;
	bool _stopped;

private:
	void actionExecutionFinished()
	{
		if (!_stopped)
		{
			_test->updateTest();

			if (_test->isTrue() == !_neg)
			{
				if (_action)
				{
					_action->end(); //end previous loop
					_action->execute();
				}
				else
					failed();
			}
			else
			{
				succeed();
			}
		}
		else
			failed();
	}
};

class PositionWhileAction : public AbstractWhile<QPointF, QRectF>
{
public:
	PositionWhileAction(StrategyManager* manager, const QRectF& rect, AbstractAction* loopedAction, bool neg, QObject* parent = 0);

	QString getActionName() const;
};

class OpponentWhileAction : public AbstractWhile<QPointF, QRectF>
{
public:
	OpponentWhileAction(StrategyManager* manager, const QRectF& rect, AbstractAction* loopedAction, bool neg, QObject* parent = 0);

	QString getActionName() const;
};

class OrientationWhileAction : public AbstractWhile<double, QPair<double, double>>
{
public:
	OrientationWhileAction(StrategyManager* manager, double angleMin, double angleMax, AbstractAction* loopedAction, bool neg, bool degrees = false, QObject* parent = 0);

	QString getActionName() const;
};

class SensorWhileAction : public AbstractWhile<int, QPair<int, int>>
{
public:
	SensorWhileAction(StrategyManager* manager, int sensorId, Comm::SensorType type, int value, AbstractAction* loopedAction, bool neg, QObject* parent = 0);

	QString getActionName() const;
};

class StrategyReversedWhileAction : public AbstractWhile<bool, bool>
{
public:
	StrategyReversedWhileAction(StrategyManager* manager, AbstractAction* loopedAction, bool neg, QObject* parent = 0);

	QString getActionName() const;
};

class InfiniteWhileAction : public AbstractWhile<bool, bool>
{
public:
	InfiniteWhileAction(AbstractAction* loopedAction, QObject* parent = 0);

	QString getActionName() const;
};


#endif // WHILEACTION_H
