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

    _NbMotors = (unsigned)_Motors.size();

    initValues();
} 

/**
 * Initializes extrem values of each motor
 */
void _BraccioNeo::initValues()
{
    //creates an array to store extrems positions of each motor
    _Limits = new unsigned* [_NbMotors];
    for (int i = 0; i < _NbMotors; i++)
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
    for (char i = 0; i < _Motors.size(); i++)
    {
        //makes sure each movement is good
        if (_Motors[i]->middle() == false)
            return false;
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
    for (char i = 0; i < _Motors.size(); i++)
    {
        success &= _Motors[i]->Infos();
    }
    return success;
}

/**
 * Returns how many motors are connected
 * @returns The number of motor
 */
const short _BraccioNeo::getMotors() const
{
    return _NbMotors; 
}

/**
 * Changes motor speed
 * @param Motor choose motor
 * @param percentage New percentage speed (Warning ! Setting percentage to 0 set the motor speed to max speed) 
 * @returns true if every speed motor changed well, else false
 */
bool _BraccioNeo::setSpeed(MOTORS Motor, unsigned percentage)
{
    unsigned speed;
    switch(Motor)
    {
        case BASE :
        case SHOULDER :
        case ELBOW :
        case WRISTVER :
        case WRISTROT :
        case GRIPPER :
            speed = mapping (percentage, 0, 100, 0, _Motors[Motor]->getMaxSpeed());
            break;

        default : 
            cout << "Error : wrong motor selected !\n" << endl;
            return false;
    }
  return _Motors[Motor]->setSpeed(speed);
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
    //puts position in an array
    unsigned* position  = new unsigned[_Motors.size()];

    //maps positions if they are given in degree
    if (degree)
    {
        base = (base < _Limits[BASE][MINANGLE]) ? _Limits[BASE][MINANGLE] : base;
        base = (base > _Limits[BASE][MAXANGLE]) ? _Limits[BASE][MAXANGLE] : base;

        shoulder    = (shoulder < _Limits[SHOULDER][MINANGLE]) ? _Limits[SHOULDER][MINANGLE] : shoulder;
        shoulder    = (shoulder > _Limits[SHOULDER][MAXANGLE]) ? _Limits[SHOULDER][MAXANGLE] : shoulder;

        elbow       = (elbow < _Limits[ELBOW][MINANGLE]) ? _Limits[ELBOW][MINANGLE] : elbow;
        elbow       = (elbow > _Limits[ELBOW][MAXANGLE]) ? _Limits[ELBOW][MAXANGLE] : elbow;

        wristver    = (wristver < _Limits[WRISTVER][MINANGLE]) ? _Limits[WRISTVER][MINANGLE] : wristver;
        wristver    = (wristver > _Limits[WRISTVER][MAXANGLE]) ? _Limits[WRISTVER][MAXANGLE] : wristver;

        wristrot    = (wristrot < _Limits[WRISTROT][MINANGLE]) ? _Limits[WRISTROT][MINANGLE] : wristrot;
        wristrot    = (wristrot > _Limits[WRISTROT][MAXANGLE]) ? _Limits[WRISTROT][MAXANGLE] : wristrot;

        gripper     = (gripper < _Limits[GRIPPER][MINANGLE]) ? _Limits[GRIPPER][MINANGLE] : gripper;
        gripper     = (gripper > _Limits[GRIPPER][MAXANGLE]) ? _Limits[GRIPPER][MAXANGLE] : gripper;
    }
    else
    {
        base = (base < _Limits[BASE][MINPOS]) ? _Limits[BASE][MINPOS] : base;
        base = (base > _Limits[BASE][MAXPOS]) ? _Limits[BASE][MAXPOS] : base;

        shoulder    = (shoulder < _Limits[SHOULDER][MINPOS]) ? _Limits[SHOULDER][MINPOS] : shoulder;
        shoulder    = (shoulder > _Limits[SHOULDER][MAXPOS]) ? _Limits[SHOULDER][MAXPOS] : shoulder;

        elbow       = (elbow < _Limits[ELBOW][MINPOS]) ? _Limits[ELBOW][MINPOS] : elbow;
        elbow       = (elbow > _Limits[ELBOW][MAXPOS]) ? _Limits[ELBOW][MAXPOS] : elbow;

        wristver    = (wristver < _Limits[WRISTVER][MINPOS]) ? _Limits[WRISTVER][MINPOS] : wristver;
        wristver    = (wristver > _Limits[WRISTVER][MAXPOS]) ? _Limits[WRISTVER][MAXPOS] : wristver;

        wristrot    = (wristrot < _Limits[WRISTROT][MINPOS]) ? _Limits[WRISTROT][MINPOS] : wristrot;
        wristrot    = (wristrot > _Limits[WRISTROT][MAXPOS]) ? _Limits[WRISTROT][MAXPOS] : wristrot;

        gripper     = (gripper < _Limits[GRIPPER][MINPOS]) ? _Limits[GRIPPER][MINPOS] : gripper;
        gripper     = (gripper > _Limits[GRIPPER][MAXPOS]) ? _Limits[GRIPPER][MAXPOS] : gripper;
    }

    position[BASE]      = base;
    position[SHOULDER]  = shoulder;
    position[ELBOW]     = elbow;
    position[WRISTVER]  = wristver;
    position[WRISTROT]  = wristrot;
    position[GRIPPER]   = gripper;

    //indicates if there is an error
    bool success = true;
    for (int i = 0; i < (int)_Motors.size(); i++)   
    {
	    success &= _Motors[i]->move(position[i], degree, blocking);
    }

    delete[] position;
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
        base = (base < _Limits[BASE][MINANGLE]) ? _Limits[BASE][MINANGLE] : base;
        base = (base > _Limits[BASE][MAXANGLE]) ? _Limits[BASE][MAXANGLE] : base;
    }
    else 
    {
        base = (base < _Limits[BASE][MINPOS]) ? _Limits[BASE][MINPOS] : base;
        base = (base > _Limits[BASE][MAXPOS]) ? _Limits[BASE][MAXPOS] : base;
    }
    return _Motors[BASE]->move(base, degree);
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
        shoulder = (shoulder < _Limits[SHOULDER][MINANGLE]) ? _Limits[SHOULDER][MINANGLE] : shoulder;
        shoulder = (shoulder > _Limits[SHOULDER][MAXANGLE]) ? _Limits[SHOULDER][MAXANGLE] : shoulder;
    }
    else 
    {
        shoulder = (shoulder < _Limits[SHOULDER][MINPOS]) ? _Limits[SHOULDER][MINPOS] : shoulder;
        shoulder = (shoulder > _Limits[SHOULDER][MAXPOS]) ? _Limits[SHOULDER][MAXPOS] : shoulder;
    }
    return _Motors[SHOULDER]->move(shoulder, degree);
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
        elbow = (elbow < _Limits[ELBOW][MINANGLE]) ? _Limits[ELBOW][MINANGLE] : elbow;
        elbow = (elbow > _Limits[ELBOW][MAXANGLE]) ? _Limits[ELBOW][MAXANGLE] : elbow;
    }
    else 
    {
        elbow = (elbow < _Limits[ELBOW][MINPOS]) ? _Limits[ELBOW][MINPOS] : elbow;
        elbow = (elbow > _Limits[ELBOW][MAXPOS]) ? _Limits[ELBOW][MAXPOS] : elbow;
    }
    return _Motors[ELBOW]->move(elbow, degree);
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
        wristver = (wristver < _Limits[WRISTVER][MINANGLE]) ? _Limits[WRISTVER][MINANGLE] : wristver;
        wristver = (wristver > _Limits[WRISTVER][MAXANGLE]) ? _Limits[WRISTVER][MAXANGLE] : wristver;
    }
    else 
    {
        wristver = (wristver < _Limits[WRISTVER][MINPOS]) ? _Limits[WRISTVER][MINPOS] : wristver;
        wristver = (wristver > _Limits[WRISTVER][MAXPOS]) ? _Limits[WRISTVER][MAXPOS] : wristver;
    }
    return _Motors[WRISTVER]->move(wristver, degree);
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
        wristrot = (wristrot < _Limits[WRISTROT][MINANGLE]) ? _Limits[WRISTROT][MINANGLE] : wristrot;
        wristrot = (wristrot > _Limits[WRISTROT][MAXANGLE]) ? _Limits[WRISTROT][MAXANGLE] : wristrot;
    }
    else 
    {
        wristrot = (wristrot < _Limits[WRISTROT][MINPOS]) ? _Limits[WRISTROT][MINPOS] : wristrot;
        wristrot = (wristrot > _Limits[WRISTROT][MAXPOS]) ? _Limits[WRISTROT][MAXPOS] : wristrot;
    }
    return _Motors[WRISTROT]->move(wristrot, degree);
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
        gripper = (gripper < _Limits[GRIPPER][MINANGLE]) ? _Limits[GRIPPER][MINANGLE] : gripper;
        gripper = (gripper > _Limits[GRIPPER][MAXANGLE]) ? _Limits[GRIPPER][MAXANGLE] : gripper;
    }
    else 
    {
        gripper = (gripper < _Limits[GRIPPER][MINPOS]) ? _Limits[GRIPPER][MINPOS] : gripper;
        gripper = (gripper > _Limits[GRIPPER][MAXPOS]) ? _Limits[GRIPPER][MAXPOS] : gripper;
    }
  return _Motors[GRIPPER]->move(gripper, degree);
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
    for (int i = 0; i < _NbMotors; i++)
    {
        delete _Limits[i];
        delete _Motors[i];
    }
    delete _Limits;
}

