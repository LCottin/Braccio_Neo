#ifndef __BRACCIO__
#define __BRACCIO__

#include <iostream>
#include <vector>

#include "Motor.hpp"
#include "MX106T.hpp"
#include "MX28AT.hpp"
#include "MX64AT.hpp"
#include "AX18A.hpp"
#include "AX12A.hpp"
#include "MX12W.hpp"

using namespace std;

enum MOTORS {SHOULDER, ELBOW, WRISTVER, WRISTROT, GRIPPER};
enum MINMAX {MIN, MAX};

class _BraccioNeo
{
    private:
        // Motor* _Shoulder;
        // Motor* _Elbow;
        // Motor* _WristVer;
        // Motor* _WristRot;
        vector<Motor*> _Motors;
        short _NbMotors;

    public:
        _BraccioNeo();
        bool stand();
        bool Infos() const;
        const short getMotors() const;
        bool moveAll(const unsigned shoulder, const unsigned elbow, const unsigned wristver, const unsigned wristrot, const unsigned gripper, const bool degree = true);
        bool moveShoulder(const unsigned shoulder, const bool degree = true);
        bool moveElbow(const unsigned elbow, const bool degree = true);
        bool moveWristVer(const unsigned wirstver, const bool degree = true);
        bool moveWristRot(const unsigned wirstrot, const bool degree = true);
        bool moveGripper(const unsigned gripper, const bool degree = true);
        ~_BraccioNeo();
};

extern _BraccioNeo BraccioNeo;

#endif
