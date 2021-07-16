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

/**
 * Moves all the motor
 * @param shoulder New position for the shoulder
 * @param elbow New position for the elbow
 * @param wristver New position for the wrist ver
 * @param wristrot New position for the wrist rot
 * @returns true if every motor moved well, else false
 */
bool Braccio::moveAll(const unsigned shoulder, const unsigned elbow, const unsigned wristver, const unsigned wristrot)
{
    //puts position in an array
    unsigned* position = new unsigned[_Motors.size()];
    position[SHOULDER] =  shoulder;
    position[ELBOW] = elbow;
    position[WRISTVER] = wristver;
    position[WRISTROT] = wristrot;

    //indicates if there is an error
    bool success = true;

    for (int i = 0; i < _Motors.size(); i++)   
    {
        success &= _Motors[i]->move(position[i]);
    }

    delete[] position;
    return success;
}

/**
 * Moves the shoulder
 * @param shoulder New position of the shoudler
 * @returns true if correctly moved, else false
 */
bool Braccio::moveShoulder(const unsigned shoulder)
{
    return _Motors[SHOULDER]->move(shoulder);
}

/**
 * Moves the elbow
 * @param elbow New position of the elbow
 * @returns true if correctly moved, else false
 */
bool Braccio::moveElbow(const unsigned elbow)
{
    return _Motors[ELBOW]->move(elbow);
}

/**
 * Makes the hand rises
 * @param wristver New position of the wrist vertically
 * @returns true if correctly moved, else false
 */
bool Braccio::moveWristVer(const unsigned wristver)
{
    return _Motors[WRISTVER]->move(wristver);
}

/**
 * Makes the hand turns
 * @param wristrot New position of the wrist 
 * @returns true if correctly moved, else false
 */
bool Braccio::moveWristRot(const unsigned wristrot)
{
    return _Motors[WRISTROT]->move(wristrot);
}

Braccio::~Braccio()
{
    for (char i = 0; i < _Motors.size(); i++)
    {
        delete _Motors[i];
    }
}

