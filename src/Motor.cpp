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
    _MaxSpeed   = 1023;
    _Threshold  = 8;
    _GoalPos    = 0;   
    _Error      = 0;
    _ComResult  = COMM_TX_FAIL; 
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
 * Shows motor maximum speed
 */
unsigned Motor::getMaxSpeed() const
{
    return _MaxSpeed;
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

bool Motor::setP(const unsigned char p)
{
    return false;
}

bool Motor::setI(const unsigned char i)
{
    return false;
}

bool Motor::setD(const unsigned char d)
{
    return false;
}

Motor::~Motor()
{

}
