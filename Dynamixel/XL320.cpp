#include "XL320.hpp"

XL320::XL320(const unsigned char ID) : Motor(ID)
{
    _TorqueEnableAddr     = 24;
    _GoalPosAddr    = 30;
    _PresentPosAddr = 37;

    _Protocol       = 2.0;

    _TorqueEnable   = true;
    _MinPos         = 0;
    _MaxPos         = 1023;

    start();
}

/**
 * Initialises the communication with the port handler
 */
void XL320::start()
{
    // Initialize PortHandler instance
	// Set the port path
	// Get methods and members of PortHandlerLinux or PortHandlerWindows
	_PortHandler = PortHandler::getPortHandler(_PortName.c_str());

	// Initialize PacketHandler instance
	// Set the protocol version
	// Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
	_PacketHandler = PacketHandler::getPacketHandler(_Protocol);

	// Open port
	if (!openPort())
		return;

	// Set port baudrate
	if (!setBaudrate(_Baudrate))
		return;

	//enable torque
	if (!enableTorque())
		return;
}

/**
 * Opens the USB port
 * @returns true if correctly opened, else false
 */
bool XL320::openPort()
{
	if (_PortHandler->openPort())
	{
		printf("Succeeded to open the port!\n");
		return true;
	}
	else
	{
		printf("Failed to open the port!\n");
		printf("Press any key to terminate...\n");
		getch();
		return false;
	}
}

/**
 * Makes the motor turn
 * @param newPos New position of the motor
 * @returns true if moved correctly, else false
 */
bool XL320::move(unsigned short newPos)
{
    newPos = newPos % 1023;
    _GoalPos = newPos;

    printf("Press any key to continue! (or press ESC to quit!)\n");
    if (getch() == ESC_ASCII_VALUE)
        return true;

    // Write goal position
    _ComResult = _PacketHandler->write4ByteTxRx(_PortHandler, _ID, _GoalPosAddr, _GoalPos, &_Error);
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

    do
    {
        // Read present position
        _ComResult = _PacketHandler->read4ByteTxRx(_PortHandler, _ID, _PresentPosAddr, (uint32_t*)&_PresentPos, &_Error);
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

        printf("[ID:%03d] GoalPos:%03d  PresPos:%03d\n", _ID, _GoalPos, _PresentPos);

    } while((abs(_GoalPos - _PresentPos) > _Threshold));
	return true;
}

/**
 * Enable the torque
 * @returns true if correctly enable, else false
 */
bool XL320::enableTorque()
{
	_TorqueEnable = true;
	_ComResult = _PacketHandler->write1ByteTxRx(_PortHandler, _ID, _TorqueEnableAddr, _TorqueEnable ? 1:0, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		printf("Error 1\n");
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		printf("Error 2\n");
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
bool XL320::disableTorque()
{
	_TorqueEnable = false;
	_ComResult = _PacketHandler->write1ByteTxRx(_PortHandler, _ID, _TorqueEnableAddr, _TorqueEnable ? 1:0, &_Error);
	if (_ComResult != COMM_SUCCESS)
	{
		printf("%s\n", _PacketHandler->getTxRxResult(_ComResult));
		printf("Error 1\n");
		return false;
	}
	else if (_Error != 0)
	{
		printf("%s\n", _PacketHandler->getRxPacketError(_Error));
		printf("Error 2\n");
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
bool XL320::setBaudrate(const int baudrate)
{
	if (_PortHandler->setBaudRate(_Baudrate))
	{
		printf("Succeeded to change the baudrate!\n");
		return true;
	}
	else
	{
		printf("Failed to change the baudrate!\n");
		printf("Press any key to terminate...\n");
		getch();
		return false;
	}
}

XL320::~XL320()
{
    // Close port
	_PortHandler->closePort();
}