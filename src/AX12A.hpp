#ifndef __AX12A__
#define __AX12A__

#include <stdlib.h>
#include <string>
#include <iostream>

#include "Motor.hpp"

using namespace std;
using namespace dynamixel;

class AX12A : public Motor
{
    private:
        bool start();
        bool openPort();
        bool setBaudrate(const unsigned baudrate);
        
    public:
        AX12A(const unsigned char ID);

        bool enableTorque();
        bool disableTorque();
        bool move(const unsigned newPos, const bool degree = true, const bool blocking = true, const bool debug = false);
        bool setSpeed(const unsigned speed);
        bool ledOn();
        bool ledOff();
        double getVoltage();
        double getTemperature();
        double getLoad();
        bool middle();
        bool Infos();
        unsigned getPosition();

        ~AX12A();
};

#endif