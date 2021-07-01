#ifndef __XL320__
#define __XL320__

#include <stdlib.h>
#include <string>
#include <iostream>

#include "functions.hpp"
#include "include/dynamixel_sdk/dynamixel_sdk.h"  

using namespace std;
using namespace dynamixel;

class XL320
{
    private:
        short _AddrTorque;
        short _AddrGoalPos;
        short _AddrPresentPos;

        double _Protocol;
        unsigned char _ID;

        int _Baudrate;
        string _PortName;

        bool _TorqueEnable;
        short _MinPos;
        short _MaxPos;
        short _PresentPos;
        short _GoalPos;
        short _Threshold;

        uint8_t _Error;

        PortHandler *portHandler;
        PacketHandler *packetHandler;

    public:
        XL320(const unsigned char ID);
        void init();

        void move(unsigned short newPos);

        ~XL320();
};

#endif