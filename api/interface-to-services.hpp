// api/interface-to-services.hpp

#ifndef CODINGTAG_HARDWARE_API_INTERFACE_TO_SERVICES_HPP_HEADER_GUARD_
#define CODINGTAG_HARDWARE_API_INTERFACE_TO_SERVICES_HPP_HEADER_GUARD_ 1

#include <cstdint>
#include <memory>

#include "networking.hpp"


namespace codingtag {
namespace hardware {

DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, MessageSerializationError)


class InterfaceToServices final
{
public:
    InterfaceToServices();
    InterfaceToServices(InterfaceToServices const& _other) = delete;
    InterfaceToServices(InterfaceToServices&& _other) = delete;
    InterfaceToServices& operator=(InterfaceToServices const& _other) & = delete;
    InterfaceToServices& operator=(InterfaceToServices&& _other) & = delete;
    ~InterfaceToServices();
    
    void receiveIR(uint32_t _src_client_id);
    
private:
    std::unique_ptr<UnixSender> m_sender;
};


} // namespace hardware
} // namespace codingtag


#endif // CODINGTAG_HARDWARE_API_INTERFACE_TO_SERVICES_HPP_HEADER_GUARD_
