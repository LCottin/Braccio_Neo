#include "MX28AT.hpp"

MX28AT::MX28AT(const unsigned char ID) : Motor(ID)
{
    _TorqueEnableAddr   = 24;
	_LedAddr			= 25;
	_DAddr				= 26;
	_IAddr				= 27;
	_PAddr				= 28;
    _GoalPosAddr    	= 30;
	_SpeedAddr			= 32;
    _PresentPosAddr 	= 36;
	_LoadAddr			= 40;
	_VoltageAddr		= 42;
	_TemperatureAddr	= 43;

    _Protocol       	= 1.0;

    _MaxSpeed           = 2047;
	_MinPos				= 0;
	_MaxPos				= 4096;
	_Middle				= (_MinPos + _MaxPos) / 2;
	_Speed				= 300;
    _TorqueEnable   	= true;
    _Led                = true;

    start();
}

/**
 * Initialises the communication with the port handler
 */
bool MX28AT::start()
{
    // Initializes PortHandler instance
	// Sets the port path
	// Gets methods and members of PortHandlerLinux or PortHandlerWindows
	_PortHandler = PortHandler::getPortHandler(_PortName.c_str());

	// Initializes PacketHandler instance
	// Sets the protocol version
	// Gets methods and members of Protocol1PacketHandler or Protocol2PacketHandler
	_PacketHandler = PacketHandler::getPacketHandler(_Protocol);

	// Opens port
	if (!openPort())
		return false;

	// Set port baudrate
	if (!setBaudrate(_Baudrate))
		return false;

	// Enables torque
	if (!enableTorque())
		return false;

	// Sets speed
	if (!setSpeed(_Speed))
		return false;

	if (!move(0))
		return false;

	//sets initial volatge, temperature and load
	getVoltage();
	getTemperature();
	getLoad();

	printf("Motor %d correctly started.\n", _ID);

	return true;
}

/**
 * Opens the USB port
 * @returns true if correctly opened, else false
 */
bool MX28AT::openPort()
{
	if (_PortHandler->openPort())
	{
		cout << "Succeeded to open the port!\n" << endl;
		return true;
	}
	else
	{
		cout << "Failed to open the port!\n" << endl;
		cout << "Press any key to terminate...\n" << endl;
		getch();
		return false;
	}
}

/**
 * Makes the motor turn
 * @param newPos New position of the motor
 * @param blocking Should the movment be blocking ? (False by default)
 * @returns true if moved correctly, else false
 */
bool MX28AT::move(const unsigned newPos, const bool blocking, const bool debug)
{
    _GoalPos = newPos % _MaxPos;

    // Write goal position
    _ComResult = _PacketHandler->write2ByteTxRx(_PortHandler, _ID, _GoalPosAddr, _GoalPos, &_Error);
    if (_ComResult != COMM_SUCCESS)
    {
        printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		return false;
    }
    else if (_Error != 0)
    {
        printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		return false;
    }

	if (blocking)
	{
		int dif = 0;
		do
		{
			// Reads present position
			_ComResult = _PacketHandler->read2ByteTxRx(_PortHandler, _ID, _PresentPosAddr, (uint16_t*)&_PresentPos, &_Error);
			if (debug)
			{
				if (_ComResult != COMM_SUCCESS)
				{
					printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
				}
				else if (_Error != 0)
				{
					printf("%s\n", _PacketHandler->getRxPacketError(_Error));
				}

				printf("[ID:%03d] GoalPos:%03d  PresPos:%03d\n", _ID, _GoalPos, _PresentPos);
			}
			
			if (_PresentPos < _MinPos)
				_PresentPos = _MinPos;
			if (_PresentPos > _MaxPos)
				_PresentPos = _MaxPos;

			dif = _GoalPos - _PresentPos;
		} while((abs(dif) > _Threshold));
	}
	return true;
}

/**
 * Enable the torque
 * @returns true if correctly enabled, else false
 */
