#ifndef __MOTOR__
#define __MOTOR__

#include <stdlib.h>
#include <string>
#include <iostream>

#include "functions.hpp"
#include "include/dynamixel_sdk/dynamixel_sdk.h"  

using namespace std;
using namespace dynamixel;

class Motor
{
    protected:
        unsigned char _ID;
        double _Protocol;
        string _PortName;
        unsigned char _Threshold;

        int _Baudrate;
        short _PresentPos;
        short _GoalPos;
        short _MinPos;
        short _MaxPos;
        bool  _TorqueEnable;

        short _PresentPosAddr;
        short _GoalPosAddr;
        short _TorqueEnableAddr;

        uint8_t _Error;
        int _ComResult;
        PortHandler *_PortHandler;
        PacketHandler *_PacketHandler;

    public:
        Motor(const unsigned char ID);
        
        short getPosition() const;
        int getBaudrate() const;
        
        virtual void start() = 0;
        virtual bool openPort() = 0;
        virtual bool move(unsigned short newPos) = 0;
        virtual bool enableTorque() = 0;
        virtual bool disableTorque() = 0;
        virtual bool setBaudrate(const int baudrate) = 0;

        virtual ~Motor() = 0;
};




#endif