#include "lib/RF24.h"
#include "lib/nRF24L01.h"
#include "lib/RF24Network.h"

#include <iostream>
using namespace std;

struct x
{
	short ID;
	short x;
	short y;
}receive;

const uint16_t noeudMere = 00;
const uint16_t noeudsFille[3] = {01, 02, 03};

const uint16_t monNoeud = noeudMere;
const uint16_t noeudCible = noeudMere;

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_16MHZ);
RF24Network network(radio);

// test

int main()
{
	radio.begin();
//	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_2MBPS);

	radio.startListening();
	
	network.begin(108, monNoeud);

	while(1)
	{
		network.update();

		while(network.available())
		{
			RF24NetworkHeader nHeader;
			network.read(nHeader, &receive, sizeof(receive));
		}

		cout << "ID = " << receive.ID << endl;
		cout << "X =  " << receive.x << endl;
		cout << "Y =  " << receive.y << endl;
	}	

	return 0;
}
