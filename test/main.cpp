#include "XL320.hpp"

int main(int argc, char const *argv[])
{
    XL320 moteur(1);
    moteur.move(1000);
    return 0;
}
