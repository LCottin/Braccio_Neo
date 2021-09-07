#ifndef __VARIABLES__
#define __VARIABLES__

// //This file contains global variables used by the Braccio

// // ---------------------------------------- //
// // -             ENUMERATIONS             - //
// // ---------------------------------------- //
// //Tells which bracelet is speaking and with which order
// enum BRACELETS  {EMITTER1, EMITTER2, EMITTER3, Telecommande};
// enum ACTIONS    {PLAY = 11, PAUSE = 12, STOP = 13};
// enum MODES      {ANGRY = 20, JOY = 21, SURPRISE = 22, SHY = 23, CONTROL = 24, NONE = 25};


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
};

// //Extrem values received by radio
// struct V_MAX
// {
//     const short XMIN = 260;
//     const short XMAX = 420;
//     const short XMOY = (XMIN + XMAX)/2;
    
//     const short YMIN = 260; 
//     const short YMAX = 420;
//     const short YMOY = (YMIN + YMAX)/2;
// } vMax; 

// //Values local, stores current values
// struct localData
// {
// 	short baseControl     = vMax.XMOY;
// 	short shoulderControl = vMax.YMOY;
// 	short elbowControl    = vMax.XMOY; 
// 	short wristVerControl = vMax.XMOY;
// 	short wristRotControl = vMax.YMOY;
// 	short gripperControl  = vMax.YMOY;
// 	char mode;
// 	char action = NONE;
// } localData;

#endif
