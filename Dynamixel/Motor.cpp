#include "Motor.hpp"

Motor::Motor(const unsigned char ID)
{
    _ID = ID;

    #if defined (__APPLE__)
        _PortName = string("/dev/ttyUSB0");
    #elif defined (__linux__)
        _PortName = string("/dev/ttyACM0");
    #endif

    _Baudrate   = 1000000;
    _Threshold  = 10;
    _GoalPos    = 0;   
    _Error      = 0;
    _ComResult  = COMM_TX_FAIL; 
}

/**
 * Tells where is the motor
 * @returns Current position
 */
short Motor::getPosition() const
{
    return _PresentPos;
}

/**
 * Tells the motor baudrate
 * @returns Current baudrate
 */
int Motor::getBaudrate() const
{
    return _Baudrate;
}

Motor::~Motor()
{

}