#include "BraccioNeo.hpp"

_BraccioNeo BraccioNeo;

_BraccioNeo::_BraccioNeo()
{
    //declares motors
    _Base       = new MX106AT(BASE);
    _Shoulder   = new MX106AT(SHOULDER);
    _Elbow      = new MX64AT(ELBOW);
    _WristVer   = new MX28AT(WRISTVER);
    _WristRot   = new AX18A(WRISTROT);
    _Gripper    = new AX18A(GRIPPER);
    
    //pushes them into the vector
    _Motors.push_back(_Base);
    _Motors.push_back(_Shoulder);
    _Motors.push_back(_Elbow);
    _Motors.push_back(_WristVer);
    _Motors.push_back(_WristRot);
    _Motors.push_back(_Gripper);
    
    initValues();
    stand();

    //led options
    _PinLed1 = 13;
    _PinLed2 = 18;
    /*
    bcm2835_init();
    bcm2835_gpio_fsel(_PinLed1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(_PinLed2, BCM2835_GPIO_FSEL_OUTP);
    */

} 

/**
 * Turns on / off ligths
 * @param pin 13 (left) or 18 (right)
 * @param on True for on, false for off
 */
void _BraccioNeo::light(const unsigned pin, const bool on)
{
  //bcm2835_gpio_write(pin, on ? 1 : 0);
}

/**
 * Updates global pause and stop variables
 * @param network Network pointer 
 */
