// api/stub/common/tcp-networking.cpp

#include <arpa/inet.h>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <limits>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

#include "tcp-networking.hpp"


namespace codingtag {


////////////////////////////////////////////////////////////////////////////////////////////////////
// TCPConnection ///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

TCPConnection::TCPConnection(std::string const& _other_end_name)
        : m_socket(PF_INET, SOCK_STREAM, 0), m_other_end(_other_end_name)
{
    // make socket non-blocking:
    fcntl(m_socket.getFD(), F_SETFL, O_NONBLOCK); // ignore errors
    
    // disable Nagle's algorithm
    int flag = 1;
    setsockopt(m_socket.getFD(), IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
            // ignore errors
}


TCPConnection::TCPConnection(CSocketWrapper&& _socket)
        : m_socket(std::move(_socket))
{
    // make socket non-blocking:
    fcntl(m_socket.getFD(), F_SETFL, O_NONBLOCK); // ignore errors
    
    // disable Nagle's algorithm
    int flag = 1;
    setsockopt(m_socket.getFD(), IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
            // ignore errors
}


TCPConnection::TCPConnection(TCPConnection&& _other)
        : m_socket(std::move(_other.m_socket)),
          m_buffered_received_data(std::move(_other.m_buffered_received_data)),
          m_other_end(std::move(_other.m_other_end))
{
    // intentionally empty
}


TCPConnection& TCPConnection::operator=(TCPConnection&& _other) &
{
    if (this == &_other)
        return *this;
    
    m_socket = std::move(_other.m_socket);
    m_buffered_received_data = std::move(_other.m_buffered_received_data);
    m_other_end = std::move(_other.m_other_end);
    
    return *this;
}


TCPConnection::~TCPConnection() = default;


void TCPConnection::send(MessageHeader::TypeType _type, std::string const& _data) const
{
    if (       _data.length() > std::numeric_limits<uint32_t>::max()
            || _data.length() > std::numeric_limits<ssize_t>::max()) {
        throw std::logic_error("Data too long.");
    }
    
    MessageHeader header = {
            htonl(static_cast<uint32_t>(_data.length())), // size
            htonl(_type)                                  // type
    };
    
    ssize_t sent;
    
    do {
        sent = ::send(m_socket.getFD(), &header, sizeof(header), 0);
    } while (sent < 0 && errno == EWOULDBLOCK);
    
    if (sent < 0) {
        if (errno == ECONNREFUSED)
            throw SendFailedConnectionRefusedException("Cannot send message header: Connection "
                                                       "refused.");
        if (errno == ENOTCONN)
            throw SendFailedNotConnectedException("Cannot send message header: Not connected.");
        perror("Error: Cannot send message header");
        throw SendFailedException("Cannot send message header.");
    }
    
    do {
        sent = ::send(m_socket.getFD(), _data.c_str(), _data.length(), 0);
    } while (sent < 0 && errno == EWOULDBLOCK);
    
    if (sent < 0) {
        if (errno == ECONNREFUSED)
            throw SendFailedConnectionRefusedException("Cannot send message: Connection refused.");
        if (errno == ENOTCONN)
            throw SendFailedNotConnectedException("Cannot send message: Not connected.");
        perror("Error: Cannot send message");
        throw SendFailedException("Cannot send message.");
    }
}


void TCPConnection::receive(std::function<void(MessageHeader::TypeType, std::string)> _func) const
{
    while (true)
    {
        char buffer[8192];
        
        memset(buffer, 0, 8192);
        
        ssize_t received = recv(m_socket.getFD(), buffer, 8192, 0);
        
        if (received < 0) {
            if (errno == EWOULDBLOCK) // no data is available
                return;
            perror("Error: Cannot receive message");
            throw RecvFailedException("Cannot receive message.");
        }
        
        if (received == 0) {
            if (m_buffered_received_data.length() != 0) {
                std::cerr << "Warning: No further data to receive from TCP-Socket. Discarding "
                          << m_buffered_received_data.length() << " bytes of data." << std::endl;
            }
            _func(MessageTypeConnectionFinalize, "");
            return;
        }
        
        m_buffered_received_data += std::string(buffer, static_cast<size_t>(received));
        
        while (m_buffered_received_data.length() >= sizeof(MessageHeader)) {
            MessageHeader header;
            memcpy(&header, m_buffered_received_data.c_str(), sizeof(header));
            header.size = ntohl(header.size);
            header.type = ntohl(header.type);
            
            if (m_buffered_received_data.length() - sizeof(MessageHeader) < header.size)
                break;
            
            if (header.type == MessageTypeConnectionInit) {
                m_other_end = m_buffered_received_data.substr(sizeof(MessageHeader),
                                                              header.size);
            }
            
            _func(header.type, m_buffered_received_data.substr(sizeof(MessageHeader),
                                                                   header.size));
            
            m_buffered_received_data = m_buffered_received_data.substr(
                    header.size + sizeof(MessageHeader),
                    m_buffered_received_data.length() - header.size - sizeof(MessageHeader));
        }
    }
}


void TCPConnection::shutdownForWriting() const noexcept
{
    m_socket.shutdown(SHUT_WR);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// TCPServer ///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

TCPServer::TCPServer(uint16_t _port)
        : m_socket(PF_INET, SOCK_STREAM, 0)
{
    struct sockaddr_in name;
    memset(&name, 0, sizeof(name));
    
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    name.sin_port = htons(_port);
    
    int iresult = bind(m_socket.getFD(), (struct sockaddr*)&name, sizeof(name));
    
    if (iresult == -1) {
        perror("Error: Cannot bind TCP socket");
        throw BindFailedException("Cannot bind TCP socket.");
    }
    
    iresult = listen(m_socket.getFD(), 16);
    
    if (iresult == -1) {
        perror("Error: Cannot listen to TCP socket");
        throw ListenFailedException("Cannot listen to TCP socket.");
    }
}


TCPServer::~TCPServer() = default;


void TCPServer::doForEachConnection(std::function<void(TCPConnection const&)> _func)
{
    for (auto const& connection : m_connections) {
        _func(connection);
    }
}


void TCPServer::receive(std::function<void(TCPConnection const&, MessageHeader::TypeType,
                                           std::string)> _func)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    
    fd_set set;
    FD_ZERO(&set);
    FD_SET(m_socket.getFD(), &set);
            
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 50000;
    
    int iresult = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
    
    if (iresult >= 1) {
        m_connections.emplace(TCPConnection(std::move(m_socket.accept((struct sockaddr*)&addr,
                                                                      &addr_len))));
    }
    
    
    bool retry = false;
    
    do {
        retry = false;
        for (auto const& connection : m_connections) {
            bool can_be_deleted = false;
            connection.receive([&](auto _type, auto _data) {
                _func(connection, _type, _data);
                if (_type == TCPConnection::MessageTypeConnectionFinalize && _data.length() == 0)
                    can_be_deleted = true;
            });
            if (can_be_deleted) {
                retry = true;
                shutdownConnection(connection);
                break;
            }
        }
    } while (retry);
}


void TCPServer::shutdownConnection(TCPConnection const& _connection)
{
    _connection.shutdownForWriting();
    m_connections.erase(_connection);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// TCPClient ///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

TCPClient::TCPClient(std::string const& _server, uint16_t _server_port)
        : m_connection(_server)
{
    struct sockaddr_in name;
    memset(&name, 0, sizeof(name));
    
    name.sin_family = AF_INET;
    name.sin_port = htons(_server_port);
    
    struct hostent* hostinfo = gethostbyname(_server.c_str());
    
    if (hostinfo == NULL) {
        perror("Error: Unknown host");
        throw UnknownHostError("Unknown host.");
    }
    
    name.sin_addr = *(struct in_addr*)hostinfo->h_addr;
    
    int iresult = connect(m_connection.m_socket.getFD(), (struct sockaddr*)&name, sizeof(name));
    
    if (iresult != 0) {
        if (errno == EINPROGRESS) {
            fd_set set;
            FD_ZERO(&set);
            FD_SET(m_connection.m_socket.getFD(), &set);
            
            struct timeval timeout;
            timeout.tv_sec = 8;
            timeout.tv_usec = 0;
            
            iresult = select(FD_SETSIZE, NULL, &set, NULL, &timeout);
            
            if (iresult > 0) {
                std::clog << "Connected to \"" << _server << "\" port " << _server_port
                          << std::endl;
                goto connection_established;
            }
                
            if (iresult == 0)
                errno = ECONNREFUSED;
        }
        
        if (errno == ECONNREFUSED)
            throw ConnectFailedConnectionRefusedException("Cannot connect to server: "
                                                          "Connection refused.");
        perror("Error: Cannot connect to server");
        throw ConnectFailedException("Cannot connect to server.");
    }
    
    
connection_established:
    // send the own hostname:
    {
        char hostname[256];
        
        int iresult = gethostname(hostname, 256);
        
        if (iresult != 0) {
            perror("Error: Could not get hostname to tell the opposite end of the TCP connection "
                   "who I am");
        }
    
        m_connection.send(TCPConnection::MessageTypeConnectionInit,
                          std::string(hostname));
    }
}


TCPClient::~TCPClient() = default;

} // namespace codingtag
