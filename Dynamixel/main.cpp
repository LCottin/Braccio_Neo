#include "XL320.hpp"

int main(int argc, char const *argv[])
{
    XL320 moteur(atoi(argv[1]));
    moteur.setLed(YELLOW);
    moteur.move(1000);
    moteur.setLed(BLUE);
    moteur.move(0);
    moteur.setLed(RED);
    moteur.move(500);
}
