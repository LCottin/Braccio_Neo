#include "BraccioNeo.hpp"
// #include "Variables.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>

#ifndef __APPLE__
    #include "lib/RF24/RF24.h"
    #include "lib/RF24/RF24Network.h"
    #include "lib/RF24/nRF24L01.h"
    
    RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_16MHZ);
    //RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24);
    RF24Network network(radio);
    const uint16_t motherNode = 00;
    const uint16_t myNode = motherNode;
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
    cout << "Control ..." << endl;
    //updates counter
    counter = (counter + 1) % AVERAGE_NB;

    //reads data
    _x1[counter] = localData.baseControl;
    _y1[counter] = localData.shoulderControl;
    _x2[counter] = localData.elbowControl;
    _y2[counter] = localData.wristVerControl;
    _x3[counter] = localData.wristRotControl;
    _y3[counter] = localData.gripperControl;

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
    baseControl       = mapping(averageX1, vMax.XMIN, vMax.XMAX, BraccioNeo.getExtremValue(BASE, MINANGLE), BraccioNeo.getExtremValue(BASE, MAXANGLE));
    shoulderControl   = mapping(averageY1, vMax.YMIN, vMax.YMAX, BraccioNeo.getExtremValue(SHOULDER, MINANGLE), BraccioNeo.getExtremValue(SHOULDER, MAXANGLE));
    elbowControl      = mapping(averageX2, vMax.XMIN, vMax.XMAX, BraccioNeo.getExtremValue(ELBOW, MINANGLE), BraccioNeo.getExtremValue(ELBOW, MAXANGLE));
    wristVerControl   = mapping(averageY2, vMax.XMIN, vMax.XMAX, BraccioNeo.getExtremValue(WRISTVER, MINANGLE), BraccioNeo.getExtremValue(WRISTVER, MAXANGLE));
    wristRotControl   = mapping(averageX3, vMax.YMIN, vMax.YMAX, BraccioNeo.getExtremValue(WRISTROT, MINANGLE), BraccioNeo.getExtremValue(WRISTROT, MAXANGLE));
    gripperControl    = mapping(averageY3, vMax.YMIN, vMax.YMAX, BraccioNeo.getExtremValue(GRIPPER, MINANGLE), BraccioNeo.getExtremValue(GRIPPER, MAXANGLE));
}

// ---------------------------------------- //
// -             INIT ARRAYS              - //
// ---------------------------------------- //
void initArrays()
{
    //inits data in each array that receive data from radio
    for (unsigned i = 0; i < AVERAGE_NB; i++)
    {
        _x1[i] = vMax.XMOY;
        _y1[i] = vMax.YMOY;
        _x2[i] = vMax.XMOY;
        _y2[i] = vMax.YMOY;
        _x3[i] = vMax.XMOY;
        _y3[i] = vMax.YMOY;
    }

    //inits other variables
    counter              = 0;
    
    baseControl         = 180;
    shoulderControl     = 180;
    elbowControl        = 180;
    wristVerControl     = 180;
    wristRotControl     = 180;
    gripperControl      = 180;

    _pause   = false;
    _stop    = false;
}

int main(int argc, char const *argv[])
{
    cout << "bras initialisé" << endl; 
    

#ifndef __APPLE__
    //inits radio
    radio.begin();
    //radio.setPALevel(RF24_PA_MAX);
    //radio.setDataRate(RF24_1MBPS);
    radio.startListening();
    network.begin(90, myNode);

    //inits data
    initArrays();

    while(true)
    {
        network.update();

        while(network.available())
        {
            RF24NetworkHeader nHeader;
            network.read(nHeader, &receivedData, sizeof(receivedData));
	    cout << "ici" << endl;
	    
            /*
            switch (receivedData.mode)
            {
                case READ :
                    cout << "READ" << endl;
                    BraccioNeo.readFromFile(receivedData.file);
                    break;

                case ANGRY :
                    cout << "ANGRY" << endl;
                    BraccioNeo.angry();
                    break;
                
                case JOY : 
                    cout << "JOY" << endl;
                    BraccioNeo.joy();
                    break;

                case SURPRISE :
                    cout << "SURPRISE" << endl;
                    BraccioNeo.surprise();
                    break;

                case SHY :
                    cout << "SHY" << endl;
                    BraccioNeo.shy();
                    break;

                case CONTROL : 
                    cout << "CONTROL" << endl;
                    localData.action = receivedData.action;
                    switch (receivedData.ID)
                    {
                        case EMITTER1 :
                            localData.baseControl       = receivedData.x;
                            localData.shoulderControl   = receivedData.y;
                            break;
                
                        case EMITTER2 :
                            localData.elbowControl      = receivedData.x;
                            localData.wristVerControl   = receivedData.y;
                            break;
                        
                        case EMITTER3 :
                            localData.wristRotControl   = receivedData.x;
                            localData.gripperControl    = receivedData.y;
                            break;
                    }
                    dataShapping();
                    BraccioNeo.moveAll(baseControl, shoulderControl, elbowControl, wristVerControl, wristRotControl, gripperControl, false);
                    break;

                case NONE :
                default:
                    if (!BraccioNeo.isStanding())
                        BraccioNeo.stand();
                    break;

		    }*/
        }
    }   
#endif

	return 0;
}
