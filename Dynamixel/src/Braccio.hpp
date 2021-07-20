#ifndef __BRACCIO__
#define __BRACCIO__

#include <iostream>
#include <vector>

#include "Motor.hpp"
#include "MX106T.hpp"
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
        bool moveAll(const unsigned shoulder, const unsigned elbow, const unsigned wristver, const unsigned wristrot, const bool degree = true);
        bool moveShoulder(const unsigned shoulder, const bool degree = true);
        bool moveElbow(const unsigned elbow, const bool degree = true);
        bool moveWristVer(const unsigned wirstver, const bool degree = true);
        bool moveWristRot(const unsigned wirstrot, const bool degree = true);
        ~Braccio();
};


#endif