#include "XL320.hpp"

int main(int argc, char const *argv[])
{
    XL320 moteur(atoi(argv[1]));
    moteur.move(1000);
    moteur.setLed(YELLOW);
    moteur.move(0);
    moteur.setLed(BLUE);
    moteur.move(500);
    moteur.setLed(RED);

    moteur.move(0);
    moteur.setSpeed(200);
    cout << "Current pos : " << moteur.getPosition() << endl;
    moteur.disableTorque();

    if (moteur.getTorque())
        cout << "Torque enabled" << endl;
    else
        cout << "Torque disabled" << endl;

    cout << "Current pos : " << moteur.getPosition() << endl;
}
