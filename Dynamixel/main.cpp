#include "XL320.hpp"

int main(int argc, char const *argv[])
{
    XL320 moteur(atoi(argv[1]));
    moteur.move(1000);
    moteur.move(0);
    moteur.move(500);
}
