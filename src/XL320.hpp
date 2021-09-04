#ifndef __XL320__
#define __XL320__

#include <stdlib.h>
#include <string>
#include <iostream>

#include "Motor.hpp"

using namespace std;
using namespace dynamixel;

enum LED {OFF, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE};

class XL320 : public Motor
{
    private:
        LED _LedColor;
        
        bool ledOn(); //not used
        bool ledOff(); //not used
        bool start();
        bool openPort();
        bool setBaudrate(const unsigned baudrate);

    public:
        XL320(const unsigned char ID);

        bool setLed(const LED color);
        bool enableTorque();
        bool disableTorque();
        bool move(const unsigned newPos, const bool degree = true, const bool blocking = true, const bool debug = false);
        bool setP(const unsigned char p);
        bool setI(const unsigned char i);
        bool setD(const unsigned char d);
        bool setSpeed(const unsigned speed);
        double getVoltage();
        double getTemperature();
        double getLoad();
        bool middle();
        bool Infos();
        unsigned getPosition();

        ~XL320();
};

#endif