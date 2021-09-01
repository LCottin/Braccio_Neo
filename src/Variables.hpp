#ifndef __VARIABLES__
#define __VARIABLES__

//This file contains global variables used by the Braccio

#include "BraccioNeo.hpp"

// ---------------------------------------- //
// -             ENUMERATIONS             - //
// ---------------------------------------- //
//Tells which bracelet is speaking and with which order
enum BRACELETS  {EMITTER1, EMITTER2, EMITTER3, Telecommande};
enum ACTIONS    {PLAY = 11, PAUSE = 12, STOP = 13};
enum MODES      {ANGRY = 20, JOY = 21, SURPRISE = 22, CONTROL = 23, NONE = 24};


// ---------------------------------------- //
// -               STRUCTURES             - //
// ---------------------------------------- //
//Values received by the radio
struct data
{
	short ID;
	short x;
	short y;
	char mode;
	char action;
} receivedData;

//Extrem values received by radio
struct V_MAX
{
    const short XMIN = 260;
    const short XMAX = 420;
    const short XMOY = (XMIN + XMAX)/2;
    
    const short YMIN = 260; 
    const short YMAX = 420;
    const short YMOY = (YMIN + YMAX)/2;
} vMax; 

//Values local, stores current values
struct localData
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
const unsigned AVERAGE_NB = 7; //must be odd
const unsigned median = AVERAGE_NB / 2;
unsigned counter = 0;

//arrays to store positions
short _x1[AVERAGE_NB];
short _y1[AVERAGE_NB];
short _x2[AVERAGE_NB];
short _y2[AVERAGE_NB];
short _x3[AVERAGE_NB];
short _y3[AVERAGE_NB];

short averageX1;
short averageY1;
short averageX2;
short averageY2;
short averageX3;
short averageY3;

//positions
unsigned baseControl 		= 180;
unsigned shoulderControl 	= 180;
unsigned elbowControl 		= 180;
unsigned wristVerControl 	= 180;
unsigned wristRotControl 	= 180;
unsigned gripperControl 	= 180;

#endif
