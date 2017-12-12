// api/main.cpp

#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "interface-to-services.hpp"

extern "C" {
    #include "../driver/IrRecieveD.h"
}


using namespace codingtag;
using namespace codingtag::hardware;

int main()
{
    // init IR driver:
    {
        int iresult = initReciever();
        
        if (iresult != 0) {
            std::cerr << "Error: IR Driver initialization failed." << std::endl;
            return EXIT_FAILURE;
        }
    
    }
    
    
    try {
        InterfaceToServices interface;
        
        while (true) { // TODO: When should it terminate?
            
            // TODO: send own ID about every second OR alternatively wait for input from Services
            
            char buffer[64]; // TODO: Is that way of using readInput correct?
            uint32_t received_id = readInput(buffer); // TODO: Does it block?
            
            if (true) { // TODO: only if there is a received ID
                try {
                    interface.receiveIR(received_id);
                }
                catch (ConnectFailedNoSuchFileException& e) {
                    std::cerr << "Error: Catched ConnectFailedNoSuchFileException: "
                              << e.what() << std::endl;
                }
                catch (ConnectFailedConnectionRefusedException& e) {
                    std::cerr << "Error: Catched ConnectFailedConnectionRefusedException: "
                              << e.what() << std::endl;
                }
            }
        }
    }
    catch (std::logic_error& e) {
        std::cerr << "Error: Catched unhandled instance of std::logic_error: "
                  << e.what() << std::endl;
        goto failure;
    }
    catch (std::runtime_error& e) {
        std::cerr << "Error: Catched unhandled instance of std::runtime_error: "
                  << e.what() << std::endl;
        goto failure;
    }
    catch (std::exception& e) {
        std::cerr << "Error: Catched unhandled instance of std::exception: "
                  << e.what() << std::endl;
        goto failure;
    }
    
    return EXIT_SUCCESS;
    
failure:
    deinitReciever();
    return EXIT_FAILURE;
}
