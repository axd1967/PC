#include "NetworkProtocol.h"
#include "CommException.h"
#include "Instructions.h"
#include <QtDebug>

using namespace Comm;
using namespace Tools;

NetworkProtocol::NetworkProtocol(QIODevice* device, AbstractLogger* logger, bool bufferMode)
	: AbstractProtocol(device, logger), _currentState(FirstFF)
{
	if (bufferMode)
		run();
}


void NetworkProtocol::sendMessage(quint8 address, quint8 instruction, const Data &data) const
{
	Q_UNUSED(address);

	if (!_device->isOpen())
	{
		if (_showError)
			logger() << "Protocol: device not opened." << Tools::endl;
		return;
	}

	Data fullFrameData;
	Data checksumData;  //Used to calculate the checksum

	quint32 length = data.length() + 1; //data length + instruction

	//Calculate the checksum
	checksumData.add(length);
	checksumData.add(instruction);
	checksumData.add(data);
	quint8 checksum = (int)checksumData.checksum();

	fullFrameData.add((quint8)0xFF);
	fullFrameData.add((quint8)0xFF);
	fullFrameData.add(checksumData);
	fullFrameData.add(checksum);

	// Write the data to the device
	if(_device->write(fullFrameData) == -1 && _showError)
		logger() << "Protocol: writing failed." << Tools::endl;

	if (_messages)
		logger() << "Send Message : " << length << " bytes: " << (unsigned int)instruction << " - " << data << Tools::endl;
}

void NetworkProtocol::read()
{
	// Buffer mode
	QDataStream in(_device);
	in.setByteOrder(QDataStream::BigEndian);

	while (_device->bytesAvailable())
	{
		quint8 value;
		in >> value;

		if (_frames)
			logger() << ' ' << value;

		switch(_currentState)
		{
			case FirstFF:
			case SecondFF:
				if (value != 0xFF)
				{
					if (_showError)
						logger() << "Invalid Frame (" << value << "): Expected '0xFF0xFF' at the begining." << Tools::endl;
					incrementErrorCounter();
					_currentState = FirstFF;
					break;
				}
				_currentData.clear();
				_checksumData.clear();
				_lengthData.clear();
				_currentByteRead = 0;
				_currentInstruction = 0;
				if (_currentState == FirstFF)
					_currentState = SecondFF;
				else
					_currentState = Lenght;
				break;
			case Lenght:
				_lengthData.add(value);
				if (_lengthData.length() == sizeof(_currentLenght)) //length on quint32
				{
					_currentState = Instruction;
					_lengthData.take(_currentLenght);
					_checksumData.add(_currentLenght);
				}
				break;
			case Instruction:
				_currentInstruction = value;
				_checksumData.add(_currentInstruction);
				if (_currentLenght > 1)
					_currentState = ReadingData;
				else
					_currentState = Checksum;
				break;
			case ReadingData:
				if (_currentLenght == 0)
				{
					_currentState = Checksum;
					break;
				}

				_checksumData.add(value);
				_currentData.add(value);
				++_currentByteRead;
				if (_currentByteRead == _currentLenght - 1)
					_currentState = Checksum;
				break;
			case Checksum:
				quint8 calcChecksum = _checksumData.checksum();
				_currentState = FirstFF;
				if (value != calcChecksum)
				{
					if (_showError)
						logger() << "Invalid checksum." << "Expected:" << calcChecksum << "Found:" << value << Tools::endl;
					incrementErrorCounter();
					break;
				}

				if (_frames)
					logger() << " - Valid message." << Tools::endl;

				resetErrorCounter();
				if (_messages)
					logger() << "Received Message : " << _currentLenght << " bytes: " << (unsigned int)_currentInstruction << " - " << _currentData << Tools::endl;

				emit message(_currentInstruction, _currentData, 0);
				break;
		}
	}

	if (_frames)
		logger() << Tools::endl;
}
