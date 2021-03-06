#ifndef NETWORKCOMMLISTENER_H
#define NETWORKCOMMLISTENER_H

#include <QList>
#include <QMap>

#include "Instructions.h"

class NetworkCommInterface;

class NetworkCommListener
{
public:
	virtual bool networkPingReceived() = 0;
	virtual void networkNoticeOfReceipt(quint8 instruction, bool result) = 0;

	virtual bool connectToRobot(NetworkCommInterface* networkInterface, bool simulation, const QString& robotPort, const QString& ax12Port, QByteArray &message) = 0;
	virtual bool disconnectToRobot(NetworkCommInterface* networkInterface) = 0;
	virtual bool updateServer(const QByteArray& data) = 0;

	virtual QStringList askFiles(int category) = 0;
	virtual QByteArray askFileData(int category, const QString& filename) = 0;
	virtual void setFileData(int category, const QString& filename, const QByteArray& data, NetworkCommInterface* sender) = 0;
	virtual void resetFile(int category, const QString& filename, NetworkCommInterface* sender) = 0;

	virtual bool startStrategy(int strategyNum, bool mirror) = 0;
	virtual bool stopStrategy(int& currentStrategyNum) = 0;
	virtual void askStrategyStatus(int& currentStrategyNum, bool& isRunning) = 0;

	virtual void askAutoStrategyInfo(bool &isEnabled, int &strategyNum, QString &robotPort, QString &ax12Port, bool &simulation, bool &mirror, int& startDelay) = 0;
	virtual void setAutoStrategy(bool enabled, int strategyNum, const QString &robotPort, const QString &ax12Port, bool simulation, bool mirror, int startDelay) = 0;

    virtual void askAx12Positions(NetworkCommInterface* networkInterface, const QList<quint8>& ids, bool recursive) = 0;
	
	virtual QByteArray askAx12MovementFile() = 0;
	virtual void setAx12MovementFile(const QByteArray& data) = 0;
    virtual void runAx12Movement(const QString& group, const QString& movement, float speedLimit, int lastPositionIndex) = 0;

	virtual void moveAx12(float maxSpeed, QList<Comm::Ax12Info>& ax12s) = 0;
	virtual void lockAx12(const QMap<quint8, bool>& servoLockInfo) = 0;

	virtual void resetParameters() = 0;
	virtual void saveParameters(const QList<float>& values) = 0;
	virtual QList<float> getParameters() = 0;

	virtual bool runScript(const QByteArray& script) = 0;
};

#endif // NETWORKCOMMLISTENER_H
