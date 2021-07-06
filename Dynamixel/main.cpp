#include "XL320.hpp"
#include "MX28AT.hpp"
#include <unistd.h>

int main(int argc, char const *argv[])
{
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

    // MX28AT moteur2(atoi(argv[1]));


    // moteur2.move(1000, true);
    // moteur2.ledOff();

    // moteur2.move(0, true);
    // moteur2.ledOn();
    // moteur2.move(500, true);
    // moteur2.ledOff();

    // moteur2.move(0, true);
    // moteur2.setSpeed(200);
    // moteur2.move(2047);
    // cout << "Current pos : " << moteur2.getPosition() << endl;
    // moteur2.disableTorque();

    // if (moteur2.getTorque())
    //     cout << "Torque enabled" << endl;
    // else
    //     cout << "Torque disabled" << endl;

    // cout << "Current pos : " << moteur2.getPosition() << endl;
}
