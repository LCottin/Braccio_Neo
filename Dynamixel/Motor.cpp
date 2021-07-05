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
    _Threshold  = 5;
    _GoalPos    = 0;   
    _Error      = 0;
    _ComResult  = COMM_TX_FAIL; 
}

/**
 * Tells where is the motor
 * @returns Current position
 */
unsigned Motor::getPosition() const
{
    return _PresentPos;
}

/**
 * Tells the motor baudrate
 * @returns Current baudrate
 */
unsigned Motor::getBaudrate() const
{
    return _Baudrate;
}

/**
 * Tells Proportionnal gain
 */
unsigned Motor::getP() const
{
    return _P;
}

/**
 * Tells Integral gain
 */
unsigned Motor::getI() const
{
    return _I;
}

/**
 * Tells Derivative gain
 */
unsigned Motor::getD() const
{
    return _D;
}

/**
 * Shows current motor speed
 */
unsigned Motor::getSpeed() const
{
    return _Speed;
}

/**
 * Tells if the torque is enabled
 * @returns true if enabled, else false
 */
bool Motor::getTorque() const
{
    return _TorqueEnable;
}

bool Motor::ledOff()
{
    _Led = false;
	return false;
}

bool Motor::ledOn()
{
    _Led = true;
	return true;
}

Motor::~Motor()
{

}
