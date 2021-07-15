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

#endif

using namespace std;

struct x
{
	short ID;
	short x;
	short y;
}received_data;

const uint16_t noeudMere = 00;
const uint16_t monNoeud = noeudMere;

int main(int argc, char const *argv[])
{
    /*
    XL320 moteur(atoi(argv[1]));

    moteur.move(1000, true);
    moteur.setLed(YELLOW);
    moteur.move(0, true);
    moteur.setLed(BLUE);
    moteur.move(500, true);
    moteur.setLed(RED);

    moteur.move(0, true);
    moteur.setSpeed(200);
    moteur.move(2047);
    cout << "Current pos : " << moteur.getPosition() << endl;
    moteur.disableTorque();

    if (moteur.getTorque())
        cout << "Torque enabled" << endl;
    else
        cout << "Torque disabled" << endl;

    cout << "Current pos : " << moteur.getPosition() << endl;
    moteur.Infos();
	

    //  MX28AT moteur2(atoi(argv[1]));

    //  moteur2.move(1000, true);
    //  moteur2.ledOff();

    //  moteur2.move(0, true);
    //  moteur2.ledOn();
    //  moteur2.move(500, true);
    //  moteur2.ledOff();

    //  moteur2.move(0, true);
    //  moteur2.setSpeed(200);
    //  moteur2.move(2047);
    //  cout << "Current pos : " << moteur2.getPosition() << endl;
    //  moteur2.disableTorque();

    //  if (moteur2.getTorque())
    //      cout << "Torque enabled" << endl;
    //  else
    //      cout << "Torque disabled" << endl;

    //  cout << "Current pos : " << moteur2.getPosition() << endl;
    //  moteur2.Infos();

    // MX64AT moteur3(atoi(argv[1]));

    // moteur3.move(1000, true);
    // moteur3.ledOff();

    // moteur3.move(0, true);
    // moteur3.ledOn();
    // moteur3.move(500, true);
    // moteur3.ledOff();

    // moteur3.move(0, true);
    // moteur3.setSpeed(200);
    // moteur3.move(2047);
    // cout << "Current pos : " << moteur3.getPosition() << endl;
    // moteur3.disableTorque();

    // if (moteur3.getTorque())
    //     cout << "Torque enabled" << endl;
    // else
    //     cout << "Torque disabled" << endl;

    // cout << "Current pos : " << moteur3.getPosition() << endl;
    // moteur3.Infos();

    // AX12A moteur4(atoi(argv[1]));

    // moteur4.move(1000, true);
    // moteur4.ledOff();

    // moteur4.move(0, true);
    // moteur4.ledOn();
    // moteur4.move(500, true);
    // moteur4.ledOff();

    // moteur4.move(0, true);
    // moteur4.setSpeed(200);
    // moteur4.move(2047);
    // cout << "Current pos : " << moteur4.getPosition() << endl;
    // moteur4.disableTorque();

    // if (moteur4.getTorque())
    //     cout << "Torque enabled" << endl;
    // else
    //     cout << "Torque disabled" << endl;

    // cout << "Current pos : " << moteur4.getPosition() << endl;
    // moteur4.Infos();

    MX106 moteur5(atoi(argv[1]));

    moteur5.move(1000, true);
    moteur5.ledOff();

    moteur5.move(0, true);
    moteur5.ledOn();
    moteur5.move(500, true);
    moteur5.ledOff();

    moteur5.move(0, true);
    moteur5.setSpeed(200);
    moteur5.move(2047);
    cout << "Current pos : " << moteur5.getPosition() << endl;
    moteur5.disableTorque();

    if (moteur5.getTorque())
        cout << "Torque enabled" << endl;
    else
        cout << "Torque disabled" << endl;

    cout << "Current pos : " << moteur5.getPosition() << endl;
    moteur5.Infos();
    */

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

    Braccio braccio;

	return 0;
}
