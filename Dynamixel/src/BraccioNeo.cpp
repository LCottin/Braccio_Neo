#include "BraccioNeo.hpp"

_BraccioNeo BraccioNeo;

_BraccioNeo::_BraccioNeo()
{
    //declares motors
    _Base       = new MX64AT(BASE);
    _Shoulder   = new MX64AT(SHOULDER);
    _Elbow      = new MX64AT(ELBOW);
    _WristVer   = new MX28AT(WRISTVER);
    _WristRot   = new AX12A(WRISTROT);
    _Gripper    = new MX12W(GRIPPER);
    
    //pushes them into the vector
    _Motors.push_back(_Base);
    _Motors.push_back(_Shoulder);
    _Motors.push_back(_Elbow);
    _Motors.push_back(_WristVer);
    _Motors.push_back(_WristRot);
    _Motors.push_back(_Gripper);
    
    initValues();
} 

/**
 * Initializes extrem values of each motor
 */
void _BraccioNeo::initValues()
{

    _NbMotors = (unsigned)_Motors.size();

    //creates an array to store current position of each motor
    _CurrentPosition = new unsigned[_NbMotors];
    for (int i = 0; i < (int)_NbMotors; i++)
    {
        _CurrentPosition[i] = _Motors[i]->getPosition();
    }

    //creates an array to store extrems positions of each motor
    _Limits = new unsigned* [_NbMotors];
    for (int i = 0; i < (int)_NbMotors; i++)
    {
        _Limits[i] = new unsigned[6];
    }

    //limits for the base
    _Limits[BASE][MINPOS]       = 0;
    _Limits[BASE][MINANGLE]     = 0;
    _Limits[BASE][MIDDLEPOS]    = 2047;
    _Limits[BASE][MIDDLEANGLE]  = 180;
    _Limits[BASE][MAXPOS]       = 4095;
    _Limits[BASE][MAXANGLE]     = 360;

    //limits for the shoulder
    _Limits[SHOULDER][MINPOS]       = 1024;
    _Limits[SHOULDER][MINANGLE]     = 90;
    _Limits[SHOULDER][MIDDLEPOS]    = 2047;
    _Limits[SHOULDER][MIDDLEANGLE]  = 180;
    _Limits[SHOULDER][MAXPOS]       = 3072;
    _Limits[SHOULDER][MAXANGLE]     = 270;

    //limits for the elbow
    _Limits[ELBOW][MINPOS]       = 683;
    _Limits[ELBOW][MINANGLE]     = 60;
    _Limits[ELBOW][MIDDLEPOS]    = 2047;
    _Limits[ELBOW][MIDDLEANGLE]  = 180;
    _Limits[ELBOW][MAXPOS]       = 3412;
    _Limits[ELBOW][MAXANGLE]     = 300;

    //limits for the wrist ver
    _Limits[WRISTVER][MINPOS]       = 683;
    _Limits[WRISTVER][MINANGLE]     = 60;
    _Limits[WRISTVER][MIDDLEPOS]    = 2047;
    _Limits[WRISTVER][MIDDLEANGLE]  = 180;
    _Limits[WRISTVER][MAXPOS]       = 3412;
    _Limits[WRISTVER][MAXANGLE]     = 300;

    //limits for the wrist rot
    _Limits[WRISTROT][MINPOS]       = 0;
    _Limits[WRISTROT][MINANGLE]     = 0;
    _Limits[WRISTROT][MIDDLEPOS]    = 512;
    _Limits[WRISTROT][MIDDLEANGLE]  = 180;
    _Limits[WRISTROT][MAXPOS]       = 1023;
    _Limits[WRISTROT][MAXANGLE]     = 300;
    
    //limits for the gripper
    _Limits[GRIPPER][MINPOS]       = 0;
    _Limits[GRIPPER][MINANGLE]     = 0;
    _Limits[GRIPPER][MIDDLEPOS]    = 512;
    _Limits[GRIPPER][MIDDLEANGLE]  = 180;
    _Limits[GRIPPER][MAXPOS]       = 1023;
    _Limits[GRIPPER][MAXANGLE]     = 300;

    cout << "Init values correctly done" << endl;
}