void _BraccioNeo::pauseStop(RF24Network& network)
{
    network.update(); //MAJ du réseau 

    while(network.available())
    {
        RF24NetworkHeader nHeader;
        network.read(nHeader, &receivedData, sizeof(receivedData));
        
        if (receivedData.ID == REMOTE)
        {   
            switch (receivedData.action)
            {
                case PAUSE :
                    _pause = true;
                    _stop  = false;
                    break;
    
                case STOP : 
                    _pause = false;
                    _stop  = true;
                    receivedData.mode = NONE;
		    _Stand = false;
                    break;
    
                case PLAY : 
                default:
                    _pause = false;
                    _stop  = false;
                    break;
            }
        }  
    }
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
    _Limits[GRIPPER][MINPOS]       = 400;
    _Limits[GRIPPER][MINANGLE]     = 117;
    _Limits[GRIPPER][MIDDLEPOS]    = 512;
    _Limits[GRIPPER][MIDDLEANGLE]  = 150;
    _Limits[GRIPPER][MAXPOS]       = 805;
    _Limits[GRIPPER][MAXANGLE]     = 236;

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
    _Stand = true;
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
 * Records a sequence of movement and allows the user to save it
 * @param network Network pointer 
 * @param filename Name of the file too save the movement 
 * @returns true if correctly recorded, else false
 */
bool _BraccioNeo::record(RF24Network& network, const string filename)
{
    //declares six vectors for each motor
    vector<unsigned> base;
    vector<unsigned> shoudler;
    vector<unsigned> elbow;
    vector<unsigned> wristVer;
    vector<unsigned> wristRot;
    vector<unsigned> gripper;

    //tells when we should stop
    bool record = true;

    unsigned stepDelay = 200;
    const unsigned delay = 200;
    const unsigned timeout = 60 * MILLISECOND;

    //tells if an erro occured 
    bool success = true;

    //puts the arm straight and disables every torque
    stand();
    usleep(100 * MILLISECOND);

    for (unsigned i = 0; i < _NbMotors; i++)
    {
        success &= _Motors[i]->disableTorque();
    }
    
    //records movement while stop wasn't pressed
    while(record && (stepDelay < timeout))
    {
        //reads each position and converts its to degrees
        base.push_back      (mapping(_Motors[BASE]->getPosition(),     _Limits[BASE][MINPOS],     _Limits[BASE][MAXPOS],     0, 360));
        shoudler.push_back  (mapping(_Motors[SHOULDER]->getPosition(), _Limits[SHOULDER][MINPOS], _Limits[SHOULDER][MAXPOS], 0, 360));
        elbow.push_back     (mapping(_Motors[ELBOW]->getPosition(),    _Limits[ELBOW][MINPOS],    _Limits[ELBOW][MAXPOS],    0, 360));
        wristVer.push_back  (mapping(_Motors[WRISTVER]->getPosition(), _Limits[WRISTVER][MINPOS], _Limits[WRISTVER][MAXPOS], 0, 360));
        wristRot.push_back  (mapping(_Motors[WRISTROT]->getPosition(), _Limits[WRISTROT][MINPOS], _Limits[WRISTROT][MAXPOS], 0, 360));
        gripper.push_back   (mapping(_Motors[GRIPPER]->getPosition(),  _Limits[GRIPPER][MINPOS],  _Limits[GRIPPER][MAXPOS],  0, 360));
        
        //reads network to search for a stop
        network.update();
        if(network.available())
        {
            RF24NetworkHeader nHeader;
            network.read(nHeader, &receivedData, sizeof(receivedData));
            
            if (receivedData.ID == REMOTE)
                record = !((short)receivedData.action == STOP);
        }

        //delay to have smaller files, longer sleep results in smaller files put movement with less accuracy
        stepDelay += delay;
        usleep(delay * MILLISECOND);
    };

    for (unsigned i = 0; i < _NbMotors; i++)
    {
        success &= _Motors[i]->enableTorque();
    }
    stand();
    //usleep(100 * MILLISECOND);

    //replays the movement
    char replay = 0;
    do
    {
        //gets the info
        network.update();
        if(network.available())
        {
            RF24NetworkHeader nHeader;
            network.read(nHeader, &receivedData, sizeof(receivedData));
            
            if (receivedData.ID == REMOTE)
                replay = (char)receivedData.action;
        }
    } while((replay != PLAY) && (replay != STOP));

    //replays 
    if (replay == PLAY)
    {
        for (unsigned long i = 0; i < base.size(); i++)
        {
            success &= moveAll(base[i], shoudler[i], elbow[i], wristVer[i], wristRot[i], gripper[i], false);
            usleep(delay * MILLISECOND);
        }
    }

    //saves the movement to the file
    char save = 0;
    do
    {
        //gets the info
        network.update();
        if(network.available())
        {
            RF24NetworkHeader nHeader;
            network.read(nHeader, &receivedData, sizeof(receivedData));
            
            if (receivedData.ID == REMOTE)
                save = (char)receivedData.action;
        }
    } while((save != PLAY) && (save != STOP));

    //Replays
    if (save == PLAY)
    {
        string filename;
        filename.push_back(receivedData.file);
        transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

        string path = "../src/Records/";
        path += filename;
        path += ".txt";

        ofstream file(path);
        if (file)
        {
            for (unsigned long i = 0; i < base.size(); i++)
                file << base[i] << " " << shoudler[i] << " " << elbow[i] << " " << wristVer[i] << " " << wristRot[i] << " " << gripper[i] << endl;
        }
        else
        {
            cout << "Error when opening the file." << endl;
            success = false;
        }
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
 * Tells if the arm is standing
 * @returns true if the arm is standing, else false
 */
bool _BraccioNeo::isStanding() const
{
    return _Stand;
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
 * Reads a file and plays the movements described in it
 * @param filename Name of the file to read
 * @returns true if everything went right, else false
 */
bool _BraccioNeo::readFromFile(const FILES filename)
{
    //makes the complete path
    string path = "../src/Records/";
    switch (filename)
    {
        case FILE_1 :
            path += "FILE_1";
            break;
        
        case FILE_2 :
            path += "FILE_2";
            break;

        case FILE_3 :
            path += "FILE_3";
            break;

        case FILE_4 :
            path += "FILE_4";
            break;

        case FILE_5 :
            path += "FILE_5";
            break;
        
        default:
            break;
    }

    path += ".txt";

    //opens the file and reads it's content
    ifstream file(path);
    if (file)
    {
        unsigned base, shoudler, elbow, wristver, wristrot, gripper;

        //reads a line and moves the arm
        while (file >> base >> shoudler >> elbow >> wristver >> wristrot >> gripper)
        {
            moveAll(base, shoudler, elbow, wristver, wristrot, gripper, false);
            usleep(500 * MILLISECOND);
        }
        return true;
    }

    cout << "File not found." << endl;
    return false;
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
    _Stand = false;
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
    _Stand = false;
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
    _Stand = false;
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
    _Stand = false;
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
    _Stand = false;
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
    _Stand = false;
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
    _Stand = false;
    return _Motors[GRIPPER]->move(_CurrentPosition[GRIPPER], degree);
}

/**
 * Closes the gripper 
 * @returns true if correctly closed, else false
 */
bool _BraccioNeo::closeGripper()
{
    return moveGripper(_Limits[GRIPPER][MAXANGLE]);
}

/**
 * Opens the gripper 
 * @returns true if correctly opened, else false
 */
bool _BraccioNeo::openGripper()
{
    return moveGripper(_Limits[GRIPPER][MINANGLE]);
}

/**
 * Plays the angry emotion
 */
void _BraccioNeo::angry(RF24Network& network, SPEED speed)
{
    _Start = clock();
    _Speed = speed;
    _Stand = false;

    for (unsigned i = 0; i < _NbMotors; i++)
    {
        changeSpeed((MOTORS)i, _Speed);
    }
    
    moveShoulder(240, _Speed);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    WAIT
    usleep(20 * MILLISECOND);
    WAIT
    moveWristVer(205);
    WAIT

    for (int i = 0; i < 3; i++)
    {
        openGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
    }
    moveGripper(150);
    WAIT

    moveShoulder(120);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(120, 200, 170, 240, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(180, 160, 150, 180, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(210, 120, 180, 180, 150, 150);
    WAIT
    usleep(100 * MILLISECOND);
    WAIT

    moveShoulder(240);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveWristVer(240);
    WAIT
    usleep(50 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    WAIT

    usleep(10 * MILLISECOND);
    WAIT
    stand();
    WAIT
    usleep(5000 * MILLISECOND);
    WAIT

    moveShoulder(240);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    WAIT
    usleep(20 * MILLISECOND);
    WAIT
    moveWristVer(205);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT

    for (int i = 0; i < 3; i++)
    {
        openGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
    }
    moveGripper(150);
    WAIT

    moveShoulder(120);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(120, 200, 170, 240, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(180, 160, 150, 180, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(210, 120, 180, 180, 150, 150);
    WAIT
    usleep(100 * MILLISECOND);
    WAIT

    moveShoulder(240);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveWristVer(150);
    WAIT
    usleep(50 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    WAIT

    usleep(10 * MILLISECOND);
    WAIT
    stand();
    WAIT
    usleep(5000 * MILLISECOND);
    WAIT

    moveShoulder(240);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    WAIT
    usleep(20 * MILLISECOND);
    WAIT
    moveWristVer(205);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT

    for (int i = 0; i < 3; i++)
    {
        
        openGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
        
    }
    moveGripper(150);
    WAIT

    moveShoulder(250);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveWristVer(240);
    WAIT
    usleep(50 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    stand();
    WAIT
    usleep(5000 * MILLISECOND);
    WAIT

    moveShoulder(240);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    WAIT
    usleep(20 * MILLISECOND);
    WAIT
    moveWristVer(205);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT

    for (int i = 0; i < 3; i++)
    {
        openGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
    }
    moveGripper(150);
    WAIT

    moveShoulder(120);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(120, 200, 170, 240, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(180, 160, 150, 180, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(210, 120, 180, 180, 150, 150);
    WAIT
    usleep(100 * MILLISECOND);
    WAIT

    moveShoulder(250);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveWristVer(240);
    WAIT
    usleep(50 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    WAIT

    usleep(1000 * MILLISECOND);
    WAIT
    stand();
    WAIT
    usleep(1000 * MILLISECOND);
    WAIT

    moveShoulder(240);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 180, 110, 270, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
    WAIT
    usleep(20 * MILLISECOND);
    WAIT
    moveWristVer(205);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT

    for (int i = 0; i < 3; i++)
    {
        openGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(10 * MILLISECOND);
        WAIT
    }
    moveGripper(150);
    WAIT

    moveShoulder(120);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(120, 200, 170, 240, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(180, 160, 150, 180, 150, 150);
    WAIT
    usleep(30 * MILLISECOND);
    WAIT
    moveAll(210, 120, 180, 180, 150, 150);
    WAIT
    usleep(100 * MILLISECOND);
    WAIT

    moveShoulder(250);
    WAIT
    usleep(10 * MILLISECOND);
    WAIT
    moveWristVer(240);
    WAIT
    usleep(50 * MILLISECOND);
    WAIT
    moveAll(_CurrentPosition[BASE], 230, 185, 185, 150, 150);
    WAIT

    usleep(1000 * MILLISECOND);
    stand();

    _Stop = clock();
    long double time = (_Stop - _Start) / CLOCKS_PER_SEC;
    cout << "Angry emotion lasted " << time << "seconds." << endl;
}

/**
 * Plays the surprise emotion
 */
void _BraccioNeo::surprise(RF24Network& network, SPEED speed)
{
    _Start = clock();
    _Speed = speed;
    _Stand = false;

    for (unsigned i = 0; i < _NbMotors; i++)
    {
        changeSpeed((MOTORS)i, _Speed);
    }
  
    stand();
    WAIT

    moveWristRot(60);
    WAIT
    usleep(500 * MILLISECOND);
    WAIT
    moveWristRot(240);
    WAIT
    usleep(500 * MILLISECOND);
    WAIT
    moveWristRot(150);
    WAIT
    usleep(500 * MILLISECOND);
    WAIT

    moveWristVer(110);
    WAIT
    usleep(500 * MILLISECOND);
    WAIT
    moveWristRot(60);
    WAIT
    usleep(500 * MILLISECOND);
    WAIT
    moveWristVer(180);
    WAIT
    usleep(500 * MILLISECOND);
    WAIT
    
    moveElbow(110);
    WAIT
    moveWristVer(270);
    WAIT
    stand();
    WAIT
    moveBase(150);
    WAIT
    usleep(500 * MILLISECOND);
    WAIT

    stand();
    WAIT
    usleep(500 * MILLISECOND);
    WAIT
    openGripper();
    WAIT
    usleep(500 * MILLISECOND);
    WAIT
    closeGripper();
    WAIT
  
    usleep(1000 * MILLISECOND);
    stand();

    _Stop = clock();
    long double time = (_Stop - _Start) / CLOCKS_PER_SEC;
    cout << "Surprise emotion lasted " << time << "seconds." << endl;
}

/*
 * Plays the shy emotion
 */
void _BraccioNeo::shy(RF24Network& network, SPEED speed)
{
    _Start = clock();
    _Speed = speed;
    _Stand = false;

    for (unsigned i = 0; i < _NbMotors; i++)
    {
        changeSpeed((MOTORS)i, _Speed);
    }

    for (int i = 0; i < 3; i++)
    {
        
        openGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
        
    }
    moveGripper(150);
    WAIT

    for (int i = 0; i < 3; i++)
    {
        moveBase(180);
        WAIT
        usleep(20 * MILLISECOND);
        WAIT
        moveBase(230);
        WAIT
        usleep(20 * MILLISECOND);
        WAIT
    }
    moveBase(180);
    WAIT
    usleep(200 * MILLISECOND);
    WAIT

    for (int i = 0; i < 2; i++)
    {
        moveAll(_CurrentPosition[BASE], 230, 115, 240, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
        moveAll(_CurrentPosition[BASE], 140, 265, 130, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
    }

    usleep(1000 * MILLISECOND);
    WAIT
    stand();
    WAIT
    usleep(5000 * MILLISECOND);
    WAIT
    
    for (int i = 0; i < 3; i++)
    {
        
        openGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
        
    }
    moveGripper(150);
    WAIT
    
    for (int i = 0; i < 3; i++)
    {
        moveBase(180);
        WAIT
        usleep(20 * MILLISECOND);
        WAIT
        moveBase(230);
        WAIT
        usleep(20 * MILLISECOND);
        WAIT
    }
    moveBase(180);
    WAIT
    usleep(200 * MILLISECOND);
    WAIT

    for (int i = 0; i < 2; i++)
    {
        moveAll(_CurrentPosition[BASE], 230, 115, 240, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
        moveAll(_CurrentPosition[BASE], 140, 265, 130, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
    }

    usleep(1000 * MILLISECOND);
    WAIT
    stand();
    WAIT
    usleep(5000 * MILLISECOND);
    WAIT

    for (int i = 0; i < 3; i++)
    {
        
        openGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
        
    }
    moveGripper(150);
    WAIT
    
    for (int i = 0; i < 3; i++)
    {
        moveBase(180);
        WAIT
        usleep(20 * MILLISECOND);
        WAIT
        moveBase(230);
        WAIT
        usleep(20 * MILLISECOND);
        WAIT
    }
    moveBase(180);
    WAIT
    usleep(200 * MILLISECOND);
    WAIT

    for (int i = 0; i < 2; i++)
    {
        moveAll(_CurrentPosition[BASE], 230, 115, 240, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
        moveAll(_CurrentPosition[BASE], 140, 265, 130, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
    }

    usleep(1000 * MILLISECOND);
    WAIT
    stand();
    WAIT
    usleep(5000 * MILLISECOND);
    WAIT

    for (int i = 0; i < 3; i++)
    {
        openGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
    }
    moveGripper(150);
    WAIT
    
    for (int i = 0; i < 3; i++)
    {
        moveBase(180);
        WAIT
        usleep(20 * MILLISECOND);
        WAIT
        moveBase(230);
        WAIT
        usleep(20 * MILLISECOND);
        WAIT
    }
    moveBase(180);
    WAIT
    usleep(200 * MILLISECOND);
    WAIT

    for (int i = 0; i < 2; i++)
    {
        moveAll(_CurrentPosition[BASE], 230, 115, 240, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
        moveAll(_CurrentPosition[BASE], 140, 265, 130, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
    }
   
    usleep(1000 * MILLISECOND);
    stand();

    _Stop = clock();
    long double time = (_Stop - _Start) / CLOCKS_PER_SEC;
    cout << "Shy emotion lasted " << time << "seconds." << endl;
}

/*
 * Plays the joy emotion
 */
void _BraccioNeo::joy(RF24Network& network, SPEED speed)
{
    _Start = clock();
    _Speed = speed;
    _Stand = false;

    for (unsigned i = 0; i < _NbMotors; i++)
    {
        changeSpeed((MOTORS)i, _Speed);
    }

    moveShoulder(230);
    WAIT
    usleep(100 * MILLISECOND);
    WAIT
    moveElbow(130);
    WAIT
    usleep(100 * MILLISECOND);
    WAIT

    for (char i = 0; i < 4; i++)
    {
        moveWristRot(240);
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
        moveWristRot(100);
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
    }

    usleep(1000 * MILLISECOND);
    WAIT
    stand();
    WAIT

    moveWristRot(150);
    WAIT
    moveBase(260);
    WAIT
    moveWristVer(180);
    WAIT
    usleep(50 * MILLISECOND);
    WAIT
    moveBase(30);
    WAIT
    moveWristVer(100);
    WAIT
    usleep(50 * MILLISECOND);
    WAIT

    for (char i = 0; i < 2; i++)
    {
        moveBase(240);
        WAIT
        usleep(25 * MILLISECOND);
        WAIT
        moveShoulder(150);
        WAIT
        usleep(25 * MILLISECOND);
        WAIT
        moveElbow(240);
        WAIT
        usleep(250 * MILLISECOND);
        WAIT
        moveWristRot(120);
        WAIT
        usleep(25 * MILLISECOND);
        WAIT

        moveBase(130);
        WAIT
        usleep(25 * MILLISECOND);
        WAIT
        moveWristRot(150);
        WAIT
        usleep(250 * MILLISECOND);
        WAIT
        moveElbow(130);
        WAIT
        usleep(25 * MILLISECOND);
        WAIT
        moveShoulder(220);
        WAIT
        usleep(25 * MILLISECOND);
        WAIT
    }
                
    moveBase(180);
    WAIT
    moveShoulder(230);
    WAIT
    usleep(25 * MILLISECOND);
    WAIT
    moveElbow(130);
    WAIT
    usleep(25 * MILLISECOND);
    WAIT

    for (char i = 0; i < 3; i++)
    {
        openGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
        closeGripper();
        WAIT
        usleep(50 * MILLISECOND);
        WAIT
    }
    moveGripper(180);
    WAIT

    for (char i = 0; i < 3; i++)
    {
        moveBase(130);
        WAIT
        usleep(25 * MILLISECOND);
        WAIT
        moveBase(230);
        WAIT
        usleep(25 * MILLISECOND);
        WAIT
    }
    moveBase(180);
    WAIT
    usleep(200 * MILLISECOND);
    WAIT

    for (char i = 0; i < 2; i++)
    {
        moveAll(_CurrentPosition[BASE], 230, 115, 240, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
        moveAll(_CurrentPosition[BASE], 140, 265, 130, _CurrentPosition[WRISTROT], _CurrentPosition[GRIPPER]);
        WAIT
        usleep(200 * MILLISECOND);
        WAIT
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
bool _BraccioNeo::takePicture(RaspiCam& cam, const string filename)
{
    if (cam.open() == false)
    {
        cout << "Error : camera is not opened." << endl;
        return false;
    }

	unsigned char* data = new unsigned char [cam.getImageTypeSize(RASPICAM_FORMAT_RGB) ];

	//takes picture
	cam.grab();

	//extracts image
	cam.retrieve(data, RASPICAM_FORMAT_RGB);

    //saves it
    string path = "../src/Records/";
    path += filename;
    path += ".jpg";
	ofstream file(path, std::ios::binary);

	file << "P6\n" << cam.getWidth() << " " << cam.getHeight() << " 255\n";
	file.write( (char*)data, cam.getImageTypeSize(RASPICAM_FORMAT_RGB) );
	
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

