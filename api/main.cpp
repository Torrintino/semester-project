// api/main.cpp

#include <atomic>
#include <cstdlib>
#include <cstdio>
#include <exception>
#include <iostream>
#include <stdexcept>

#include <signal.h>

#include "interface-to-services.hpp"
#include "../driver/IrRecieveD.h"


using namespace codingtag;
using namespace codingtag::hardware;


static bool termination_requested;


extern "C" {
    static void signal_handler(int _signum, siginfo_t* _siginfo, void* _context)
    {
        fprintf(stderr, "Received SIGTERM.\n");
        termination_requested = true;
    }
}


int main()
{
    termination_requested = false;
    
    
    // ignore SIGPIPEs:
    signal(SIGPIPE, SIG_IGN);
    
    
    // handle SIGTERMs correctly:
    struct sigaction action;
    
    memset(&action, 0, sizeof(action));
    action.sa_sigaction = signal_handler;
    action.sa_flags = SA_SIGINFO;
    
    if (sigaction(SIGTERM, &action, NULL) != 0) {
        perror("Error: Signal handler for SIGTERM could not be set");
        return EXIT_FAILURE;
    }
    
    
    // init IR driver:
    {
        int iresult = initReceiver();
        
        if (iresult != 0) {
            std::cerr << "Error: IR Driver initialization failed." << std::endl;
            return EXIT_FAILURE;
        }
    }
    
    
    try {
        InterfaceToServices interface;
        
        while (!termination_requested) {
            try {
                interface.checkForReceivedMessages();
            }
            catch (MessageParsingError& e) {
                std::cerr << "Error: Catched MessageParsingError: " << e.what() << std::endl;
            }
            catch (MessageOfUnknownTypeError& e) {
                std::cerr << "Error: Catched MessageOfUnknownTypeError: " << e.what() << std::endl;
            }
            catch (ReceivedUnexpectedMessageException& e) {
                std::cerr << "Error: Catched ReceivedUnexpectedMessageException: "
                << e.what() << std::endl;
            }
            
            
            // TODO: send own ID about every second OR alternatively wait for input from Services
            
            char buffer[64];
            int received_id = readInput(buffer);
            
            if (received_id > 0) {
                try {
                    interface.receiveIR((uint32_t)received_id);
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
    
    deinitReceiver();
    return EXIT_SUCCESS;
    
failure:
    deinitReceiver();
    return EXIT_FAILURE;
}
