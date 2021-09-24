/**
 * This code allows user to send data from arduino to raspberry
 */
#include <stdlib.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#include <printf.h>

/**
    Number of emitter to change :
      EMITTER 1 : 1 (base / shoudler)
      EMITTER 2 : 2 (elbow / poignet ver)
      EMITTER 3 : 3 (poignet rot / gripper)
*/
#define EMITTER 2

RF24 radio(9,10); //emission with Arduino Nano-rf

RF24Network network(radio);   

struct dataToSend
{
    short id = EMITTER - 1;
    short xAxis;
    short yAxis;
    /*
    short mode = 30000;
    short _action = 31000;
    short file = 32000;
    */
} sendData;

// Réseau
const uint16_t motherNode      = 00; 
const uint16_t sonNodes[4] = {01, 02, 03, 04};

const uint16_t myNode       = sonNodes[EMITTER - 1];
const uint16_t targetedNode = motherNode;

//outputs of the accelerometer
const byte x_out = A0;
const byte y_out = A1;

short i = 0;

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    Serial.begin(9600);
    SPI.begin();
    printf_begin();
    
    //init radio
    radio.begin();
    //radio.setPALevel(RF24_PA_MAX);
    //radio.setDataRate(RF24_2MBPS);
    //radio.setAutoAck(1);
    //radio.setRetries(1,3);
    //radio.setRetries(15,15);
    //radio.enableDynamicPayloads();
    //radio.setPayloadSize(sizeof(sendData));
    //radio.powerUp();
    radio.printDetails();
    
    radio.stopListening();

    //radio.printDetails();
    
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
    //i++;

    
    //reads data from the accelerometer
    sendData.xAxis = analogRead(x_out);
    sendData.yAxis = analogRead(y_out);
    
    //sendData.id = i;
    //sendData.xAxis = 4;
    //sendData.yAxis = 4;
    /*
    sendData.mode = 4;
    sendData._action = 4;
    sendData.file = 4;  
    */
    
    Serial.print("ID = ");
    Serial.println(sendData.id);
    Serial.print("x = ");
    Serial.println(sendData.xAxis);
    Serial.print("y = ");
    Serial.println(sendData.yAxis);
    Serial.println(" ");
    /*
    Serial.print("PayLoadSize = ");
    Serial.println(radio.getDynamicPayloadSize());
    //Serial.println(sendData.id);
    //Serial.println(sizeof(sendData));
    delay(1000);
    */
  
    //sends data
    RF24NetworkHeader nHeader(targetedNode);
    network.write(nHeader, &sendData, sizeof(sendData));  
    //delay(100);
}
