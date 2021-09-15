#ifndef __MOTOR__
#define __MOTOR__

#include <stdlib.h>
#include <string>
#include <cmath>
#include <iostream>

#include "functions.hpp"
#include "./lib/SDK/dynamixel_sdk.h"  

using namespace std;
using namespace dynamixel;


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
        unsigned _Middle;
        unsigned _MaxSpeed; 

        //users attributs
        unsigned _PresentPos;
        unsigned _GoalPos;
        unsigned _Speed;
        double _Load;
        double _Voltage;
        double _Temperature;
        unsigned char _P;
        unsigned char _I;
        unsigned char _D;
    
        bool  _TorqueEnable;
        bool _Led;

        //addreses attributs
        unsigned char _PresentPosAddr;
        unsigned char _GoalPosAddr;
        unsigned char _TorqueEnableAddr;
        unsigned char _SpeedAddr;
        unsigned char _LedAddr;
        unsigned char _PAddr;
        unsigned char _IAddr;
        unsigned char _DAddr;
        unsigned char _VoltageAddr;
        unsigned char _TemperatureAddr;
        unsigned char _LoadAddr;

        uint8_t _Error;
        int _ComResult;
        PortHandler *_PortHandler;
        PacketHandler *_PacketHandler;

    public:
        Motor(const unsigned char ID);
        
        unsigned getBaudrate() const;
        unsigned getP() const;
        unsigned getI() const;
        unsigned getD() const;
        unsigned getSpeed() const;
        bool     getTorque() const;
        unsigned getMaxSpeed() const;

        virtual bool ledOn(); //not for XL320
        virtual bool ledOff(); //not for XL320
        virtual bool setP(const unsigned char p); // not for AX12A
        virtual bool setI(const unsigned char i); // not for AX12A
        virtual bool setD(const unsigned char d); // not for AX12A
        
        virtual bool start() = 0;
        virtual bool middle() = 0;
        virtual bool openPort() = 0;
        virtual bool move(const unsigned newPos, const bool degree = true, const bool blocking = true, const bool debug = false) = 0;
        virtual bool enableTorque() = 0;
        virtual bool disableTorque() = 0;
        virtual bool setBaudrate(const unsigned baudrate) = 0;
        virtual bool setSpeed(const unsigned speed) = 0;
        virtual double getVoltage() = 0;
        virtual double getTemperature() = 0;
        virtual double getLoad() = 0;
        virtual bool Infos() = 0;
        virtual unsigned getPosition() = 0;

        virtual ~Motor() = 0;
};




#endif
