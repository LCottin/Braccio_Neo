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
        bool move(unsigned int newPos);
        bool enableTorque();
        bool disableTorque();
        bool setBaudrate(const int baudrate);

        ~XL320();
};

#endif