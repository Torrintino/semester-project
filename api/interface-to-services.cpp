// api/interface-to-services.cpp

#include <string>

#include "interface-to-services.hpp"
#include "api.pb.h"


namespace codingtag {
namespace hardware {


static std::string const hardware_socket_file = "/var/run/codingtag-hardware.socket";
static std::string const hardware_client_socket_file = "/var/run/codingtag-hardware-client.socket";
static std::string const services_socket_file = "/var/run/codingtag-services.socket";


#if 0 // just for testing purposes
static std::string const hardware_socket_file = "codingtag-hardware.socket";
static std::string const hardware_client_socket_file = "codingtag-hardware-client.socket";
static std::string const services_socket_file = "codingtag-services.socket";
#endif


namespace messages {
    static MessageHeader::TypeType const SendIR = 1;
    static MessageHeader::TypeType const ReceiveIR = 2;
}


InterfaceToServices::InterfaceToServices(std::function<void(uint32_t, uint32_t, uint32_t)>
                                             _led_event_callback_function)
        : m_receiver(hardware_socket_file),
          m_led_event_callback_function(_led_event_callback_function)
{
    // intentionally empty
}


InterfaceToServices::~InterfaceToServices() = default;


void InterfaceToServices::receiveIR(uint32_t _src_client_id)
{
    if (m_sender == nullptr) {
        m_sender = std::make_unique<UnixSender>(hardware_client_socket_file, services_socket_file);
    }
    
    ReceiveIR receive_ir;
    receive_ir.set_src_client_id(_src_client_id);
                    
    std::string to_send;
    bool serialized = receive_ir.SerializeToString(&to_send);
    
    if (!serialized)
        throw MessageSerializationError("Google Protocol Buffers message ReceiveIR cannot be "
                                        "serialized.");
    
    try {
        m_sender->send(messages::ReceiveIR, to_send);
    }
    catch (SendFailedConnectionError& e) {
        m_sender = nullptr;
        // If the connection fails then the Receiver has most likely crashed. Try to reconnect by
        // recreating the socket:
        m_sender = std::make_unique<UnixSender>(hardware_client_socket_file, services_socket_file);
    }
}


void InterfaceToServices::checkForReceivedMessages()
{
    m_receiver.receive([&](MessageHeader::TypeType _type, std::string _data) {
        switch (_type) {
        case 3: {
            TriggerLEDEvent trigger_led_event;
            bool parsed = trigger_led_event.ParseFromString(_data);
            
            if (!parsed)
                throw MessageParsingError("Received message of type TriggerLEDEvent that could not "
                                          "be parsed");
            
            uint32_t event = trigger_led_event.event();
            uint32_t time1 = trigger_led_event.has_time1() ? trigger_led_event.time1() : 0;
            uint32_t time2 = trigger_led_event.has_time2() ? trigger_led_event.time2() : 0;
            
            if (event < 1 || event > 6)
                throw MessageOfUnknownTypeError("Received TriggerLEDEvent with an unknown event "
                                                + std::to_string(event) + ".");
            
            m_led_event_callback_function(event, time1, time2);
            
            break;
        }
        default:
            throw MessageOfUnknownTypeError("Received message of unknown type "
                                            + std::to_string(_type) + ".");
        }
    });
}


} // namespace hardware
} // namespace codingtag
