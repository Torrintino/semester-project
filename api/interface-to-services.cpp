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


InterfaceToServices::InterfaceToServices()
        : m_receiver(hardware_socket_file)
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
            // TODO: No messages to receive from Services yet.
        default:
            throw MessageOfUnknownTypeError("Received message of unknown type "
                                            + std::to_string(_type) + ".");
        }
    });
}


} // namespace hardware
} // namespace codingtag
