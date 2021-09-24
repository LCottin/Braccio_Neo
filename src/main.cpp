//#include <wiringPi.h>
#include "BraccioNeo.hpp"
// #include "Variables.hpp"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
//#include <wiringPi.h>

#ifndef __APPLE__
/*
    #include "lib/RF24/RF24.h"
    #include "lib/RF24/RF24Network.h"
    #include "lib/RF24/nRF24L01.h"
*/

    #include <RF24/RF24.h>
    #include <RF24Network/RF24Network.h>
    #include <RF24/nRF24L01.h>
    
    
    //RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
    //RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24);
    RF24 radio(25,0);
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

void affichebin(unsigned n)
{
	unsigned bit = 0 ;
	unsigned mask = 1 ;
	for (int i = 0 ; i < 32 ; ++i)
	{
		bit = (n & mask) >> i ;
		printf("%d", bit) ;
		mask <<= 1 ;
	}
}

unsigned int dtobin(unsigned char h)
{
	double n;
   unsigned int b=0;
 
   for(n=0;n<=7;n++)
   {
       b+=(pow(10,n)*(h%2));
       h/=2;
   }
 
   return b;
}



int main(int argc, char const *argv[])
{
    cout << "bras initialisé" << endl; 

    int pwmPin1 = 1;
    int pwmPin2 = 23;

    //system("gpio mode 1 output; gpio mode 23 output; gpio write 1 1; gpio write 23 1 ");
    /*
    BraccioNeo.light(13, true);
    BraccioNeo.light(18, true);
    usleep(500 * MILLISECOND);
    BraccioNeo.light(13, false);
    BraccioNeo.light(18, false);
    usleep(500 * MILLISECOND);
    BraccioNeo.light(13, true);
    BraccioNeo.light(18, true);
    */
#ifndef __APPLE__
    //inits radio
    radio.begin();
    /*
    wiringPiSetup();
    pinMode(pwmPin1, PWM_OUTPUT);
    pwmWrite(pwmPin1, 1023);
    pinMode(pwmPin2, PWM_OUTPUT);
    pwmWrite(pwmPin2, 1023);
    */

    //system("gpio mode 1 pwm ; gpio mode 23 pwm ; gpio pwm 1 256 ; gpio pwm 23 1024 ");
    //system("gpio mode 1 pwm ; gpio pwm 1 256 ");
    
    
    //radio.setAutoAck(1);
    //radio.setRetries(1,3);
    //radio.setRetries(15,15);
    //radio.enableDynamicPayloads();
    //radio.setPayloadSize(sizeof(receivedData));
    //radio.setCRCLength(RF24_CRC_16);
    //radio.setPALevel(RF24_PA_MAX);
    //radio.setDataRate(RF24_2MBPS);
    radio.startListening();
    //radio.powerDown();
    radio.printDetails();



    network.begin(108, myNode);
 
    cout << "debut radio " << endl; 
 
    //inits data
    initArrays();

    /*
    for (unsigned i = 0; i < 6; i++)
    {
        BraccioNeo.changeSpeed((MOTORS)i, 15);
    }
    */
    BraccioNeo.changeSpeed(BASE, 25);
    BraccioNeo.changeSpeed(SHOULDER, 25);
    BraccioNeo.changeSpeed(ELBOW, 40);
    BraccioNeo.changeSpeed(WRISTVER, 25);
    BraccioNeo.changeSpeed(WRISTROT, 25);
    BraccioNeo.changeSpeed(GRIPPER, 10);
    
    
    while(true)
    {
        
        _pause = false;
        _stop  = false;
	if(receivedData.mode  != CONTROL)
	  receivedData.mode = NONE;
	
        network.update();

        while(network.available())
        {
	
            RF24NetworkHeader nHeader;
	    //network.peek(nHeader);
            
            network.read(nHeader, &receivedData, sizeof(receivedData));
	    //network.read(nHeader, &receivedData, sizeof(struct data));
	    //usleep(500 * MILLISECOND);
	    //network.read(nHeader, &receivedData, radio.getDynamicPayloadSize());
	    
	    //cout << "sizeof recievedData = " << (int)sizeof(receivedData) << endl;
	    //cout << "sizeof PayloadSize = " << (int)radio.getDynamicPayloadSize() << endl;
	    //cout << "ID = " << (short)receivedData.ID << endl; 
	    //cout << "x = " << (short)receivedData.x << endl;
	    //cout << "y = " << (short)receivedData.y << endl;
	    cout << "mode = " << (short)receivedData.mode << endl;
	    //cout << "action = " << (short)receivedData.action << endl;
	    //cout << "file = " << (short)receivedData.file << endl;
	    //printf("file = %d\n", receivedData.file);
	    cout << endl;
	    //usleep(100 * MILLISECOND);
   	}
	/*
	BraccioNeo.changeSpeed(BASE, 25);
	BraccioNeo.changeSpeed(SHOULDER, 25);
	BraccioNeo.changeSpeed(ELBOW, 40);
	BraccioNeo.changeSpeed(WRISTVER, 25);
	BraccioNeo.changeSpeed(WRISTROT, 25);
	BraccioNeo.changeSpeed(GRIPPER, 25);
	*/
	switch (receivedData.mode)
            {
	        
                case READ :
                    cout << "READ" << endl;
                    BraccioNeo.readFromFile((FILES)receivedData.file);
                    break;
	        case RECORD :
                    cout << "RECORD" << endl;
                    BraccioNeo.record(network);
                    break;

                case ANGRY :
                    cout << "ANGRY" << endl;
                    BraccioNeo.angry(network);
                    break;
                
                case JOY : 
                    cout << "JOY" << endl;
                    BraccioNeo.joy(network);
                    break;

                case SURPRISE :
                    cout << "SURPRISE" << endl;
                    BraccioNeo.surprise(network);
                    break;

                case SHY :
                    cout << "SHY" << endl;
                    BraccioNeo.shy(network);
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

	    }
    		
    }   
#endif

	return 0;
}
