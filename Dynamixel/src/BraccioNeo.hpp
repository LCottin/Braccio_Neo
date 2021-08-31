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
#include "lib/RASPICAM/raspicam.h"
#include "AX12A.hpp"
#include "MX12W.hpp"

#define MILLISECOND 1000

using namespace std;
using namespace raspicam;

enum MOTORS {BASE, SHOULDER, ELBOW, WRISTVER, WRISTROT, GRIPPER};
enum EXTREM {MINPOS, MINANGLE, MIDDLEPOS, MIDDLEANGLE, MAXPOS, MAXANGLE};
enum SPEED {V_SLOW = 10, SLOW = 25, NORMAL = 50, FAST = 75, V_FAST = 90};

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

        clock_t _Start;
        clock_t _Stop;
        SPEED _Speed;  

        unsigned* _CurrentPosition;

    public:
        _BraccioNeo();
        void initValues();
        bool stand();
        bool Infos() const;
        short getMotors() const;
        bool changeSpeed(MOTORS Motor, unsigned percentage);
        bool moveAll(unsigned base, unsigned shoulder, unsigned elbow, unsigned wristver, unsigned wristrot, unsigned gripper, const bool blocking = true, const bool degree = true);
        bool moveBase(unsigned base, const bool degree = true);
        bool moveShoulder(unsigned shoulder, const bool degree = true);
        bool moveElbow(unsigned elbow, const bool degree = true);
        bool moveWristVer(unsigned wirstver, const bool degree = true);
        bool moveWristRot(unsigned wirstrot, const bool degree = true);
        bool moveGripper(unsigned gripper, const bool degree = true);
        
        void angry();
        void surprise(SPEED speed = NORMAL);

        #ifndef __APPLE__
            bool takePicture(RaspiCam& cam, string filename);
        #endif
        ~_BraccioNeo();
};

extern _BraccioNeo BraccioNeo;

#endif
