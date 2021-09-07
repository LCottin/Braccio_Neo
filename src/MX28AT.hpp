#ifndef __MX28AT__
#define __MX28AT__

#include <stdlib.h>
#include <string>
#include <iostream>

#include "Motor.hpp"

using namespace std;
using namespace dynamixel;

class MX28AT : public Motor
{
    private:
        bool start();
        bool openPort();
        bool setBaudrate(const unsigned baudrate);
        
    public:
        MX28AT(const unsigned char ID);

        bool enableTorque();
        bool disableTorque();
        bool move(const unsigned newPos, const bool degree = true, const bool blocking = true, const bool debug = false);
        bool setP(const unsigned char p);
        bool setI(const unsigned char i);
        bool setD(const unsigned char d);
        bool setSpeed(const unsigned speed);
        bool ledOn();
        bool ledOff();
        double getVoltage();
        double getTemperature();
        double getLoad();
        bool middle();
        bool Infos();
        unsigned getPosition();

        ~MX28AT();
};

#endif