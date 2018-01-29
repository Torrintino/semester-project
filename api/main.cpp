// api/main.cpp

#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <signal.h>
#include <time.h>
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


DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, IRInitializationError)


class IRDriverWrapper
{
public:
    IRDriverWrapper() {
        int iresult = initReceiver();
        
        if (iresult == -1)
            throw IRInitializationError("IR Driver initialization failed.");
    }
    IRDriverWrapper(IRDriverWrapper const& _other) = delete;
    IRDriverWrapper(IRDriverWrapper&& _other) = delete;
    IRDriverWrapper& operator=(IRDriverWrapper const& _other) & = delete;
    IRDriverWrapper& operator=(IRDriverWrapper&& _other) & = delete;
    ~IRDriverWrapper() { deinitReceiver(); }
};


void trySendingIRToServices(InterfaceToServices& _interface, uint32_t _received_id)
{
    try {
        _interface.receiveIR(_received_id);
    }
    catch (ConnectFailedNoSuchFileException& e) {
        std::cerr << "Error: Catched ConnectFailedNoSuchFileException: " << e.what() << std::endl;
    }
    catch (ConnectFailedConnectionRefusedException& e) {
        std::cerr << "Error: Catched ConnectFailedConnectionRefusedException: " << e.what()
                  << std::endl;
    }
}


int main(int argc, char* argv[])
{
    termination_requested = false;
    
    srand(time(nullptr));
    
    bool simulate_random = false;
    bool simulate_manual = false;
    
    if (argc > 1) {
        if (argc == 2) {
            if (strcmp(argv[1], "simulate-random") == 0) {
                simulate_random = true;
            }
            else if (strcmp(argv[1], "simulate-manual") == 0) {
                simulate_manual = true;
            }
            else {
                std::cerr << "Error: Unknown option: \"" << argv[1] << "\"." << std::endl
                          << "Valid options are \"simulate-random\", \"simulate-manual\"."
                          << std::endl;
                return EXIT_FAILURE;
            }
        }
        else {
            std::cerr << "Error: Too many parameters were set. None or one was expected."
                      << std::endl
                      << "Valid options are \"simulate-random\", \"simulate-manual\"." << std::endl;
            return EXIT_FAILURE;
        }
    }
    
    
    // ignore SIGPIPEs:
    signal(SIGPIPE, SIG_IGN);
    
    
    // handle SIGTERMs correctly:
    struct sigaction action;
    
    memset(&action, 0, sizeof(action));
    action.sa_sigaction = signal_handler;
    action.sa_flags = SA_SIGINFO;
    
    if (sigaction(SIGTERM, &action, nullptr) != 0) {
        perror("Error: Signal handler for SIGTERM could not be set");
        return EXIT_FAILURE;
    }
    
    
    // get own client id:
    uint32_t own_client_id = 0;
    
    if ((!simulate_random) && (!simulate_manual))
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
    
    
    try {
        std::unique_ptr<IRDriverWrapper> ir_driver;
        
        if ((!simulate_random) && (!simulate_manual)) {
            ir_driver = std::make_unique<IRDriverWrapper>();
        }
        
        InterfaceToServices interface;
        
        std::cerr << "Initialization complete. Entering the event loop." << std::endl;
        
        if (simulate_random) {
            std::cerr << "Note: Random simulation mode is activated. No actual IR data is being "
                         "received or sent via the hardware."
                      << std::endl;
        }
        
        if (simulate_manual) {
            std::cerr << "Note: Manual simulation mode is activated. No actual IR data is being "
                         "received or sent via the hardware."
                      << std::endl;
        }
        
        
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
            
            
            // if we use the manual simulation mode, check if we have input to send to Services:
            if (simulate_manual) {
                fd_set set;
                FD_ZERO(&set);
                FD_SET(STDIN_FILENO, &set);
                
                struct timeval timeout;
                timeout.tv_sec = 0;
                timeout.tv_usec = 10000;
                
                int iresult = select(FD_SETSIZE, &set, nullptr, nullptr, &timeout);
                
                if (iresult >= 1) {
                    uint32_t signal_to_send = 0;
                    
                    iresult = fscanf(stdin, " %" PRIu32, &signal_to_send);
                    
                    if (iresult != 1) {
                        std::cout << "Error parsing the input: " << strerror(errno) << "."
                                  << std::endl;
                        while (true) {
                            iresult = select(FD_SETSIZE, &set, nullptr, nullptr, &timeout);
                            
                            if (iresult <= 0)
                                break;
                            
                            getchar();
                        }
                    }
                    
                    if (signal_to_send == 0) {
                        std::cout << "Error: Signal must not be zero." << std::endl;
                    }
                    else {
                        std::cout << "Sending received IR message " << signal_to_send
                                  << " to Services..." << std::endl;
                        trySendingIRToServices(interface, signal_to_send);
                    }
                }
            }
            
            
            // send own ID via IR every 0.5 seconds:
            {
                auto current_timepoint = std::chrono::system_clock::now();
                
                if (current_timepoint - last_timepoint > 500ms) {
                    if (simulate_random) {
                        uint32_t val = rand();
                        
                        if (val % 7 == 0) {    
                            trySendingIRToServices(interface, val % 11 + 2);
                        }
                    }
                    else if (!simulate_manual) {
                        sendCode(static_cast<int>(own_client_id));
                    }
                    
                    last_timepoint = current_timepoint;
                }
            }
            
            
            // check for incoming IR events:
            if ((!simulate_random) && (!simulate_manual))
            {
                char buffer[64];
                int received_id = readInput(buffer);
                
                if (received_id > 0) {
                    std::cerr << "Received IR message \"" << received_id << "\"." << std::endl;
                    trySendingIRToServices(interface, received_id);
                }
            }
            
            
            // wait a little so the PI does not get to hot:
            struct timespec gap = { 0, 2000000l };
            nanosleep(&gap, nullptr);
        }
    }
    catch (IRInitializationError& e) {
        std::cerr << "Error: IRInitializationError: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::logic_error& e) {
        std::cerr << "Error: Catched unhandled instance of std::logic_error: "
                  << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::runtime_error& e) {
        std::cerr << "Error: Catched unhandled instance of std::runtime_error: "
                  << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& e) {
        std::cerr << "Error: Catched unhandled instance of std::exception: "
                  << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
