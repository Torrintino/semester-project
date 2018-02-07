// api/led-event-status.cpp

#include <iostream>

#include <unistd.h>

#include <wiringPi.h>

#include "led-event-status.hpp"
#include "gpio-pins.h"


using namespace std::literals;

namespace codingtag {
namespace hardware {


LEDEventStatus::LEDEventStatus(int _wiring_pi_status)
        : m_stderr_is_terminal(isatty(STDERR_FILENO) == 1),
          m_wiring_pi_status(_wiring_pi_status),
          m_dead(false),
          m_last_life(false),
          m_game_starts_in(-1),
          m_game_ends_in(-1),
          m_invulnerable_led_left(-1),
          m_has_hit(-1),
          m_time_elapsed(0),
          m_last_timepoint(std::chrono::system_clock::now())
{
    if (m_wiring_pi_status != -1) {
        pinMode(LED_R0, OUTPUT);
        pinMode(LED_Y0, OUTPUT);
        pinMode(LED_G0, OUTPUT);
    }
}


LEDEventStatus::~LEDEventStatus() = default;

    
void LEDEventStatus::triggerLEDEvent(uint32_t _event, uint32_t _time1, uint32_t _time2)
{
    switch (_event) {
    case 1:
        m_invulnerable_led_left = _time1 * 100l;
        break;
    case 2:
        m_has_hit = (_time1 == 0 ? 4 : _time1) * 100l;
        break;
    case 3:
        if (_time2 == 0) {
            m_game_starts_in = _time1 * 100l;
            m_game_ends_in = 2000000000;
        }
        else {
            m_game_starts_in = _time1 * 100l;
            m_game_ends_in = m_game_starts_in + _time2 * 100l;
        }
        m_dead = false;
        m_last_life = false;
        m_invulnerable_led_left = -1;
        m_has_hit = -1;
        break;
    case 4:
        m_game_ends_in = _time1 * 100l;
        break;
    case 5:
        m_dead = true;
        m_last_life = false;
        break;
    case 6:
        if (!m_dead)
            m_last_life = true;
        break;
    default:
        std::cerr << "Error: Unknown LED event " << _event << ".";
    }
}


void LEDEventStatus::setNow(LEDEventStatus::Timepoint _t)
{
    int32_t elapsed = 0;
    
    if (_t > m_last_timepoint + 80ms) {
        m_last_timepoint += 80ms;
        elapsed += 8;
    }
    if (_t > m_last_timepoint + 40ms) {
        m_last_timepoint += 40ms;
        elapsed += 4;
    }
    if (_t > m_last_timepoint + 20ms) {
        m_last_timepoint += 20ms;
        elapsed += 2;
    }
    if (_t > m_last_timepoint + 10ms) {
        m_last_timepoint += 10ms;
        elapsed += 1;
    }
    
    m_time_elapsed += elapsed;
    m_game_starts_in -= elapsed;
    m_game_ends_in -= elapsed;
    m_invulnerable_led_left -= elapsed;
    m_has_hit -= elapsed;
    
    if (m_game_ends_in < 0) {
        m_dead = false;
        m_last_life = false;
    }
    
    if (elapsed >= 1)
        refreshLEDs();
}


void LEDEventStatus::refreshLEDs()
{
    bool r = false;
    bool y = false;
    bool g = false;
    
    if (m_game_ends_in >= 0) {
        // game is running or scheduled:
        if (m_game_starts_in >= 0) {
            // game is scheduled:
            if (m_game_starts_in >= 1000)
                r = y = g = (m_game_starts_in / 50) % 2 == 1;
            else if (m_game_starts_in >= 400)
                r = (m_game_starts_in / 50) % 2 == 1;
            else if (m_game_starts_in >= 200)
                r = true;
            else
                r = y = true;
        }
        else {
            // game is running:
            
            // WAS HIT / INVULNERABLE / DEAD DISPLAY (RED):
            if (m_dead)
                r = true;
            else {
                if (m_invulnerable_led_left >= 0) {
                    if (m_invulnerable_led_left >= 800)
                        r = (m_invulnerable_led_left / 70) % 2 == 1;
                    else if (m_invulnerable_led_left >= 400)
                        r = (m_invulnerable_led_left / 50) % 2 == 1;
                    else if (m_invulnerable_led_left >= 200)
                        r = (m_invulnerable_led_left / 25) % 2 == 1;
                    else if (m_invulnerable_led_left >= 100)
                        r = (m_invulnerable_led_left / 12) % 2 == 1;
                    else
                        r = (m_invulnerable_led_left / 6) % 2 == 1;
                }
            }
            
            // HAS HIT DISPLAY (YELLOW):
            if (m_has_hit >= 0) {
                y = true;
            }
            
            // GAME RUNNING DISPLAY (GREEN):
            if (m_game_ends_in >= 6400)
                g = true;
            else if (m_game_ends_in >= 3200)
                g = (m_game_ends_in / 25) % 12 != 0;
            else if (m_game_ends_in >= 1600)
                g = (m_game_ends_in / 25) % 8 != 0;
            else if (m_game_ends_in >= 800)
                g = (m_game_ends_in / 25) % 4 != 0;
            else if (m_game_ends_in >= 400)
                g = (m_game_ends_in / 25) % 2 == 1;
            else if (m_game_ends_in >= 200)
                g = (m_game_ends_in / 12) % 2 == 1;
            else if (m_game_ends_in >= 100)
                g = (m_game_ends_in / 6) % 2 == 1;
            
            // LAST LIFE DISPLAY (FLASH ALL LIGHTS EVERY 2 SECONDS):
            if (m_last_life && (m_time_elapsed / 10) % 23 == 0) {
                r = y = g = true;
            }
        }
    }
    else {
        // no game is running or scheduled:
        r = (m_time_elapsed / 17) % 3 == 0;
        y = (m_time_elapsed / 17) % 3 == 1;
        g = (m_time_elapsed / 17) % 3 == 2;
    }
    
    
    if (m_wiring_pi_status != -1) {
        digitalWrite(LED_R0, r ? HIGH : LOW);
        digitalWrite(LED_Y0, y ? HIGH : LOW);
        digitalWrite(LED_G0, g ? HIGH : LOW);
    }
    else {
        if (m_stderr_is_terminal) {
            char bits[5];
            bits[0] = ' ';
            bits[1] = r ? 'R' : '.';
            bits[2] = y ? 'Y' : '.';
            bits[3] = g ? 'G' : '.';
            bits[4] = '\0';
            fprintf(stderr, "%s\r", bits);
        }
    }
}


} // namespace hardware
} // namespace codingtag
