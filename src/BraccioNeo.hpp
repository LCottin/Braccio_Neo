#ifndef __BRACCIO__
#define __BRACCIO__

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <unistd.h>

#include "Motor.hpp"
#include "MX106AT.hpp"
#include "MX28AT.hpp"
#include "MX64AT.hpp"
#include "AX18A.hpp"
#include "AX12A.hpp"
#include "MX12W.hpp"
// #include "Variables.hpp"
#include "lib/RASPICAM/raspicam.h"
#include "lib/RF24/RF24.h"
#include "lib/RF24/nRF24L01.h"
#include "lib/RF24/RF24Network.h"

#define MILLISECOND 1000

using namespace std;
using namespace raspicam;

// ---------------------------------------- //
// -             ENUMERATIONS             - //
// ---------------------------------------- //
//Tells which bracelet is speaking and with which order
enum BRACELETS  {EMITTER1, EMITTER2, EMITTER3, Telecommande};
enum ACTIONS    {PLAY = 11, PAUSE = 12, STOP = 13};
enum MODES      {ANGRY = 20, JOY = 21, SURPRISE = 22, SHY = 23, CONTROL = 24, NONE = 25};
enum MOTORS {BASE, SHOULDER, ELBOW, WRISTVER, WRISTROT, GRIPPER};
enum EXTREM {MINPOS, MINANGLE, MIDDLEPOS, MIDDLEANGLE, MAXPOS, MAXANGLE};
enum SPEED {V_SLOW = 10, SLOW = 25, NORMAL = 50, FAST = 75, V_FAST = 90};

struct data2
{
	short ID;
	short x;
	short y;
	char mode;
	char action;
} databraccio;

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
        bool isStanding() const;
        bool record(RF24Network& network, const bool replay = false, const bool save = true, const string filename = "test");

        unsigned getExtremValue(MOTORS motor, EXTREM extrem);
        
        void surprise(SPEED speed = NORMAL);
        void angry(SPEED speed = NORMAL);
        void shy(SPEED speed = SLOW);
        void joy(SPEED speed = NORMAL);
  
        #ifndef __APPLE__
            bool takePicture(RaspiCam& cam, const string filename);
        #endif
        ~_BraccioNeo();
};

extern _BraccioNeo BraccioNeo;

#endif
