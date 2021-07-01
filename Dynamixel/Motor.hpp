#ifndef __MOTOR__
#define __MOTOR__

#include <stdlib.h>
#include <string>
#include <cmath>
#include <iostream>

#include "functions.hpp"
#include "include/dynamixel_sdk/dynamixel_sdk.h"  

using namespace std;
using namespace dynamixel;

enum LED {OFF, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE};

class Motor
{
    protected:
        //motors attributs
        unsigned char _ID;
        double _Protocol;
        string _PortName;
        unsigned char _Threshold;
        int _Baudrate;
        unsigned int _MinPos;
        unsigned int _MaxPos;

        //users attributs
        unsigned int _PresentPos;
        unsigned int _GoalPos;
        bool  _TorqueEnable;
        LED _ColorLed;

        //addreses attributs
        short _PresentPosAddr;
        short _GoalPosAddr;
        short _TorqueEnableAddr;
        short _LedAddr;

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
        virtual bool setLed(const LED color) = 0;
        virtual bool move(unsigned int newPos) = 0;
        virtual bool enableTorque() = 0;
        virtual bool disableTorque() = 0;
        virtual bool setBaudrate(const int baudrate) = 0;

        virtual ~Motor() = 0;
};




#endif