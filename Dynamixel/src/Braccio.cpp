#include "Braccio.hpp"


Braccio::Braccio()
{
    //declares motors
    MX64AT* _Shoulder   = new MX64AT(SHOULDER);
    MX64AT* _Elbow      = new MX64AT(ELBOW);
    MX28AT* _WristVer   = new MX28AT(WRISTVER);
    AX18A*  _WristRot   = new AX18A(WRISTROT);

    //pushes them into the vector
    _Motors.push_back(_Shoulder);
    _Motors.push_back(_Elbow);
    _Motors.push_back(_WristVer);
    _Motors.push_back(_WristRot);
} 

/**
 * Makes the arm stand
 * @returns true if the movment went right, else false
 */
bool Braccio::stand()
{
    for (char i = 0; i < _Motors.size(); i++)
    {
        //makes sure each movement is good
        if (_Motors[i]->middle() == false)
            return false;
    }
    return true;
}

Braccio::~Braccio()
{
    for (char i = 0; i < _Motors.size(); i++)
    {
        delete _Motors[i];
    }
}