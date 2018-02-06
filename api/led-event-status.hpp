// api/led-event-status.hpp

#ifndef CODINGTAG_HARDWARE_API_LED_EVENT_STATUS_HPP_HEADER_GUARD_
#define CODINGTAG_HARDWARE_API_LED_EVENT_STATUS_HPP_HEADER_GUARD_ 1

#include <chrono>
#include <cstdint>


namespace codingtag {
namespace hardware {


class LEDEventStatus final
{
public:
    typedef decltype(std::chrono::system_clock::now()) Timepoint;
    
    LEDEventStatus(int _wiring_pi_status);
    LEDEventStatus(LEDEventStatus const& _other) = delete;
    LEDEventStatus(LEDEventStatus&& _other) = delete;
    LEDEventStatus& operator=(LEDEventStatus const& _other) & = delete;
    LEDEventStatus& operator=(LEDEventStatus&& _other) & = delete;
    ~LEDEventStatus();
    
    void triggerLEDEvent(uint32_t _event, uint32_t _time1, uint32_t _time2);
    void setNow(Timepoint _t);
    
    bool isDead() { return m_dead; }
    
private:
    void refreshLEDs();
    
    bool m_stderr_is_terminal;
    int m_wiring_pi_status;
    bool m_dead;
    bool m_last_life;
    int64_t m_game_starts_in;
        // time in 10s of milliseconds the game starts in
        // negative value: no game scheduled
    int64_t m_game_ends_in;
        // time in 10s of milliseconds the game ends in
        // negative value: no game running or scheduled
    int64_t m_invulnerable_led_left;
        // time in 10s of milliseconds in which the player is still invulnerable
        // negative value: vulnerable
    int64_t m_has_hit;
        // time in 10s of milliseconds that a hit is shown
        // negative value: has not hit anything recently
    uint64_t m_time_elapsed;
        // total time elapsed in 10s of milliseconds
    Timepoint m_last_timepoint;
};


} // namespace hardware
} // namespace codingtag


#endif // CODINGTAG_HARDWARE_API_LED_EVENT_STATUS_HPP_HEADER_GUARD_
