#ifndef __VARIABLES__
#define __VARIABLES__

// //This file contains global variables used by the Braccio

// ---------------------------------------- //
// -             ENUMERATIONS             - //
// ---------------------------------------- //
//Tells which bracelet is speaking and with which order
enum BRACELETS  {EMITTER1, EMITTER2, EMITTER3, REMOTE};
enum ACTIONS    {PLAY = 11, PAUSE = 12, STOP = 13};
enum MODES      {ANGRY = 20, JOY = 21, SURPRISE = 22, SHY = 23, CONTROL = 24, RECORD = 25, READ = 26, NONE = 27};
enum FILES      {FILE_1, FILE_2, FILE_3, FILE_4, FILE_5};
enum MOTORS 	{BASE, SHOULDER, ELBOW, WRISTVER, WRISTROT, GRIPPER};
enum EXTREM 	{MINPOS, MINANGLE, MIDDLEPOS, MIDDLEANGLE, MAXPOS, MAXANGLE};
enum SPEED 		{V_SLOW = 10, SLOW = 25, NORMAL = 50, FAST = 75, V_FAST = 90};


// ---------------------------------------- //
// -               STRUCTURES             - //
// // ---------------------------------------- //
// //Values received by the radio
static struct data
{
	short ID;
	short x;
	short y;
	short mode;
	short action;
	short file;
} receivedData;

//Extrem values received by radio
static struct V_MAX
{
    const short XMIN = 265;
    const short XMAX = 405;
    const short XMOY = (XMIN + XMAX)/2;
    
    const short YMIN = 265; 
    const short YMAX = 405;
    const short YMOY = (YMIN + YMAX)/2;
} vMax; 

//Values local, stores current values
static struct localData
{
	short baseControl     = vMax.XMOY;
	short shoulderControl = vMax.YMOY;
	short elbowControl    = vMax.XMOY; 
	short wristVerControl = vMax.XMOY;
	short wristRotControl = vMax.YMOY;
	short gripperControl  = vMax.YMOY;
	char mode;
	char action = NONE;
	char file;
} localData;


// ---------------------------------------- //
// -                AVERAGING             - //
// ---------------------------------------- //
//global variables for averaging data
const static unsigned AVERAGE_NB = 7; //must be odd
const static unsigned median = AVERAGE_NB / 2;
static unsigned counter = 0;
static bool _pause = false;
static bool _stop = false;

//arrays to store positions
static short _x1[AVERAGE_NB] = {0};
static short _y1[AVERAGE_NB] = {0};
static short _x2[AVERAGE_NB] = {0};
static short _y2[AVERAGE_NB] = {0};
static short _x3[AVERAGE_NB] = {0};
static short _y3[AVERAGE_NB] = {0};

static short averageX1 = 0;
static short averageY1 = 0;
static short averageX2 = 0;
static short averageY2 = 0;
static short averageX3 = 0;
static short averageY3 = 0;

//positions
static unsigned baseControl = 0;
static unsigned shoulderControl = 0;
static unsigned elbowControl = 0;
static unsigned wristVerControl = 0;
static unsigned wristRotControl = 0;
static unsigned gripperControl = 0;

#endif
