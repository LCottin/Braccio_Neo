#ifndef __XL320__
#define __XL320__

#include <stdlib.h>
#include <string>
#include <iostream>

#include "functions.hpp"
#include "Motor.hpp"
#include "include/dynamixel_sdk/dynamixel_sdk.h"  

using namespace std;
using namespace dynamixel;

class XL320 : public Motor
{
    private:

    public:
        XL320(const unsigned char ID);

        void start();
        bool openPort();
        bool setLed(const LED color);
        bool enableTorque();
        bool disableTorque();
        bool setBaudrate(const unsigned baudrate);
        bool move(const unsigned newPos);
        bool setP(const unsigned char p);
        bool setI(const unsigned char i);
        bool setD(const unsigned char d);
        bool setSpeed(const unsigned speed);

        ~XL320();
};

#endif