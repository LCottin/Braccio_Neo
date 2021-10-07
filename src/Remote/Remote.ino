/**
 * This code allows the user to pilot the arm remotly
 */
 
#include <U8g2lib.h>
#include <RF24Network.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define REMOTE 4 

//buttons
//enum BUTTONS {PIN_HIGH = 6, PIN_LOW = 5, PIN_PLAY = 3, PIN_PAUSE = 2, PIN_STOP = 4};
enum BUTTONS {PIN_HIGH = 2, PIN_LOW = 3, PIN_PLAY = 5, PIN_PAUSE = 6, PIN_STOP = 4};
enum ACTIONS {PLAY = 11, PAUSE = 12, STOP = 13};
enum MODES   {ANGRY = 20, JOY = 21, SURPRISE = 22, SHY = 23, CONTROL = 24, RECORD = 25, READ = 26, NONE = 27};
enum FILES   {FILE_1, FILE_2, FILE_3, FILE_4, FILE_5};

//RF24 radio(7,8); //Arduino Nano + NRF24l01
RF24 radio(9,10); //Arduino Nano-rf

//Inits network
RF24Network network(radio); 
const byte motherNode  = 00; 
const byte myNode      = 04;

//Screen 
U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
const byte ORDER_NUMBERS = 7;
const char orderText[ORDER_NUMBERS][20] = {"Colere", "Joie", "Surprise", "Timide", "Controle", "Enregistrer", "Lecture"};
const byte order[ORDER_NUMBERS]         = {ANGRY, JOY, SURPRISE, SHY, CONTROL, RECORD, READ};
short orderNum = -1; 

//to read/write files
const byte FILES_NUMBERS = 5;
const char fileText[FILES_NUMBERS][20]  = {"Fichier 1", "Fichier 2", "Fichier 3", "Fichier 4", "Fichier 5"};
const byte files[FILES_NUMBERS]         = {FILE_1, FILE_2, FILE_3, FILE_4, FILE_5};
short fileNum = 0;

//data structure 
struct dataToSend
{
    short id      = REMOTE - 1;
    short xAxis   = 0; //useless for the remote
    short yAxis   = 0; //useless for the remote
    short mode     = NONE;
    short _action  = STOP;
    short file     = 0;
} sendData;

// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    //led pin
    //const byte PIN_LED = A7;

    //inits radio
    radio.begin();
    //radio.setPALevel(RF24_PA_MAX);
    //radio.setDataRate(RF24_2MBPS);
    
    radio.stopListening();

    //inits network
    network.begin(108, myNode);
  
    //inits buttons
    pinMode(PIN_HIGH, INPUT);
    pinMode(PIN_LOW, INPUT);
    pinMode(PIN_PLAY, INPUT);
    pinMode(PIN_PAUSE, INPUT);
    pinMode(PIN_STOP, INPUT);
    
    pinMode(A0, OUTPUT);
    digitalWrite(A0, HIGH);
    
    //inits screen
    u8g2.begin();
    u8g2.enableUTF8Print();
    
    u8g2.firstPage();
    do 
    {
        u8g2.setFont(u8g2_font_ncenB14_tr);
        u8g2.drawStr(20, 40, "Init ...");
    } while (u8g2.nextPage());

    delay(1000);
    u8g2.clear();
}

