#ifndef __BRACCIO__
#define __BRACCIO__

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <algorithm>
//#include <wiringPi.h>

#include "Motor.hpp"
#include "MX106AT.hpp"
#include "MX28AT.hpp"
#include "MX64AT.hpp"
#include "AX18A.hpp"
#include "AX12A.hpp"
#include "MX12W.hpp"
#include "Variables.hpp"

#include "lib/RASPICAM/raspicam.h"
/*
#include "lib/RF24/RF24.h"
#include "lib/RF24/nRF24L01.h"
#include "lib/RF24/RF24Network.h"
*/
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24/nRF24L01.h>


#define MILLISECOND 1000
#define WAIT _BraccioNeo::pauseStop (network); \
        while(_pause){_BraccioNeo::pauseStop(network);} \
        if (_stop) {receivedData.mode = NONE ; return;}

using namespace std;
using namespace raspicam;

class _BraccioNeo
{
    private:
        Motor* _Base;
        Motor* _Shoulder;
        Motor* _Elbow;
        Motor* _WristVer;
        Motor* _WristRot;
        Motor* _Gripper;

        vector<Motor*> _Motors;
        
        unsigned _NbMotors;
        unsigned** _Limits;
        unsigned* _CurrentPosition;
        bool _Stand;

        char _PinLed1;
        char _PinLed2;

        clock_t _Start;
        clock_t _Stop;
        SPEED _Speed;  


    public:
        _BraccioNeo();
        void initValues();
        bool stand();
        bool Infos() const;
        short getMotors() const;
        bool changeSpeed(MOTORS motor, unsigned percentage);
        bool moveAll(unsigned base, unsigned shoulder, unsigned elbow, unsigned wristver, unsigned wristrot, unsigned gripper, const bool blocking = true, const bool degree = true);
        bool moveBase(unsigned base, const bool degree = true);
        bool moveShoulder(unsigned shoulder, const bool degree = true);
        bool moveElbow(unsigned elbow, const bool degree = true);
        bool moveWristVer(unsigned wirstver, const bool degree = true);
        bool moveWristRot(unsigned wirstrot, const bool degree = true);
        bool moveGripper(unsigned gripper, const bool degree = true);
        bool openGripper();
        bool closeGripper();
        bool isStanding() const;

        void pauseStop(RF24Network& network);

        bool record(RF24Network& network, const string filename = "test");
        bool takePicture(RaspiCam& cam, const string filename);
        bool readFromFile(const FILES filename);

        unsigned getExtremValue(MOTORS motor, EXTREM extrem);
        
        void light(const unsigned pin, const bool on);
        void surprise(RF24Network& network, SPEED speed = NORMAL);
        void angry(RF24Network& network, SPEED speed = NORMAL);
        void shy(RF24Network& network, SPEED speed = SLOW);
        void joy(RF24Network& network, SPEED speed = NORMAL);
  
        ~_BraccioNeo();
};

extern _BraccioNeo BraccioNeo;

#endif