/**
 * Makes the arm stand
 * @returns true if the movment went right, else false
 */
bool _BraccioNeo::stand()
{
    for (int i = 0; i < (int)_NbMotors; i++)
    {
        //makes sure each movement is good
        if (_Motors[i]->middle() == false)
            return false;

        _CurrentPosition[i] = _Motors[i]->getPosition();
    }
    return true;
}

/**
 * Prints infos about each motor
 * @returns true if every info is pwell printed, else false
 */
bool _BraccioNeo::Infos() const
{
    bool success = true;
    for (int i = 0; i < (int)_NbMotors; i++)
    {
        success &= _Motors[i]->Infos();
    }
    return success;
}

/**
 * Returns how many motors are connected
 * @returns The number of motor
 */
short _BraccioNeo::getMotors() const
{
    return _NbMotors; 
}

/**
 * Returns extrem value available for a motor
 * @param motor Motor to get the extrem value
 * @param extrem What should be returned ?
 * @returns Extrem value of the given motor
 */
unsigned _BraccioNeo::getExtremValue(MOTORS motor, EXTREM extrem)
{
    return _Limits[motor][extrem];
}

/**
 * Changes motor speed
 * @param Motor choose motor
 * @param percentage New percentage speed (Warning ! Setting percentage to 0 set the motor speed to max speed) 
 * @returns true if every speed motor changed well, else false
 */
bool _BraccioNeo::changeSpeed(MOTORS motor, unsigned percentage)
{
    unsigned speed;
    switch(motor)
    {
        case BASE :
        case SHOULDER :
        case ELBOW :
        case WRISTVER :
        case WRISTROT :
        case GRIPPER :
            speed = mapping (percentage, 0, 100, 0, _Motors[motor]->getMaxSpeed());
            break;

        default : 
            cout << "Error : wrong motor selected !\n" << endl;
            return false;
    }
  return _Motors[motor]->setSpeed(speed);
}

/**
 * Moves all the motor
 * @param base New position for the base
 * @param shoulder New position for the shoulder
 * @param elbow New position for the elbow
 * @param wristver New position for the wrist ver
 * @param wristrot New position for the wrist rot
 * @returns true if every motor moved well, else false
 */

