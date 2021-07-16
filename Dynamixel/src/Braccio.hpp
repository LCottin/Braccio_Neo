#ifndef __BRACCIO__
#define __BRACCIO__

#include <iostream>
#include <vector>

#include "Motor.hpp"
#include "MX106.hpp"
#include "MX28AT.hpp"
#include "MX64AT.hpp"
#include "AX18A.hpp"

using namespace std;

enum MOTORS {SHOULDER, ELBOW, WRISTVER, WRISTROT};

class Braccio
{
    private:
        // Motor* _Shoulder;
        // Motor* _Elbow;
        // Motor* _WristVer;
        // Motor* _WristRot;
        vector<Motor*> _Motors;

    public:
        Braccio();
        bool stand();
        bool Infos();
        bool moveAll(const unsigned shoulder, const unsigned elbow, const unsigned wristver, const unsigned wristrot);
        bool moveShoulder(const unsigned shoulder);
        bool moveElbow(const unsigned elbow);
        bool moveWristVer(const unsigned wirstver);
        bool moveWristRot(const unsigned wirstrot);
        ~Braccio();
};


#endif