bool MX28AT::enableTorque()
{
	_TorqueEnable = true;
	_ComResult = _PacketHandler->write1ByteTxRx(_PortHandler, _ID, _TorqueEnableAddr, _TorqueEnable ? 1:0, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Enable Torque : Error 1\n" << endl;
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Enable Torque : Error 2\n" << endl;
		return false;
	}
	else
	{
		cout << "Torque successfully enabled " << endl;
		return true;
	}
}

/**
 * Disable the torque
 * @returns true if correctly disabled, else false
 */
bool MX28AT::disableTorque()
{
	_TorqueEnable = false;
	_ComResult = _PacketHandler->write1ByteTxRx(_PortHandler, _ID, _TorqueEnableAddr, _TorqueEnable ? 1:0, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Disable Torque : Error 1\n" << endl;
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Disable Torque : Error 2\n" << endl;
		return false;
	}
	else
	{
		cout << "Torque successfully disabled " << endl;
		return true;
	}
}

/**
 * Allows to change the baudrate
 * @param baudrate New baudrate
 * @returns true if the baudrate is correctly changed, else false
 */
bool MX28AT::setBaudrate(const unsigned baudrate)
{
	if (_PortHandler->setBaudRate(_Baudrate))
	{
		_Baudrate = baudrate;
		cout << "Succeeded to change the baudrate!\n" << endl;
		return true;
	}
	else
	{
		cout << "Failed to change the baudrate!\n" << endl;
		cout << "Press any key to terminate...\n" << endl;
		getch();
		return false;
	}
}

/**
 * Turns the led on
 * @returns true if correctly turned on, else false
 */
bool MX28AT::ledOn()
{
	_Led = true;
	_ComResult = _PacketHandler->write1ByteTxRx(_PortHandler, _ID, _LedAddr, _Led ? 1 : 0, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Led On : Error 1\n" << endl;
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Led On: Error 2\n" << endl;
		return false;
	}
    cout << "Led successfully turned on" << endl;
    return true;
}

/**
 * Turns the led off
 * @returns true if correctly turned off, else false
 */
bool MX28AT::ledOff()
{
	_Led = false;
	_ComResult = _PacketHandler->write1ByteTxRx(_PortHandler, _ID, _LedAddr, _Led ? 1 : 0, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Led Off : Error 1\n" << endl;
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Led Off: Error 2\n" << endl;
		return false;
	}
    cout << "Led successfully turned off" << endl;
    return true;
}

/**
 * Changes Propotionnal Gain
 * @param p New value
 * @returns true if correctly changed, else false
 */
bool MX28AT::setP(const unsigned char p)
{
	_P = p;
	_ComResult = _PacketHandler->write1ByteTxRx(_PortHandler, _ID, _PAddr, _P, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Set P : Error 1\n" << endl;
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Set P : Error 2\n" << endl;
		return false;
	}
	else
	{
		cout << "Proportionnal gain successfully changed " << endl;
		return true;
	}
}

/**
 * Changes Integral Gain
 * @param i New value
 * @returns true if correctly changed, else false
 */
bool MX28AT::setI(const unsigned char i)
{
	_I = i;
	_ComResult = _PacketHandler->write1ByteTxRx(_PortHandler, _ID, _IAddr, _I, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Set I : Error 1\n" << endl;
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Set I : Error 2\n" << endl;
		return false;
	}
	else
	{
		cout << "Integral gain successfully changed " << endl;
		return true;
	}
}

/**
 * Changes Derivative Gain
 * @param d New value
 * @returns true if correctly changed, else false
 */
bool MX28AT::setD(const unsigned char d)
{
	_D = d;
	_ComResult = _PacketHandler->write1ByteTxRx(_PortHandler, _ID, _DAddr, _D, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Set D : Error 1\n" << endl;
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Set D : Error 2\n" << endl;
		return false;
	}
	else
	{
		cout << "Derivative gain successfully changed " << endl;
		return true;
	}
}

