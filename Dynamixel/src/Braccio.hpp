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
        Motor* _Shoulder;
        Motor* _Elbow;
        Motor* _WristVer;
        Motor* _WristRot;
        vector<Motor*> _Motors;

    public:
        Braccio();
        ~Braccio();
};


#endif