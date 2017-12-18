// api/stub/common/tcp-networking.hpp

#ifndef CODINGTAG_HARDWARE_API_STUB_COMMON_TCP_NETWORKING_HPP_HEADER_GUARD_
#define CODINGTAG_HARDWARE_API_STUB_COMMON_TCP_NETWORKING_HPP_HEADER_GUARD_ 1

#include <algorithm>
#include <set>

#include "../../networking.hpp"



namespace codingtag {

DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, ListenFailedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, UnknownHostError)


class TCPConnection final
{
public:
    TCPConnection(TCPConnection const& _other) = delete;
    TCPConnection(TCPConnection&& _other);
    TCPConnection& operator=(TCPConnection const& _other) & = delete;
    TCPConnection& operator=(TCPConnection&& _other) &;
    ~TCPConnection();
    
    void send(MessageHeader::TypeType _type, std::string const& _data) const;
    void receive(std::function<void(MessageHeader::TypeType, std::string)> _func) const;
    
    void shutdownForWriting() const noexcept;
    
    bool operator<(TCPConnection const& _other) const { return m_socket < _other.m_socket; }
    
    std::string const& getOtherEndName() const { return m_other_end; }
    
    
    static MessageHeader::TypeType const MessageTypeConnectionInit =
        std::numeric_limits<MessageHeader::TypeType>::max();
    
    static MessageHeader::TypeType const MessageTypeConnectionFinalize = 0;
    
    
private:
    friend class TCPServer;
    friend class TCPClient;
    
    TCPConnection(std::string const& _other_end_name);
    TCPConnection(CSocketWrapper&& _socket);
    
    CSocketWrapper m_socket;
    mutable std::string m_buffered_received_data;
    mutable std::string m_other_end;
};


class TCPServer final
{
public:
    TCPServer(uint16_t _port);
    TCPServer(TCPServer const& _other) = delete;
    TCPServer(TCPServer&& _other) = delete;
    TCPServer& operator=(TCPServer const& _other) & = delete;
    TCPServer& operator=(TCPServer&& _other) & = delete;
    ~TCPServer();
    
    void doForEachConnection(std::function<void(TCPConnection const&)> _func);
    void receive(std::function<void(TCPConnection const&, MessageHeader::TypeType,
                                    std::string)> _func);
    void shutdownConnection(TCPConnection const& _connection);
    
private:
    CSocketWrapper m_socket;
    std::set<TCPConnection> m_connections;
};


class TCPClient final
{
public:
    TCPClient(std::string const& _server, uint16_t _server_port);
    TCPClient(TCPClient const& _other) = delete;
    TCPClient(TCPClient&& _other) = delete;
    TCPClient& operator=(TCPClient const& _other) & = delete;
    TCPClient& operator=(TCPClient&& _other) & = delete;
    ~TCPClient();
    
    TCPConnection& getConnection() { return m_connection; };
    
private:
    TCPConnection m_connection;
};


} // namespace codingtag


#endif // CODINGTAG_HARDWARE_API_STUB_COMMON_TCP_NETWORKING_HPP_HEADER_GUARD_