/**
 * Changes the velocity
 * @param speed New velocity
 * @returns true if correctly changed, else false
 */
bool MX28AT::setSpeed(const unsigned speed)
{
	_Speed = speed < _MaxSpeed ? speed : _MaxSpeed;
	_ComResult = _PacketHandler->write2ByteTxRx(_PortHandler, _ID, _SpeedAddr, _Speed, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Set Speed : Error 1\n" << endl;
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Set Speed : Error 2\n" << endl;
		return false;
	}
	else
	{
		cout << "Velocity successfully changed " << endl;
		return true;
	}
}

/**
 * Tells under what voltage the motor is 
 * @returns Motor voltage
 */
double MX28AT::getVoltage()
{	
	uint8_t temp;
	_ComResult = _PacketHandler->read1ByteTxRx(_PortHandler, _ID, _VoltageAddr, (uint8_t*)&temp, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Read voltage : Error 1\n" << endl;
		return -1;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Read voltage : Error 2\n" << endl;
		return -1;
	}
	else
	{
		cout << "Voltage successfully read " << endl;
		_Voltage = (double)temp / 10;
		return _Voltage;
	}
} 

/**
 * Tells how hot he motor is
 * @returns Motor temperature
 */
double MX28AT::getTemperature()
{
	uint8_t temp;
	_ComResult = _PacketHandler->read1ByteTxRx(_PortHandler, _ID, _TemperatureAddr, (uint8_t*)&temp, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Read temperature : Error 1\n" << endl;
		return -1;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Read temperature : Error 2\n" << endl;
		return -1;
	}
	else
	{
		cout << "Temperature successfully read " << endl;
		_Temperature = (double)temp;
		return _Temperature;
	}
}

/**
 * Tells how loaded the motor is
 * @returns Motor load
 */
double MX28AT::getLoad()
{
	uint16_t temp;
	_ComResult = _PacketHandler->read2ByteTxRx(_PortHandler, _ID, _LoadAddr, (uint16_t*)&temp, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		cout << "Read load : Error 1\n" << endl;
		return -1;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		cout << "Read load : Error 2\n" << endl;
		return -1;
	}
	else
	{
		cout << "Load successfully read " << endl;
		if (temp > 2047) temp = 2047;

		if (temp < 1024)
		{
			cout << "The motor is loaded counter clock wise" << endl;
		}
		else if (temp >= 1024)
		{
			cout << "The motor is loaded clock wise" << endl;
			temp -= 1024;
		}

        _Load = (double)temp * (double)100 / (double)1024;
		return _Load;
	}
}

/**
 * Puts the motor at its middle position
 * @returns true if successfully moved, else false
 */
bool MX28AT::middle()
{
	return move(_Middle, true);
}

/**
 * Prints all information about the motor
 * @returns true if everything went normal, else false
 */
bool MX28AT::Infos()
{
	if (getVoltage() == -1) return false;
	if (getTemperature() == -1) return false;
	if (getLoad() == -1) return false;

	char* torque;
	char allume[] = "allumé";
	char eteint[] = "éteint";
	if (_TorqueEnable) torque = allume;
	else 			   torque = eteint;

	cout<< "*********************************" << endl;
	printf("Position 	: %d\n", _PresentPos);
	printf("Température	: %lf\n", _Temperature);
	printf("Charge 		: %lf\n", _Load);
	printf("Voltage 	: %lf\n", _Voltage);
	printf("Gain P 		: %d\n", _P);
	printf("Gain I 		: %d\n", _I);
	printf("Gain D 		: %d\n", _D);
	printf("Couple 		: %s\n", torque);
	printf("Vitesse 	: %d\n", _Speed);
	cout<< "*********************************" << endl;
	return true;
}

MX28AT::~MX28AT()
{
    // Close port
	_PortHandler->closePort();
}
