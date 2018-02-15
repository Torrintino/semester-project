// api/stub/led-trigger/main.cpp

#include <cstdlib>
#include <iostream>

#include <signal.h>

#include "../../networking.hpp"
#include "api.pb.h"


using namespace codingtag;


static bool termination_requested;


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Error: Expected at least one argument." << std::endl;
        std::cerr << "Possible arguments are (all times are in seconds):" << std::endl
                  << "was hit:" << std::endl
                  << "  1 <time that the player is invulnerable>" << std::endl
                  << "has hit:" << std::endl
                  << "  2 [<time that it is shown>]" << std::endl
                  << "game starts:" << std::endl
                  << "  3 <delay> <runtime>" << std::endl
                  << "game ends:" << std::endl
                  << "  4 <delay>" << std::endl
                  << "dead:" << std::endl
                  << "  5" << std::endl
                  << "last life:" << std::endl
                  << "  6" << std::endl;
        return EXIT_FAILURE;
    }
    
    
    // ignore SIGPIPEs:
    signal(SIGPIPE, SIG_IGN);
    
    
    // do stuff:
    try {
        UnixSender sender("/var/run/codingtag-led-trigger-client.socket",
                          "/var/run/codingtag-hardware.socket");
        
        TriggerLEDEvent trigger_led_event;
        trigger_led_event.set_event(static_cast<uint32_t>(atoll(argv[1])));
        if (argc >= 3)
            trigger_led_event.set_time1(static_cast<uint32_t>(atoll(argv[2])));
        if (argc >= 4)
            trigger_led_event.set_time2(static_cast<uint32_t>(atoll(argv[3])));
        
        std::string to_send;
        bool serialized = trigger_led_event.SerializeToString(&to_send);
        
        if (!serialized) {
            std::cerr << "Error: The message could not be serialized." << std::endl;
            return EXIT_FAILURE;
        }
        
        sender.send(3, to_send);
        
        std::cerr << "Message sent." << std::endl;
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
