// api/tests/gpio-test.cpp

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>

#include <signal.h>
#include <time.h>
#include <unistd.h>

#include <wiringPi.h>


// pin IDs: 
//   Wiring  BCM_GPIO  Header
//      0       17       11  // in use for IR
//      1       18       12  // in use for IR
//      2       27       13
//      3       22       15
//      4       23       16
//      5       24       18
//      6       25       22
//      7        4        7
//      8        2        3
//      9        3        5
//     10        8       24
//     11        7       26


#define LED_R0 2
#define LED_Y0 3
#define LED_G0 4


using namespace std::literals;


static bool termination_requested;


extern "C" {
    static void signal_handler(int _signum, siginfo_t* _siginfo, void* _context)
    {
        fprintf(stderr, "Received SIGTERM.\n");
        termination_requested = true;
    }
}


int main(int argc, char* argv[])
{
    termination_requested = false;
    
    srand(time(nullptr));
    
    
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
    
    
    // try to initialize wiringPi:
    int wiring_pi_status = wiringPiSetup();
    
    if (wiring_pi_status == -1) {
        fprintf(stderr, "Error: WiringPi initalization failed! This error will be ignored.\n");
    }
    
    if (wiring_pi_status != -1) {
        pinMode(LED_R0, OUTPUT);
        pinMode(LED_Y0, OUTPUT);
        pinMode(LED_G0, OUTPUT);
    }
    
    
    bool led_status = false;
    
    
    try {
        auto last_timepoint = std::chrono::system_clock::now();
        
        while (!termination_requested) {
            // every 0.5 seconds:
            {
                auto current_timepoint = std::chrono::system_clock::now();
                
                if (current_timepoint - last_timepoint > 500ms) {
                    if (wiring_pi_status != -1) {
                        digitalWrite(LED_R0, led_status ? HIGH : LOW);
                        digitalWrite(LED_Y0, led_status ? HIGH : LOW);
                        digitalWrite(LED_G0, led_status ? HIGH : LOW);
                        led_status = !led_status;
                    }
                    
                    last_timepoint = current_timepoint;
                }
            }
            
            
            // wait a little so the PI does not get to hot:
            struct timespec gap = { 0, 100000000l };
            nanosleep(&gap, nullptr);
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
