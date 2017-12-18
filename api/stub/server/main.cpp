// api/stub/server/main.cpp

#include <cstdlib>
#include <iostream>

#include <signal.h>
#include <string.h>

#include "../common/tcp-networking.hpp"


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
        TCPServer server(7436);
        
        while (!termination_requested) {
            try {
                server.receive([&](auto const& _connection, auto _type, auto _data) {
                    if (_type == TCPConnection::MessageTypeConnectionInit) {
                        std::cout << _connection.getOtherEndName() << " connected." << std::endl;
                    }
                    else if (_type == TCPConnection::MessageTypeConnectionFinalize
                             && _data.length() == 0) {
                        std::cout << _connection.getOtherEndName() << " disconnected." << std::endl;
                    }
                    else {
                        std::cout << _connection.getOtherEndName() << ": " << _data << std::endl;
                    }
                });
            }
            catch (RecvFailedException& e) {
                std::cerr << "Error: Catched unhandled instance of RecvFailedException: "
                          << e.what() << std::endl;
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
}
