#ifndef COMMLISTENER_H
#define COMMLISTENER_H

#include <QtGlobal>
#include <QList>
#include <QStringList>

namespace Comm
{
	class CommListener
	{
	public:
		virtual ~CommListener() {}

		virtual void coordinates(qint16 x, qint16 y, double theta, quint8 direction) {Q_UNUSED(x); Q_UNUSED(y); Q_UNUSED(theta); Q_UNUSED(direction);}
		virtual void objective(qint16 x, qint16 y, double theta) {Q_UNUSED(x); Q_UNUSED(y); Q_UNUSED(theta);}
		virtual void avoidingSensors(const QList<quint8>& values) {Q_UNUSED(values);}
		virtual void microswitchs(const QList<quint8>& values) {Q_UNUSED(values);}
		virtual void otherSensors(const QList<quint8>& values) {Q_UNUSED(values);}
        virtual void colorSensors(const QList<QColor>& values) {Q_UNUSED(values);}
		virtual bool initDone() {return true;}
		virtual bool go(bool mirrored) {Q_UNUSED(mirrored); return false;}
		virtual bool pingReceived() {return true;}
		virtual void noticeOfReceipt(quint8 instruction, bool result) {Q_UNUSED(instruction); Q_UNUSED(result);}
		virtual void opponentPosition(qint16 x, qint16 y) {Q_UNUSED(x); Q_UNUSED(y);}
		virtual bool restart() {return false;}
		virtual void quit() {}
		virtual void isArrived() {}
		virtual void isBlocked() {}
		virtual void log(const QByteArray& text) {Q_UNUSED(text);}
		virtual void parameters(const QList<float>& values) {Q_UNUSED(values);}
		virtual void parameterNames(const QStringList& names) {Q_UNUSED(names);}
	};
}

#endif // COMMLISTENER_H