bool _BraccioNeo::moveAll(unsigned base, unsigned shoulder, unsigned elbow, unsigned wristver, unsigned wristrot, unsigned gripper, const bool blocking, const bool degree)
{
    //maps positions if they are given in degree
    if (degree)
    {
        _CurrentPosition[BASE]      = (base < _Limits[BASE][MINANGLE]) ? _Limits[BASE][MINANGLE] : base;
        _CurrentPosition[BASE]      = (base > _Limits[BASE][MAXANGLE]) ? _Limits[BASE][MAXANGLE] : base;

        _CurrentPosition[SHOULDER]  = (shoulder < _Limits[SHOULDER][MINANGLE]) ? _Limits[SHOULDER][MINANGLE] : shoulder;
        _CurrentPosition[SHOULDER]  = (shoulder > _Limits[SHOULDER][MAXANGLE]) ? _Limits[SHOULDER][MAXANGLE] : shoulder;

        _CurrentPosition[ELBOW]     = (elbow < _Limits[ELBOW][MINANGLE]) ? _Limits[ELBOW][MINANGLE] : elbow;
        _CurrentPosition[ELBOW]     = (elbow > _Limits[ELBOW][MAXANGLE]) ? _Limits[ELBOW][MAXANGLE] : elbow;

        _CurrentPosition[WRISTVER]  = (wristver < _Limits[WRISTVER][MINANGLE]) ? _Limits[WRISTVER][MINANGLE] : wristver;
        _CurrentPosition[WRISTVER]  = (wristver > _Limits[WRISTVER][MAXANGLE]) ? _Limits[WRISTVER][MAXANGLE] : wristver;

        _CurrentPosition[WRISTROT]  = (wristrot < _Limits[WRISTROT][MINANGLE]) ? _Limits[WRISTROT][MINANGLE] : wristrot;
        _CurrentPosition[WRISTROT]  = (wristrot > _Limits[WRISTROT][MAXANGLE]) ? _Limits[WRISTROT][MAXANGLE] : wristrot;

        _CurrentPosition[GRIPPER]   = (gripper < _Limits[GRIPPER][MINANGLE]) ? _Limits[GRIPPER][MINANGLE] : gripper;
        _CurrentPosition[GRIPPER]   = (gripper > _Limits[GRIPPER][MAXANGLE]) ? _Limits[GRIPPER][MAXANGLE] : gripper;
    }
    else
    {
        _CurrentPosition[BASE]      = (base < _Limits[BASE][MINPOS]) ? _Limits[BASE][MINPOS] : base;
        _CurrentPosition[BASE]      = (base > _Limits[BASE][MAXPOS]) ? _Limits[BASE][MAXPOS] : base;

        _CurrentPosition[SHOULDER]  = (shoulder < _Limits[SHOULDER][MINPOS]) ? _Limits[SHOULDER][MINPOS] : shoulder;
        _CurrentPosition[SHOULDER]  = (shoulder > _Limits[SHOULDER][MAXPOS]) ? _Limits[SHOULDER][MAXPOS] : shoulder;

        _CurrentPosition[ELBOW]     = (elbow < _Limits[ELBOW][MINPOS]) ? _Limits[ELBOW][MINPOS] : elbow;
        _CurrentPosition[ELBOW]     = (elbow > _Limits[ELBOW][MAXPOS]) ? _Limits[ELBOW][MAXPOS] : elbow;

        _CurrentPosition[WRISTVER]  = (wristver < _Limits[WRISTVER][MINPOS]) ? _Limits[WRISTVER][MINPOS] : wristver;
        _CurrentPosition[WRISTVER]  = (wristver > _Limits[WRISTVER][MAXPOS]) ? _Limits[WRISTVER][MAXPOS] : wristver;

        _CurrentPosition[WRISTROT]  = (wristrot < _Limits[WRISTROT][MINPOS]) ? _Limits[WRISTROT][MINPOS] : wristrot;
        _CurrentPosition[WRISTROT]  = (wristrot > _Limits[WRISTROT][MAXPOS]) ? _Limits[WRISTROT][MAXPOS] : wristrot;

        _CurrentPosition[GRIPPER]   = (gripper < _Limits[GRIPPER][MINPOS]) ? _Limits[GRIPPER][MINPOS] : gripper;
        _CurrentPosition[GRIPPER]   = (gripper > _Limits[GRIPPER][MAXPOS]) ? _Limits[GRIPPER][MAXPOS] : gripper;
    }

    //indicates if there is an error
    bool success = true;
    for (int i = 0; i < (int)_Motors.size(); i++)   
    {
	    success &= _Motors[i]->move(_CurrentPosition[i], degree, blocking);
    }

    return success;
}

