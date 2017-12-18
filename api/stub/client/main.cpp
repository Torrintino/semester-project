// api/stub/client/main.cpp

#include <cstdlib>
#include <iostream>

#include <signal.h>
#include <string.h>

#include "../common/tcp-networking.hpp"
#include "api.pb.h"


using namespace codingtag;


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
    
    
    // do stuff:
    try {
        while (!termination_requested) {
            try {
                TCPClient client("server-1", 7436);
                UnixReceiver receiver("/var/run/codingtag-services.socket");
                
                while (!termination_requested) {
                    receiver.receive([&](MessageHeader::TypeType _type, std::string _data) {
                        switch (_type) {
                        case 2: {
                            ReceiveIR receive_ir;
                            bool parsed = receive_ir.ParseFromString(_data);
                            
                            if (!parsed)
                                return; // ignore message
                            
                            client.getConnection().send(2,
                                                        "received IR signal \""
                                                        + std::to_string(receive_ir.src_client_id())
                                                        + "\".");
                            break;
                        }
                        default:
                            break;
                        }
                    });
                }
            }
            catch (std::runtime_error& e) {
                std::cerr << "Error: Catched unhandled instance of std::runtime_error: "
                          << e.what() << std::endl;
                return EXIT_FAILURE;
            }
        }
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
    
    // do stuff:
    
    return EXIT_SUCCESS;
}
