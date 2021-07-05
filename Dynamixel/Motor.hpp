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
        string _PortName;
        double _Protocol;
        unsigned char _ID;
        unsigned char _Threshold;
        unsigned _Baudrate;
        unsigned _MinPos;
        unsigned _MaxPos;

        //users attributs
        unsigned _PresentPos;
        unsigned _GoalPos;
        unsigned _Speed;
        unsigned char _P;
        unsigned char _I;
        unsigned char _D;
        bool  _TorqueEnable;
        LED _ColorLed;

        //addreses attributs
        unsigned char _PresentPosAddr;
        unsigned char _GoalPosAddr;
        unsigned char _TorqueEnableAddr;
        unsigned char _SpeedAddr;
        unsigned char _LedAddr;
        unsigned char _PAddr;
        unsigned char _IAddr;
        unsigned char _DAddr;

        uint8_t _Error;
        int _ComResult;
        PortHandler *_PortHandler;
        PacketHandler *_PacketHandler;

    public:
        Motor(const unsigned char ID);
        
        unsigned getPosition() const;
        unsigned getBaudrate() const;
        unsigned getP() const;
        unsigned getI() const;
        unsigned getD() const;
        unsigned getSpeed() const;
        bool     getTorque() const;
        
        virtual void start() = 0;
        virtual bool openPort() = 0;
        virtual bool setLed(const LED color) = 0;
        virtual bool move(unsigned newPos) = 0;
        virtual bool enableTorque() = 0;
        virtual bool disableTorque() = 0;
        virtual bool setBaudrate(const unsigned baudrate) = 0;
        virtual bool setP(const unsigned char p) = 0;
        virtual bool setI(const unsigned char i) = 0;
        virtual bool setD(const unsigned char d) = 0;
        virtual bool setSpeed(const unsigned speed) = 0;

        virtual ~Motor() = 0;
};




#endif