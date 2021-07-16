#include "MX64AT.hpp"
#include "XL320.hpp"
#include "MX28AT.hpp"
#include "AX12A.hpp"
#include "AX18A.hpp"
#include "MX106.hpp"
#include "Braccio.hpp"

#ifndef __APPLE__
#include "lib/RF24.h"
#include "lib/nRF24L01.h"
#include "lib/RF24Network.h"

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_16MHZ);
//RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24);
RF24Network network(radio);

struct x
{
	short ID;
	short x;
	short y;
}received_data;

const uint16_t noeudMere = 00;
const uint16_t monNoeud = noeudMere;

#endif

using namespace std;

int main(int argc, char const *argv[])
{
    Braccio braccio;

    braccio.Infos();
    braccio.stand();
    braccio.moveAll(2000, 2000, 2000, 1000);
    braccio.moveAll(1200, 3000, 1200, 1200);
    braccio.moveWristVer(3000);
    braccio.moveElbow(1000);
    braccio.moveAll(3000, 1000, 3000, 3000);
    braccio.Infos();

/*

   #ifndef __APPLE__
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
			network.read(nHeader, &received_data, sizeof(received_data));
		}

		cout << "ID = " << received_data.ID << endl;
		cout << "X =  " << received_data.x << endl;
		cout << "Y =  " << received_data.y << endl;
	}	
    	#endif
*/
	return 0;
}
