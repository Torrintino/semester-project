// api/interface-to-services.hpp

#ifndef CODINGTAG_HARDWARE_API_INTERFACE_TO_SERVICES_HPP_HEADER_GUARD_
#define CODINGTAG_HARDWARE_API_INTERFACE_TO_SERVICES_HPP_HEADER_GUARD_ 1

#include <cstdint>
#include <functional>
#include <memory>

#include "networking.hpp"


namespace codingtag {
namespace hardware {

DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, MessageSerializationError)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, MessageParsingError)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, MessageOfUnknownTypeError)


class InterfaceToServices final
{
public:
    InterfaceToServices(std::function<void(uint32_t, uint32_t, uint32_t)>
                            _led_event_callback_function);
    InterfaceToServices(InterfaceToServices const& _other) = delete;
    InterfaceToServices(InterfaceToServices&& _other) = delete;
    InterfaceToServices& operator=(InterfaceToServices const& _other) & = delete;
    InterfaceToServices& operator=(InterfaceToServices&& _other) & = delete;
    ~InterfaceToServices();
    
    void receiveIR(uint32_t _src_client_id);
    
    void checkForReceivedMessages();
    
private:
    UnixReceiver m_receiver;
    std::unique_ptr<UnixSender> m_sender;
    std::function<void(uint32_t, uint32_t, uint32_t)> m_led_event_callback_function;
};


} // namespace hardware
} // namespace codingtag


#endif // CODINGTAG_HARDWARE_API_INTERFACE_TO_SERVICES_HPP_HEADER_GUARD_
