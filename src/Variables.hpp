#ifndef __VARIABLES__
#define __VARIABLES__

//This file contains global variables used by the Braccio

#include "BraccioNeo.hpp"

// ---------------------------------------- //
// -             ENUMERATIONS             - //
// ---------------------------------------- //
//Tells which bracelet is speaking and with which order
enum BRACELETS  {Bracelet1, Bracelet2, Bracelet3, Telecommande};
enum ACTIONS    {PLAY = 11, PAUSE = 12, STOP = 13};
enum MODES      {COLERE = 20, JOIE = 21, SURPRISE = 22, CONTROLE = 23, RIEN = 24};


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
	char _action;
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
	short posBase     = vMax.XMOY;
	short posShoulder = vMax.YMOY;
	short posElbow    = vMax.XMOY; 
	short posWristRot = vMax.YMOY;
	short posWristVer = vMax.XMOY;
	short posGripper  = vMax.YMOY;
	char mode;
	char _action = RIEN;
} localData;


// ---------------------------------------- //
// -                AVERAGING             - //
// ---------------------------------------- //
//global variables for averaging data
const unsigned AVERAGE_NB = 7; //must be odd
const unsigned median = (unsigned)AVERAGE_NB / 2;
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
unsigned char baseControle 		= 90;
unsigned char shoulderControle 	= 95;
unsigned char elbowControle 	= 95;
unsigned char wristRotControle 	= 90;
unsigned char wristVerControle 	= 90;
unsigned char gripperControle 	= 95;

#endif