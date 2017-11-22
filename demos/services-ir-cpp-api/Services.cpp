// Services.cpp

#include <iostream> // just for demo
#include <cstdlib> // just for demo

#include <unistd.h> // just for demo

#include "IR.h"


int main()
{
    hardware::ir::initIR();

    for (uint32_t i = 0; i < 30; ++i) {
        hardware::ir::checkForIncomingIR([&](uint32_t _data) {
            // Hier kann man jetzt mit den empfangenen Daten machen, was man will.
            std::cout << ("Received: " + std::to_string(_data) + "\n");
        });

        if (i % 2 == 0) {
            // Hier werden zum Beispiel alle 2 Sekunden Daten gesendet.
            hardware::ir::sendIR(i);
        }

        sleep(1);
    }

    return EXIT_SUCCESS;
}
