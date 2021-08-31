/**
 * Code permettant d'envoyer les données 
 * de l'acceléromètre au robot
 */
#include <stdlib.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>

/**
    Numéro de l'émetteur à modifier :
      Emetteur 1 : 1 (base / épaule)
      Emetteur 2 : 2 (coude / poignet rot)
      Emetteur 3 : 3 (poignet ver / pince)
*/
#define EMETTEUR 3

RF24 radio(9,10); //emission avec Arduino Nano-rf

RF24Network network(radio);   // Nota : "Network" utilise la librairie "radio"

//structure de données pour l'envoie des inclinaisons fournies par l'accéléromètre
struct dataToSend
{
    short id = EMETTEUR - 1;
    short xAxis;
    short yAxis;
} send_data;

// Réseau
const uint16_t noeudMere      = 00; // Valeur "0" écrit au format "octal" (d'où l'autre "0" devant)
const uint16_t noeudsFille[3] = {01, 02, 03};

const uint16_t monNoeud   = noeudsFille[EMETTEUR - 1];
const uint16_t noeudCible = noeudMere;

//variables du moyennage
const byte NB_MOYENNAGE = 5; //doit etre impair !!
short X[NB_MOYENNAGE];
short Y[NB_MOYENNAGE];
byte cmp = 0;
short moyX;
short moyY;

//ports de sortie de l'accéléromètre
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
    network.begin(108, monNoeud);

    //init buffer moyennage
    initMoyennage();
}   


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    //definition des constantes
    //cmp = (cmp + 1) % NB_MOYENNAGE;
    
    //MAJ du réseau
    network.update();

    //à décommenter si on veut un moyennage 
    /*
    //lecture des données de l'accelromètre
    X[cmp] = analogRead(x_out);
    Y[cmp] = analogRead(y_out);

    //moyennage et affectation = filtre médian glissant
    qsort(X, NB_MOYENNAGE, sizeof(short), compare);
    qsort(Y, NB_MOYENNAGE, sizeof(short), compare);

    send_data.xAxis = X[NB_MOYENNAGE / 2];
    send_data.yAxis = Y[NB_MOYENNAGE / 2];
    */

    //lecture des données de l'accelromètre
    send_data.xAxis = analogRead(x_out);
    send_data.yAxis = analogRead(y_out);
  
    //envoie des données
    RF24NetworkHeader nHeader(noeudCible);
    network.write(nHeader, &send_data, sizeof(send_data));  
}


// ---------------------------------------- //
// -     INITIALISATION DES BUFFERS       - //
// ---------------------------------------- //
void initMoyennage()
{
    for(byte i = 0; i < NB_MOYENNAGE; i++)
    {
        X[i] = 300;
        Y[i] = 300;
    }
}


// ---------------------------------------- //
// -              COMPARAISON             - //
// ---------------------------------------- //
int compare (const void * a, const void * b) 
{
   return ( *(int*)a - *(int*)b );
}
