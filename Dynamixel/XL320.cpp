#include "XL320.hpp"

XL320::XL320(const unsigned char ID)
{
    _AddrTorque     = 24;
    _AddrGoalPos    = 30;
    _AddrPresentPos = 37;

    _Protocol       = 2.0;
    _ID             = ID;

    _Baudrate       = 1000000;

    //may change according to the device
    _PortName       = string("/dev/ttyACM0");

    _TorqueEnable   = true;
    _MinPos         = 0;
    _MaxPos         = 1023;
    _Threshold      = 5;
    _Error          = 0;

    init();

}

void XL320::init()
{
    // Initialize PortHandler instance
	// Set the port path
	// Get methods and members of PortHandlerLinux or PortHandlerWindows
	portHandler = PortHandler::getPortHandler(_PortName.c_str());

	// Initialize PacketHandler instance
	// Set the protocol version
	// Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
	packetHandler = PacketHandler::getPacketHandler(_Protocol);

	int index = 0;
	int dxl_comm_result = COMM_TX_FAIL;             // Communication result
	int dxl_goal_position[2] = {_MinPos, _MaxPos};         // Goal position

	uint8_t dxl_error = 0;                          // Dynamixel error
	int32_t dxl_present_position = 0;               // Present position

	// Open port
	if (portHandler->openPort())
	{
		printf("Succeeded to open the port!\n");
	}
	else
	{
		printf("Failed to open the port!\n");
		printf("Press any key to terminate...\n");
		getch();
		return;
	}

	// Set port baudrate
	if (portHandler->setBaudRate(_Baudrate))
	{
		printf("Succeeded to change the baudrate!\n");
	}
	else
	{
		printf("Failed to change the baudrate!\n");
		printf("Press any key to terminate...\n");
		getch();
		return;
	}

	// Enable Dynamixel Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, _ID, _AddrTorque, _TorqueEnable ? 1:0, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		// printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
		printf("Error 1\n");
	}
	else if (dxl_error != 0)
	{
		// printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		printf("%s\n", packetHandler->getRxPacketError(dxl_error));
		printf("Error 2\n");
	}
	else
	{
		printf("Dynamixel has been successfully connected \n");
	}
}

/**
 * Makes the motor turn
 * @param newPos New position of the motor
 */
void XL320::move(unsigned short newPos)
{
    newPos = newPos % 1023;
    _GoalPos = newPos;

    printf("Press any key to continue! (or press ESC to quit!)\n");
    if (getch() == ESC_ASCII_VALUE)
        return;

    // Write goal position
    char dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, _ID, _AddrGoalPos, _GoalPos, &_Error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
    }
    else if (_Error != 0)
    {
        printf("%s\n", packetHandler->getRxPacketError(_Error));
    }

    do
    {
        // Read present position
        dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, _ID, _AddrPresentPos, (uint32_t*)&_PresentPos, &_Error);
        if (dxl_comm_result != COMM_SUCCESS)
        {
            printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
        }
        else if (_Error != 0)
        {
            printf("%s\n", packetHandler->getRxPacketError(_Error));
        }

        printf("[ID:%03d] GoalPos:%03d  PresPos:%03d\n", _ID, _GoalPos, _PresentPos);

    } while((abs(_GoalPos - _PresentPos) > _Threshold));

}

XL320::~XL320()
{
    // Close port
	portHandler->closePort();
}