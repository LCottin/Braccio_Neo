#include "Braccio.hpp"


Braccio::Braccio()
{
    //declares motors
    _Shoulder   = new MX106(SHOULDER);
    _Elbow      = new MX64AT(ELBOW);
    _WristVer   = new MX28AT(WRISTVER);
    _WristRot   = new AX18A(WRISTROT);

    //pushes them into the vector
    _Motors.push_back(_Shoulder);
    _Motors.push_back(_Elbow);
    _Motors.push_back(_WristVer);
    _Motors.push_back(_WristRot);
} 

Braccio::~Braccio()
{
    for (char i = 0; i < _Motors.size(); i++)
    {
        delete _Motors[i];
    }
}