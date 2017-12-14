// api/main.cpp

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>

#include <signal.h>
#include <unistd.h>

#include "interface-to-services.hpp"
#include "../driver/IrRecieveD.h"


using namespace std::literals;

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
    
    
    // get own client id:
    uint32_t own_client_id;
    {
        char hostname[256];
        
        int iresult = gethostname(hostname, 256);
        
        if (iresult != 0) {
            perror("Error: Could not get hostname which is required for client identification");
            return EXIT_FAILURE;
        }
        
        // check whether the hostname starts with "client-":
        if (memcmp(hostname, "client-", 7) != 0) {
            std::cerr << "Error: Hostname \"" << hostname
                      << "\" has invalid pattern. The pattern \"client-N\" was expected."
                      << std::endl;
            return EXIT_FAILURE;
        }
        
        own_client_id = 0;
        
        // parse the client id:
        for (size_t i = 7; true; ++i) {
            if (hostname[i] == '\0') {
                break;
            }
            else if (i >= 10) {
                std::cerr << "Error: Hostname \"" << hostname
                          << "\" has invalid pattern. The pattern \"client-N\" was expected where "
                             "\"N\" is a positive integer with maximum 3 digits." << std::endl;
                return EXIT_FAILURE;
            }
            else if (hostname[i] >= '0' && hostname[i] <= '9') {
                own_client_id *= 10;
                own_client_id += static_cast<uint32_t>(hostname[i] - '0');
            }
            else {
                std::cerr << "Error: Hostname \"" << hostname
                          << "\" has invalid pattern. The pattern \"client-N\" was expected where "
                             "\"N\" is a positive integer with maximum 3 digits." << std::endl;
                return EXIT_FAILURE;
            }
        }
        
        if (own_client_id == 0) {
            std::cerr << "Error: Hostname \"" << hostname
                      << "\" not allowed. Client ID must be bigger than 0." << std::endl;
            return EXIT_FAILURE;
        }
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
        
        auto last_timepoint = std::chrono::system_clock::now();
        
        while (!termination_requested) {
            // check for incoming messages from Services:
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
            
            
            // send own ID via IR every 0.5 seconds:
            {
                auto current_timepoint = std::chrono::system_clock::now();
                
                if (current_timepoint - last_timepoint > 500ms) {
                    last_timepoint = current_timepoint;
                    sendCode(static_cast<int>(own_client_id));
                }
            }
            
            
            // check for incoming IR events:
            {
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