/**
 * Moves the base
 * @param base New position of the base
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveBase(unsigned base, const bool degree)
{
    if (degree)
    {
        _CurrentPosition[BASE] = (base < _Limits[BASE][MINANGLE]) ? _Limits[BASE][MINANGLE] : base;
        _CurrentPosition[BASE] = (base > _Limits[BASE][MAXANGLE]) ? _Limits[BASE][MAXANGLE] : base;
    }
    else 
    {
        _CurrentPosition[BASE] = (base < _Limits[BASE][MINPOS]) ? _Limits[BASE][MINPOS] : base;
        _CurrentPosition[BASE] = (base > _Limits[BASE][MAXPOS]) ? _Limits[BASE][MAXPOS] : base;
    }
    return _Motors[BASE]->move(_CurrentPosition[BASE], degree);
}

/**
 * Moves the shoulder
 * @param shoulder New position of the shoudler
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveShoulder(unsigned shoulder, const bool degree)
{
    if (degree)
    {
        _CurrentPosition[SHOULDER] = (shoulder < _Limits[SHOULDER][MINANGLE]) ? _Limits[SHOULDER][MINANGLE] : shoulder;
        _CurrentPosition[SHOULDER] = (shoulder > _Limits[SHOULDER][MAXANGLE]) ? _Limits[SHOULDER][MAXANGLE] : shoulder;
    }
    else 
    {
        _CurrentPosition[SHOULDER] = (shoulder < _Limits[SHOULDER][MINPOS]) ? _Limits[SHOULDER][MINPOS] : shoulder;
        _CurrentPosition[SHOULDER] = (shoulder > _Limits[SHOULDER][MAXPOS]) ? _Limits[SHOULDER][MAXPOS] : shoulder;
    }
    return _Motors[SHOULDER]->move(_CurrentPosition[SHOULDER], degree);
}

/**
 * Moves the elbow
 * @param elbow New position of the elbow
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveElbow(unsigned elbow, const bool degree)
{
    if (degree)
    {
        _CurrentPosition[ELBOW] = (elbow < _Limits[ELBOW][MINANGLE]) ? _Limits[ELBOW][MINANGLE] : elbow;
        _CurrentPosition[ELBOW] = (elbow > _Limits[ELBOW][MAXANGLE]) ? _Limits[ELBOW][MAXANGLE] : elbow;
    }
    else 
    {
        _CurrentPosition[ELBOW] = (elbow < _Limits[ELBOW][MINPOS]) ? _Limits[ELBOW][MINPOS] : elbow;
        _CurrentPosition[ELBOW] = (elbow > _Limits[ELBOW][MAXPOS]) ? _Limits[ELBOW][MAXPOS] : elbow;
    }
    return _Motors[ELBOW]->move(_CurrentPosition[ELBOW], degree);
}

/**
 * Makes the hand rises
 * @param wristver New position of the wrist vertically
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveWristVer(unsigned wristver, const bool degree)
{
    if (degree)
    {
        _CurrentPosition[WRISTVER] = (wristver < _Limits[WRISTVER][MINANGLE]) ? _Limits[WRISTVER][MINANGLE] : wristver;
        _CurrentPosition[WRISTVER] = (wristver > _Limits[WRISTVER][MAXANGLE]) ? _Limits[WRISTVER][MAXANGLE] : wristver;
    }
    else 
    {
        _CurrentPosition[WRISTVER] = (wristver < _Limits[WRISTVER][MINPOS]) ? _Limits[WRISTVER][MINPOS] : wristver;
        _CurrentPosition[WRISTVER] = (wristver > _Limits[WRISTVER][MAXPOS]) ? _Limits[WRISTVER][MAXPOS] : wristver;
    }
    return _Motors[WRISTVER]->move(_CurrentPosition[WRISTVER], degree);
}

/**
 * Makes the hand turns
 * @param wristrot New position of the wrist 
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveWristRot(unsigned wristrot, const bool degree)
{
    if (degree)
    {
        _CurrentPosition[WRISTROT] = (wristrot < _Limits[WRISTROT][MINANGLE]) ? _Limits[WRISTROT][MINANGLE] : wristrot;
        _CurrentPosition[WRISTROT] = (wristrot > _Limits[WRISTROT][MAXANGLE]) ? _Limits[WRISTROT][MAXANGLE] : wristrot;
    }
    else 
    {
        _CurrentPosition[WRISTROT] = (wristrot < _Limits[WRISTROT][MINPOS]) ? _Limits[WRISTROT][MINPOS] : wristrot;
        _CurrentPosition[WRISTROT] = (wristrot > _Limits[WRISTROT][MAXPOS]) ? _Limits[WRISTROT][MAXPOS] : wristrot;
    }
    return _Motors[WRISTROT]->move(_CurrentPosition[WRISTROT], degree);
}

/*
 * Makes the gripper catch
 * @param gripper New position of the wrist 
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveGripper(unsigned gripper, const bool degree)
{
    if (degree)
    {
        _CurrentPosition[GRIPPER] = (gripper < _Limits[GRIPPER][MINANGLE]) ? _Limits[GRIPPER][MINANGLE] : gripper;
        _CurrentPosition[GRIPPER] = (gripper > _Limits[GRIPPER][MAXANGLE]) ? _Limits[GRIPPER][MAXANGLE] : gripper;
    }
    else 
    {
        _CurrentPosition[GRIPPER] = (gripper < _Limits[GRIPPER][MINPOS]) ? _Limits[GRIPPER][MINPOS] : gripper;
        _CurrentPosition[GRIPPER] = (gripper > _Limits[GRIPPER][MAXPOS]) ? _Limits[GRIPPER][MAXPOS] : gripper;
    }
  return _Motors[GRIPPER]->move(_CurrentPosition[GRIPPER], degree);
}

/**
 * Plays the angry emotion
 */
