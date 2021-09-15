/**
 * This code allows user to send data from arduino to raspberry
 */
#include <stdlib.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>

/**
    Number of emitter to change :
      EMITTER 1 : 1 (base / shoudler)
      EMITTER 2 : 2 (elbow / poignet ver)
      EMITTER 3 : 3 (poignet rot / gripper)
*/
#define EMITTEUR 2

RF24 radio(9,10); //emission with Arduino Nano-rf

RF24Network network(radio);   

struct dataToSend
{
    short id = EMITTEUR - 1;
    short xAxis;
    short yAxis;
} sendData;

// Réseau
const uint16_t motherNode      = 00; 
const uint16_t sonNodes[3] = {01, 02, 03};

const uint16_t myNode       = sonNodes[EMITTEUR - 1];
const uint16_t targetedNode = motherNode;

//outputs of the accelerometer
const byte x_out = A0;
const byte y_out = A1;


// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    SPI.begin();
    
    //init radio
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    
    radio.stopListening();

    //init network
    network.begin(108, myNode);
}   


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    //updates network
    network.update();

    //reads data from the accelerometer
    sendData.xAxis = analogRead(x_out);
    sendData.yAxis = analogRead(y_out);
  
    //sends data
    RF24NetworkHeader nHeader(targetedNode);
    network.write(nHeader, &sendData, sizeof(sendData));  
}