// ---------------------------------------- //
// -                 LOOP                 - //
// ---------------------------------------- //
void loop() 
{
    //updates network
    network.update();
  
    //scrolls up
    if (digitalRead(PIN_HIGH) == HIGH)
    {
        u8g2.clear();
        
        orderNum = (orderNum + 1) % (short)ORDER_NUMBERS;
        
        while (digitalRead(PIN_HIGH) == HIGH)
        {
            u8g2.firstPage();
            do 
            {
              u8g2.setFont(u8g2_font_ncenB14_tr);
              u8g2.drawStr(10, 50, orderText[orderNum]);
            } while (u8g2.nextPage());
        }
        u8g2.clear();
    }
    
    //scrolls down
    else if (digitalRead(PIN_LOW) == HIGH)
    {
        u8g2.clear();
       
        orderNum--;
        orderNum = orderNum < 0 ? (short)(ORDER_NUMBERS - 1): orderNum;
        
        while (digitalRead(PIN_LOW) == HIGH)
        { 
            u8g2.firstPage();
            do 
            {
              u8g2.setFont(u8g2_font_ncenB14_tr);
              u8g2.drawStr(10, 50, orderText[orderNum]);
            } while (u8g2.nextPage());
        }
        u8g2.clear();
    }
    
    //starts the action selected
    else if (digitalRead(PIN_PLAY) == HIGH)
    {
        u8g2.clear();
        while (digitalRead(PIN_PLAY) == HIGH)
        {
            u8g2.firstPage();
            do 
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(10, 20, "Play : ");
                u8g2.drawStr(10, 50, orderText[orderNum]);
            } while (u8g2.nextPage());
        }
        sendData.mode = order[orderNum];
        sendData._action = PLAY;
        u8g2.clear();

        //if we want to read a movement from a file
        if (sendData.mode == READ)
        {
            do 
            {
                //Scrolls up
                if (digitalRead(PIN_HIGH) == HIGH)
                {
                    fileNum = (fileNum + 1) % (short)FILES_NUMBERS;
                }

                //scrolls down
                else if (digitalRead(PIN_LOW) == HIGH)
                {
                    fileNum--;
                    fileNum = fileNum < 0 ? (short)(FILES_NUMBERS - 1): fileNum;
                }

                //prints file selected
                u8g2.firstPage();
                do
                {
                    delay(25);
                    u8g2.setFont(u8g2_font_ncenB14_tr);
                    u8g2.drawStr(10, 20, "A lire : ");
                    u8g2.drawStr(10, 40, fileText[fileNum]);
                    u8g2.drawStr(10, 60, "Oui = PLAY");
                } while (u8g2.nextPage());

            } while (digitalRead(PIN_PLAY) != HIGH);

            //sends on network
            sendData.file = files[fileNum];
            RF24NetworkHeader nHeader(motherNode);
            network.write(nHeader, &sendData, sizeof(sendData));
            delay(500);
            return;
        }

        //if we want to record a movement, to replay and to save if
        if (sendData.mode == RECORD)
        {
            RF24NetworkHeader nHeader(motherNode);
            network.write(nHeader, &sendData, sizeof(sendData));
            delay(10);
            //waits for the end of record
            unsigned long t = millis();
            while (digitalRead(PIN_STOP) != HIGH) {}
            t = millis() - t;

            sendData._action = STOP;
            network.write(nHeader, &sendData, sizeof(sendData));

            delay(1000);
            //asks for replay
            u8g2.clear();
            u8g2.firstPage();
            do 
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(10, 20, "Rejouer ?");
                u8g2.drawStr(10, 40, "Oui = PLAY");
                u8g2.drawStr(10, 60, "Non = STOP");
            } while ((u8g2.nextPage()) || ((digitalRead(PIN_PLAY) == LOW) && (digitalRead(PIN_STOP) == LOW)));

            u8g2.clear();
            if (digitalRead(PIN_PLAY) == HIGH)
            {
                sendData._action = PLAY;
                //sends on network
                RF24NetworkHeader nHeader(motherNode);
                network.write(nHeader, &sendData, sizeof(sendData));
                u8g2.firstPage();
                do 
                {
                    u8g2.setFont(u8g2_font_ncenB14_tr);
                    u8g2.drawStr(10, 20, "Relecture ...");
                } while ((u8g2.nextPage()));

                //waits for the replay to be done
                delay(t);
            }
            else
            {
                sendData._action = STOP;
                RF24NetworkHeader nHeader(motherNode);
                network.write(nHeader, &sendData, sizeof(sendData));
            }

            //asks for save
            delay(500);
            u8g2.clear();
            u8g2.firstPage();
            do 
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(10, 20, "Enregistrer?");
                u8g2.drawStr(10, 40, "Oui = PLAY");
                u8g2.drawStr(10, 60, "Non = STOP");
            } while ((u8g2.nextPage()) || ((digitalRead(PIN_PLAY) == LOW) && (digitalRead(PIN_STOP) == LOW)));

            u8g2.clear();
            if (digitalRead(PIN_PLAY) == HIGH)
            {
                sendData._action = PLAY;

                do 
                {
                    //Scrolls up
                    if (digitalRead(PIN_HIGH) == HIGH)
                    {
                        fileNum = (fileNum + 1) % (short)FILES_NUMBERS;
                    }

                    //scrolls down
                    else if (digitalRead(PIN_LOW) == HIGH)
                    {
                        fileNum--;
                        fileNum = fileNum < 0 ? (short)(FILES_NUMBERS - 1): fileNum;
                    }

                    //prints file selected
                    u8g2.firstPage();
                    do
                    {
                        delay(25);
                        u8g2.setFont(u8g2_font_ncenB14_tr);
                        u8g2.drawStr(10, 20, "A ecraser : ");
                        u8g2.drawStr(10, 40, fileText[fileNum]);
                        u8g2.drawStr(10, 60, "Oui = PLAY");
                    } while (u8g2.nextPage());

                } while (digitalRead(PIN_PLAY) != HIGH);

                //sends on network
                sendData.file = files[fileNum];
                //sends on network
                RF24NetworkHeader nHeader(motherNode);
                network.write(nHeader, &sendData, sizeof(sendData));
                delay(500);
                sendData._action = STOP;
                return; 
            }
            else
            {
                sendData._action = STOP;
                u8g2.firstPage();
                do 
                {
                    u8g2.setFont(u8g2_font_ncenB14_tr);
                    u8g2.drawStr(10, 20, "Non enregistré.");
                } while ((u8g2.nextPage()));
            }

            //sends on network
            //RF24NetworkHeader nHeader(motherNode);
            network.write(nHeader, &sendData, sizeof(sendData));
            delay(500);
            return;
        }

        //sends on network
        RF24NetworkHeader nHeader(motherNode);
        network.write(nHeader, &sendData, sizeof(sendData));
        delay(10);
        //sendData.mode = NONE;
        return;
    }
    
    //pauses the action
    else if (digitalRead(PIN_PAUSE) == HIGH)
    {
        u8g2.clear();

        while(digitalRead(PIN_PAUSE) == HIGH)
        {
            u8g2.firstPage();
            do 
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(10, 50, "Pause");
            } while (u8g2.nextPage());
        }
        sendData._action = PAUSE;
    }
    
    //Stops the action
    else if (digitalRead(PIN_STOP) == HIGH)
    {
        u8g2.clear();

        while (digitalRead(PIN_STOP) == HIGH)
        {
            u8g2.firstPage();
            do 
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(10, 50, "STOP");
            } while (u8g2.nextPage());
        }
        sendData.mode = NONE;
        sendData._action = STOP;
        
        u8g2.clear();
    }
    
    //when nothing is pressed
    else
    {
        if(sendData.mode != CONTROL)
          sendData.mode = NONE;
          
        switch (sendData._action)
        {
            case PLAY :
                u8g2.firstPage();
                do 
                {
                    u8g2.setFont(u8g2_font_ncenB14_tr);
                    u8g2.drawStr(10, 20, "En cours : ");
                    u8g2.drawStr(10, 50, orderText[orderNum]);
                } while (u8g2.nextPage());
                break;
  
              case STOP :
                  u8g2.firstPage();
                  do 
                  {
                      u8g2.setFont(u8g2_font_ncenB14_tr);
                      u8g2.drawStr(10, 50, "STOP !");
                  } while (u8g2.nextPage());
                  break;
  
              case PAUSE : 
                  u8g2.firstPage();
                  do 
                  {
                      u8g2.setFont(u8g2_font_ncenB14_tr);
                      u8g2.drawStr(10, 50, "PAUSE !");
                  } while (u8g2.nextPage());
                  break;
          }
    }

    //sends on network
    RF24NetworkHeader nHeader(motherNode);
    network.write(nHeader, &sendData, sizeof(sendData));
    delay(10);
}
