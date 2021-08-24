#ifndef __BRACCIO__
#define __BRACCIO__

#include <iostream>
#include <vector>
#include <fstream>

#include "Motor.hpp"
#include "MX106AT.hpp"
#include "MX28AT.hpp"
#include "MX64AT.hpp"
#include "AX18A.hpp"
#include "lib/RASPICAM/raspicam.h"
#include "AX12A.hpp"
#include "MX12W.hpp"


using namespace std;
using namespace raspicam;

enum MOTORS {BASE, SHOULDER, ELBOW, WRISTVER, WRISTROT, GRIPPER};
enum EXTREM {MINPOS, MINANGLE, MIDDLEPOS, MIDDLEANGLE, MAXPOS, MAXANGLE};

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
        
        short _NbMotors;
        short** _Limits;

    public:
        _BraccioNeo();
        void initValues();
        bool stand();
        bool Infos() const;
        const short getMotors() const;
        bool moveAll(const unsigned shoulder, const unsigned elbow, const unsigned wristver, const unsigned wristrot, const unsigned gripper, const bool degree = true);
        bool moveShoulder(const unsigned shoulder, const bool degree = true);
        bool moveElbow(const unsigned elbow, const bool degree = true);
        bool moveWristVer(const unsigned wirstver, const bool degree = true);
        #ifndef __APPLE__
            bool takePicture(RaspiCam& cam, string filename);
        #endif
        bool moveWristRot(const unsigned wirstrot, const bool degree = true);
        bool moveGripper(const unsigned gripper, const bool degree = true);
        ~_BraccioNeo();
};

extern _BraccioNeo BraccioNeo;

#endif
