#include "BraccioNeo.hpp"

_BraccioNeo BraccioNeo;

_BraccioNeo::_BraccioNeo()
{
    //declares motors
    _Base       = new MX106AT(BASE);
    _Shoulder   = new MX106AT(SHOULDER);
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

    _NbMotors = (short)_Motors.size();

    initValues();
} 

/**
 * Initializes extrem values of each motor
 */
void _BraccioNeo::initValues()
{
    //creates an array to store extrems positions of each motor
    _Limits = new short* [_NbMotors];
    for (int i = 0; i < _NbMotors; i++)
    {
        _Limits[i] = new short[6];
    }

    //limits for the base
    _Limits[BASE][MINPOS]       = 0;
    _Limits[BASE][MINANGLE]     = 0;
    _Limits[BASE][MIDDLEPOS]    = 2047;
    _Limits[BASE][MIDDLEANGLE]  = 180;
    _Limits[BASE][MAXPOS]       = 4095;
    _Limits[BASE][MAXPOS]       = 360;

    //limits for the shoulder
    _Limits[SHOULDER][MINPOS]       = 1024;
    _Limits[SHOULDER][MINANGLE]     = 90;
    _Limits[SHOULDER][MIDDLEPOS]    = 2047;
    _Limits[SHOULDER][MIDDLEANGLE]  = 180;
    _Limits[SHOULDER][MAXPOS]       = 3072;
    _Limits[SHOULDER][MAXPOS]       = 270;

    //limits for the elbow
    _Limits[ELBOW][MINPOS]       = 683;
    _Limits[ELBOW][MINANGLE]     = 60;
    _Limits[ELBOW][MIDDLEPOS]    = 2047;
    _Limits[ELBOW][MIDDLEANGLE]  = 180;
    _Limits[ELBOW][MAXPOS]       = 3412;
    _Limits[ELBOW][MAXPOS]       = 300;

    //limits for the wrist rot
    _Limits[WRISTROT][MINPOS]       = 683;
    _Limits[WRISTROT][MINANGLE]     = 60;
    _Limits[WRISTROT][MIDDLEPOS]    = 2047;
    _Limits[WRISTROT][MIDDLEANGLE]  = 180;
    _Limits[WRISTROT][MAXPOS]       = 3412;
    _Limits[WRISTROT][MAXPOS]       = 300;

    //limits for the wrist ver
    _Limits[WRISTVER][MINPOS]       = 0;
    _Limits[WRISTVER][MINANGLE]     = 0;
    _Limits[WRISTVER][MIDDLEPOS]    = 512;
    _Limits[WRISTVER][MIDDLEANGLE]  = 180;
    _Limits[WRISTVER][MAXPOS]       = 1023;
    _Limits[WRISTVER][MAXPOS]       = 300;
    
    //limits for the gripper
    _Limits[GRIPPER][MINPOS]       = 0;
    _Limits[GRIPPER][MINANGLE]     = 0;
    _Limits[GRIPPER][MIDDLEPOS]    = 512;
    _Limits[GRIPPER][MIDDLEANGLE]  = 180;
    _Limits[GRIPPER][MAXPOS]       = 1023;
    _Limits[GRIPPER][MAXPOS]       = 300;
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
 * Moves all the motor
 * @param shoulder New position for the shoulder
 * @param elbow New position for the elbow
 * @param wristver New position for the wrist ver
 * @param wristrot New position for the wrist rot
 * @returns true if every motor moved well, else false
 */
bool _BraccioNeo::moveAll(const unsigned shoulder, const unsigned elbow, const unsigned wristver, const unsigned wristrot, const unsigned gripper, const bool degree)
{
    //puts position in an array
    unsigned* position  = new unsigned[_Motors.size()];
    position[SHOULDER]  = shoulder;
    position[ELBOW]     = elbow;
    position[WRISTVER]  = wristver;
    position[WRISTROT]  = wristrot;
    position[GRIPPER]    = gripper;

    //maps positions if they are given in degree
    if (degree)
    {
        position[SHOULDER]  = mapping(position[shoulder], 0, 360, 0, 4095);
        position[ELBOW]     = mapping(position[elbow], 0, 360, 0, 4095);
        position[WRISTVER]  = mapping(position[wristver], 0, 360, 0, 4095);
        position[WRISTROT]  = mapping(position[wristrot], 0, 360, 0, 1023);
	    position[GRIPPER]   = mapping(position[gripper], 0, 360, 0, 4095);
    }

    //indicates if there is an error
    bool success = true;

    for (int i = 0; i < (int)_Motors.size(); i++)   
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
bool _BraccioNeo::moveShoulder(const unsigned shoulder, const bool degree)
{
    return _Motors[SHOULDER]->move(shoulder, degree);
}

/**
 * Moves the elbow
 * @param elbow New position of the elbow
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveElbow(const unsigned elbow, const bool degree)
{
    return _Motors[ELBOW]->move(elbow, degree);
}

/**
 * Makes the hand rises
 * @param wristver New position of the wrist vertically
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveWristVer(const unsigned wristver, const bool degree)
{
    return _Motors[WRISTVER]->move(wristver, degree);
}

/**
 * Makes the hand turns
 * @param wristrot New position of the wrist 
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveWristRot(const unsigned wristrot, const bool degree)
{
    return _Motors[WRISTROT]->move(wristrot, degree);
}

/*
 * Makes the gripper catch
 * @param gripper New position of the wrist 
 * @returns true if correctly moved, else false
 */
bool _BraccioNeo::moveGripper(const unsigned gripper, const bool degree)
{
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

