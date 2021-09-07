#ifndef __VARIABLES__
#define __VARIABLES__

// //This file contains global variables used by the Braccio

// ---------------------------------------- //
// -             ENUMERATIONS             - //
// ---------------------------------------- //
//Tells which bracelet is speaking and with which order
enum BRACELETS  {EMITTER1, EMITTER2, EMITTER3, REMOTE};
enum ACTIONS    {PLAY = 11, PAUSE = 12, STOP = 13};
enum MODES      {ANGRY = 20, JOY = 21, SURPRISE = 22, SHY = 23, CONTROL = 24, RECORD = 25, NONE = 26};
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
	char mode;
	char action;
} receivedData;

//Extrem values received by radio
static struct V_MAX
{
    const short XMIN = 260;
    const short XMAX = 420;
    const short XMOY = (XMIN + XMAX)/2;
    
    const short YMIN = 260; 
    const short YMAX = 420;
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
} localData;


// ---------------------------------------- //
// -                AVERAGING             - //
// ---------------------------------------- //
//global variables for averaging data
const static unsigned AVERAGE_NB = 7; //must be odd
const static unsigned median = AVERAGE_NB / 2;
static unsigned counter;
static bool _pause;
static bool _stop;

//arrays to store positions
static short _x1[AVERAGE_NB];
static short _y1[AVERAGE_NB];
static short _x2[AVERAGE_NB];
static short _y2[AVERAGE_NB];
static short _x3[AVERAGE_NB];
static short _y3[AVERAGE_NB];

static short averageX1;
static short averageY1;
static short averageX2;
static short averageY2;
static short averageX3;
static short averageY3;

//positions
static unsigned baseControl;
static unsigned shoulderControl;
static unsigned elbowControl;
static unsigned wristVerControl;
static unsigned wristRotControl;
static unsigned gripperControl;

#endif
