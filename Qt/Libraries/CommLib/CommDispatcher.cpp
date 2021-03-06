#include "CommDispatcher.h"

using namespace Comm;

CommDispatcher::CommDispatcher() : _main(0)
{
}


void Comm::CommDispatcher::setMainListener(Comm::CommListener *listener)
{
	_main = listener;
}

void Comm::CommDispatcher::registerResponder(Comm::CommListener *listener)
{
	_responders << listener;
}

void CommDispatcher::unregisterResponder(CommListener *listener)
{
	_responders.removeAll(listener);
}

void Comm::CommDispatcher::coordinates(qint16 x, qint16 y, double theta, quint8 forward)
{
	foreach(CommListener* responder, _responders)
		responder->coordinates(x, y, theta, forward);

	if (_main)
		_main->coordinates(x, y, theta, forward);
}

void Comm::CommDispatcher::objective(qint16 x, qint16 y, double theta)
{
	foreach(CommListener* responder, _responders)
		responder->objective(x, y, theta);

	if (_main)
		_main->objective(x, y, theta);
}

void Comm::CommDispatcher::activatedSensors(const QList<quint8> &values)
{
	foreach(CommListener* responder, _responders)
		responder->activatedSensors(values);

	if (_main)
		_main->activatedSensors(values);
}

bool Comm::CommDispatcher::go(bool mirrored)
{
	foreach(CommListener* responder, _responders)
		responder->go(mirrored);

	if (_main)
		return _main->go(mirrored);

	return true;
}

void CommDispatcher::initReceived()
{
	foreach(CommListener* responder, _responders)
		responder->initReceived();

	if (_main)
		return _main->initReceived();
}

bool Comm::CommDispatcher::pingReceived()
{
	foreach(CommListener* responder, _responders)
		responder->pingReceived();

	if (_main)
		return _main->pingReceived();

	return true;
}

void Comm::CommDispatcher::noticeOfReceipt(quint8 instruction, bool result)
{
	foreach(CommListener* responder, _responders)
		responder->noticeOfReceipt(instruction, result);

	if (_main)
		_main->noticeOfReceipt(instruction, result);
}

void Comm::CommDispatcher::opponentPosition(qint16 x, qint16 y)
{
	foreach(CommListener* responder, _responders)
		responder->opponentPosition(x, y);

	if (_main)
		_main->opponentPosition(x, y);
}

void Comm::CommDispatcher::log(const QByteArray &text)
{
	foreach(CommListener* responder, _responders)
		responder->log(text);

	if (_main)
		_main->log(text);
}

void CommDispatcher::parameters(const QList<float> &values)
{
	foreach(CommListener* responder, _responders)
		responder->parameters(values);

	if (_main)
		_main->parameters(values);
}

void CommDispatcher::parameterNames(const QStringList &names)
{
	foreach(CommListener* responder, _responders)
		responder->parameterNames(names);

	if (_main)
		_main->parameterNames(names);
}

void CommDispatcher::robotEvent(RobotEvent event)
{
	foreach(CommListener* responder, _responders)
		responder->robotEvent(event);

	if (_main)
		_main->robotEvent(event);
}

void CommDispatcher::sensorEvent(SensorType type, int sensorId, int value)
{
	foreach(CommListener* responder, _responders)
		responder->sensorEvent(type, sensorId, value);

	if (_main)
		_main->sensorEvent(type, sensorId, value);
}

void CommDispatcher::registerGraph(int graphId, GraphType type, const QString &name, const QStringList &parameterNames)
{
	foreach(CommListener* responder, _responders)
		responder->registerGraph(graphId, type, name, parameterNames);

	if (_main)
		_main->registerGraph(graphId, type, name, parameterNames);
}

void CommDispatcher::graphValues(int graphId, const QList<float> &values)
{
	foreach(CommListener* responder, _responders)
		responder->graphValues(graphId, values);

	if (_main)
		_main->graphValues(graphId, values);
}

void CommDispatcher::graphSingleValues(int graphId, int parameterId, float value)
{
	foreach(CommListener* responder, _responders)
		responder->graphSingleValues(graphId, parameterId, value);

	if (_main)
		_main->graphSingleValues(graphId, parameterId, value);
}