void _BraccioNeo::angry(SPEED speed)
{
    _Start = clock();
    _Speed = speed;

    for (unsigned i = 0; i < _NbMotors; i++)
    {
        changeSpeed((MOTORS)i, _Speed);
    }
    
    moveShoulder(240, _Speed);
    usleep(10 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    usleep(20 * MILLISECOND);
    moveWristVer(205);

    for (int i = 0; i < 3; i++)
    {
        /*
        openGripper();
        usleep(10 * MILLISECOND);
        closeGripper();
        usleep(10 * MILLISECOND);
        */
    }
    // moveGripper(150);

    moveShoulder(120);
    usleep(10 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(120, 200, 170, 240, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(180, 160, 150, 180, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(210, 120, 180, 180, 150, 150);
    usleep(100 * MILLISECOND);

    moveShoulder(240);
    usleep(10 * MILLISECOND);
    moveWristVer(240);
    usleep(50 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);

    usleep(10 * MILLISECOND);
    stand();
    usleep(5000 * MILLISECOND);

    moveShoulder(240);
    usleep(10 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    usleep(20 * MILLISECOND);
    moveWristVer(205);
    usleep(10 * MILLISECOND);

    for (int i = 0; i < 3; i++)
    {
        /*
        openGripper();
        usleep(10 * MILLISECOND);
        closeGripper();
        usleep(10 * MILLISECOND);
        */
    }
    // moveGripper(150);

    moveShoulder(120);
    usleep(10 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(120, 200, 170, 240, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(180, 160, 150, 180, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(210, 120, 180, 180, 150, 150);
    usleep(100 * MILLISECOND);

    moveShoulder(240);
    usleep(10 * MILLISECOND);
    moveWristVer(150);
    usleep(50 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);

    usleep(10 * MILLISECOND);
    stand();
    usleep(5000 * MILLISECOND);

    moveShoulder(240);
    usleep(10 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    usleep(20 * MILLISECOND);
    moveWristVer(205);
    usleep(10 * MILLISECOND);

    for (int i = 0; i < 3; i++)
    {
        /*
        openGripper();
        usleep(10 * MILLISECOND);
        closeGripper();
        usleep(10 * MILLISECOND);
        */
    }
    // moveGripper(150);

    moveShoulder(250);
    usleep(10 * MILLISECOND);
    moveWristVer(240);
    usleep(50 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    usleep(10 * MILLISECOND);
    stand();
    usleep(5000 * MILLISECOND);

    moveShoulder(240);
    usleep(10 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    usleep(20 * MILLISECOND);
    moveWristVer(205);
    usleep(10 * MILLISECOND);

    for (int i = 0; i < 3; i++)
    {
        /*
        openGripper();
        usleep(10 * MILLISECOND);
        closeGripper();
        usleep(10 * MILLISECOND);
        */
    }
    // moveGripper(150);

    moveShoulder(120);
    usleep(10 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(120, 200, 170, 240, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(180, 160, 150, 180, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(210, 120, 180, 180, 150, 150);
    usleep(100 * MILLISECOND);

    moveShoulder(250);
    usleep(10 * MILLISECOND);
    moveWristVer(240);
    usleep(50 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);

    usleep(1000 * MILLISECOND);
    stand();
    usleep(1000 * MILLISECOND);

    moveShoulder(240);
    usleep(10 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    usleep(20 * MILLISECOND);
    moveWristVer(205);
    usleep(10 * MILLISECOND);

    for (int i = 0; i < 3; i++)
    {
        /*
        openGripper();
        usleep(10 * MILLISECOND);
        closeGripper();
        usleep(10 * MILLISECOND);
        */
    }
    // moveGripper(150);

    moveShoulder(120);
    usleep(10 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(120, 200, 170, 240, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(180, 160, 150, 180, 150, 150);
    usleep(30 * MILLISECOND);
    moveAll(210, 120, 180, 180, 150, 150);
    usleep(100 * MILLISECOND);

    moveShoulder(250);
    usleep(10 * MILLISECOND);
    moveWristVer(240);
    usleep(50 * MILLISECOND);
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);

    usleep(1000 * MILLISECOND);
    stand();

    _Stop = clock();
    long double time = (_Stop - _Start) / CLOCKS_PER_SEC;
    cout << "Angry emotion lasted " << time << "seconds." << endl;
}

/**
 * Plays the surprise emotion
 */
void _BraccioNeo::surprise(SPEED speed)
{
    _Start = clock();
    _Speed = speed;

    for (unsigned i = 0; i < _NbMotors; i++)
    {
        changeSpeed((MOTORS)i, _Speed);
    }
  
    stand();

    moveWristRot(60);
    usleep(500 * MILLISECOND);
    moveWristRot(240);
    usleep(500 * MILLISECOND);
    moveWristRot(150);
    usleep(500 * MILLISECOND);

    moveWristVer(110);
    usleep(500 * MILLISECOND);
    moveWristRot(60);
    usleep(500 * MILLISECOND);
    moveWristVer(180);
    usleep(500 * MILLISECOND);
    
    moveElbow(110);
    moveWristVer(270);
    stand();
    moveBase(150);
    usleep(500 * MILLISECOND);

    stand();
    usleep(500 * MILLISECOND);
    //openGripper();
    usleep(500 * MILLISECOND);
    //closeGripper();
  
    usleep(1000 * MILLISECOND);
    stand();

    _Stop = clock();
    long double time = (_Stop - _Start) / CLOCKS_PER_SEC;
    cout << "Surprise emotion lasted " << time << "seconds." << endl;
}

/*
 * Plays the shy emotion
 */
void _BraccioNeo::shy(SPEED speed)
{
    _Start = clock();
    _Speed = speed;

    for (unsigned i = 0; i < _NbMotors; i++)
    {
        changeSpeed((MOTORS)i, _Speed);
    }

    for (int i = 0; i < 3; i++)
    {
        /*
        openGripper();
        usleep(50 * MILLISECOND);
        closeGripper();
        usleep(50 * MILLISECOND);
        */
    }
    // moveGripper(150);

    for (int i = 0; i < 3; i++)
    {
        moveBase(180);
        usleep(20 * MILLISECOND);
        moveBase(230);
        usleep(20 * MILLISECOND);
    }
    moveBase(180);
    usleep(200 * MILLISECOND);

    for (int i = 0; i < 2; i++)
    {
        moveAll(_CurrentPosition[BASE], 230, 115, 240, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        usleep(200 * MILLISECOND);
        moveAll(_CurrentPosition[BASE], 140, 265, 130, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        usleep(200 * MILLISECOND);
    }

    usleep(1000 * MILLISECOND);
    stand();
    usleep(5000 * MILLISECOND);
    
    for (int i = 0; i < 3; i++)
    {
        /*
        openGripper();
        usleep(50 * MILLISECOND);
        closeGripper();
        usleep(50 * MILLISECOND);
        */
    }
    // moveGripper(150);
    
    for (int i = 0; i < 3; i++)
    {
        moveBase(180);
        usleep(20 * MILLISECOND);
        moveBase(230);
        usleep(20 * MILLISECOND);
    }
    moveBase(180);
    usleep(200 * MILLISECOND);

    for (int i = 0; i < 2; i++)
    {
        moveAll(_CurrentPosition[BASE], 230, 115, 240, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        usleep(200 * MILLISECOND);
        moveAll(_CurrentPosition[BASE], 140, 265, 130, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        usleep(200 * MILLISECOND);
    }

    usleep(1000 * MILLISECOND);
    stand();
    usleep(5000 * MILLISECOND);

    for (int i = 0; i < 3; i++)
    {
        /*
        openGripper();
        usleep(50 * MILLISECOND);
        closeGripper();
        usleep(50 * MILLISECOND);
        */
    }
    // moveGripper(150);
    
    for (int i = 0; i < 3; i++)
    {
        moveBase(180);
        usleep(20 * MILLISECOND);
        moveBase(230);
        usleep(20 * MILLISECOND);
    }
    moveBase(180);
    usleep(200 * MILLISECOND);

    for (int i = 0; i < 2; i++)
    {
        moveAll(_CurrentPosition[BASE], 230, 115, 240, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        usleep(200 * MILLISECOND);
        moveAll(_CurrentPosition[BASE], 140, 265, 130, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        usleep(200 * MILLISECOND);
    }

    usleep(1000 * MILLISECOND);
    stand();
    usleep(5000 * MILLISECOND);

    for (int i = 0; i < 3; i++)
    {
        /*
        openGripper();
        usleep(50 * MILLISECOND);
        closeGripper();
        usleep(50 * MILLISECOND);
        */
    }
    // moveGripper(150);
    
    for (int i = 0; i < 3; i++)
    {
        moveBase(180);
        usleep(20 * MILLISECOND);
        moveBase(230);
        usleep(20 * MILLISECOND);
    }
    moveBase(180);
    usleep(200 * MILLISECOND);

    for (int i = 0; i < 2; i++)
    {
        moveAll(_CurrentPosition[BASE], 230, 115, 240, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        usleep(200 * MILLISECOND);
        moveAll(_CurrentPosition[BASE], 140, 265, 130, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        usleep(200 * MILLISECOND);
    }
   
    usleep(1000 * MILLISECOND);
    stand();

    _Stop = clock();
    long double time = (_Stop - _Start) / CLOCKS_PER_SEC;
    cout << "Shy emotion lasted " << time << "seconds." << endl;
}

#ifndef __APPLE__
/**
 * Takes a picture and saves it
 * @param cam Camera to take the picture with
 * @param filename Name of the saved picture
 * @returns true if successfully taken and saved, else false
 */
bool _BraccioNeo::takePicture(RaspiCam& cam, string filename)
{
    if (cam.open() == false)
    {
        cout << "Erreur lors de l'ouverture" << endl;
        return false;
    }

	//takes picture
	cam.grab();

	unsigned char* data;
    data = new unsigned char [cam.getImageTypeSize(RASPICAM_FORMAT_RGB) ];

	//extracts image
	cam.retrieve(data, RASPICAM_FORMAT_RGB);

    //saves it
    filename += ".jpg";
	ofstream outfile(filename, std::ios::binary);
	outfile << "P6\n" << cam.getWidth() << " " << cam.getHeight() << " 255\n";
	outfile.write( (char*)data, cam.getImageTypeSize(RASPICAM_FORMAT_RGB) );
	cout << "Image saved" << endl;
	
    delete[] data;
	return true;
}
#endif

_BraccioNeo::~_BraccioNeo()
{
    for (int i = 0; i < (int)_NbMotors; i++)
    {
        delete _Limits[i];
        delete _Motors[i];
    }
    delete _Limits;
    delete _CurrentPosition;
}

