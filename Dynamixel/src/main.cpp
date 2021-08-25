#include "BraccioNeo.hpp"
#include "Variables.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>

#ifndef __APPLE__
	#include "lib/RF24/RF24.h"
	#include "lib/RF24/nRF24L01.h"
	#include "lib/RF24/RF24Network.h"
	
	RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_16MHZ);
	RF24Network network(radio);
	const uint16_t noeudMere = 00;
	const uint16_t monNoeud = noeudMere;
#endif

using namespace std;


// ---------------------------------------- //
// -              COMPARAISON             - //
// ---------------------------------------- //
/**
 * Compares two values using pointers
 */
int compare (const void * a, const void * b) 
{
   return ( *(int*)a - *(int*)b );
}


// ---------------------------------------- //
// -            DATA SHAPPING             - //
// ---------------------------------------- //
void dataShapping()
{
    cout << "Controle ..." << endl;
    //updates counter
    counter = (counter + 1) % AVERAGE_NB;

    //reads data
    _x1[counter] = localData.posBase;
    _y1[counter] = localData.posShoulder;
    _x2[counter] = localData.posElbow;
    _y2[counter] = localData.posWristRot;
    _x3[counter] = localData.posWristVer;
    _y3[counter] = localData.posGripper;

    //sorts arrays
    qsort(_x1, AVERAGE_NB, sizeof(short), compare);
    qsort(_y1, AVERAGE_NB, sizeof(short), compare);
    qsort(_x2, AVERAGE_NB, sizeof(short), compare);
    qsort(_y2, AVERAGE_NB, sizeof(short), compare);
    qsort(_x3, AVERAGE_NB, sizeof(short), compare);
    qsort(_y3, AVERAGE_NB, sizeof(short), compare);

    //gets median value
    averageX1 = _x1[median];
    averageX2 = _x2[median];
    averageX3 = _x3[median];
    averageY1 = _y1[median];
    averageY2 = _y2[median];
    averageY3 = _y3[median];
    
    //gets value to send to motor thanks to a map
    baseControle       = mapping(averageX1, vMax.XMIN, vMax.XMAX, 0, 180);
    shoulderControle   = mapping(averageY1, vMax.YMIN, vMax.YMAX, 20, 160);
    elbowControle      = mapping(averageX2, vMax.XMIN, vMax.XMAX, 0, 180);
    wristRotControle   = mapping(averageY2, vMax.YMIN, vMax.YMAX, 0, 180);
    wristVerControle   = mapping(averageX3, vMax.XMIN, vMax.XMAX, 0, 180);
    gripperControle    = mapping(averageY3, vMax.YMIN, vMax.YMAX, 25, 90);
    
    //makes sure values are not breaking the arm
    if (baseControle > 180) baseControle = 180;
    
    if (shoulderControle > 165) shoulderControle = 165;
    if (shoulderControle < 15)  shoulderControle = 15;
    
    if (elbowControle > 180) elbowControle = 180;
    
    if (wristRotControle > 180) wristRotControle = 180;
    
    if (wristVerControle > 180) wristVerControle = 180;
    
    if (gripperControle > 90) gripperControle = 90;
    if (gripperControle < 25) gripperControle = 25;
}


int main(int argc, char const *argv[])
{

    BraccioNeo.Infos();
    BraccioNeo.moveWristRot(300);
    unsigned base = 80;
    unsigned shoulder = 120;
    unsigned elbow = 90;
    unsigned wristver = 50;
    unsigned wristrot = 100;
    unsigned gripper = 90;
    BraccioNeo.changeSpeed(BASE, 20);
    BraccioNeo.changeSpeed(SHOULDER, 10);
    BraccioNeo.changeSpeed(ELBOW, 10);
    BraccioNeo.changeSpeed(WRISTVER, 10);
    BraccioNeo.changeSpeed(WRISTROT, 10);
    BraccioNeo.changeSpeed(GRIPPER, 10);
    //BraccioNeo.moveAll(base, shoulder, elbow, wristver, wristrot, gripper, true, true);
    BraccioNeo.moveWristVer(80, true);
    BraccioNeo.changeSpeed(WRISTVER, 25);
    BraccioNeo.moveWristVer(270, true);
    
    
    /*
    BraccioNeo.Infos();
    BraccioNeo.stand();
    BraccioNeo.moveAll(2000, 2000, 2000, 1000, 2000, false);
    BraccioNeo.moveAll(1200, 3000, 1200, 1200, 2000, false);
    BraccioNeo.moveWristVer(3000, false);
    BraccioNeo.moveElbow(1000, false);
    BraccioNeo.moveAll(3000, 1000, 3000, 3000, 2000, false);
    BraccioNeo.moveGripper(500);
    BraccioNeo.moveGripper(200);
    BraccioNeo.moveGripper(1500);
    BraccioNeo.moveGripper(1000);    
    BraccioNeo.Infos();

	#ifndef __APPLE__
	radio.begin();
	//	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_2MBPS);

	radio.startListening();

	network.begin(108, monNoeud);

	for (int i = 0; i < 1000; i++)
	{
		network.update();

		while(network.available())
		{
			RF24NetworkHeader nHeader;
			network.read(nHeader, &receivedData, sizeof(receivedData));

            switch (receivedData.mode)
            {
                case COLERE :
                    cout << "Colère" << endl;
                    break;
                
                case JOIE : 
                    cout << "JOIE" << endl;
                    break;

                case SURPRISE :
                    cout << "SURPRISE" << endl;
                    break;

                case CONTROLE : 
                    cout << "CONTROLE" << endl;
                    localData._action = receivedData._action;
                    switch (receivedData.ID)
                    {
                        case Bracelet1 :
                            localData.posBase       = receivedData.x;
                            localData.posShoulder     = receivedData.y;
                            break;
                
                        case Bracelet2 :
                            localData.posElbow      = receivedData.x;
                            localData.posWristRot = receivedData.y;
                            break;
                        
                        case Bracelet3 :
                            localData.posWristVer = receivedData.x;
                            localData.posGripper      = receivedData.y;
                            break;
                    }
                    dataShapping();
                    //Braccio.moveAll(baseControle, shoulderControle, elbowControle, wristRotControle, wristVerControle, gripperControle, speedMove);
                    break;

                case RIEN :
                default:
                    cout << "Droit" << endl;
                    break;
            }
		}

		cout << "ID = " << receivedData.ID << endl;
		cout << "X =  " << receivedData.x << endl;
		cout << "Y =  " << receivedData.y << endl;
	}	
	#endif
    */
	return 0;
}
