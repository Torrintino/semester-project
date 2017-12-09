// api/networking.cpp

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <utility>

#include "networking.hpp"


namespace codingtag {

static void bindUnixSocket(CSocketWrapper& _socket, std::string const& _file_name)
{
    struct sockaddr_un name;
    name.sun_family = AF_LOCAL;
    
    if (_file_name.length() >= sizeof(name.sun_path)) {
        throw std::logic_error("Cannot bind local Unix socket: file name too long");
    }
    
    memcpy(name.sun_path, _file_name.c_str(), _file_name.length());
    name.sun_path[_file_name.length()] = '\0';
    
    int iresult = unlink(_file_name.c_str());
    
    if (iresult != 0 && errno != ENOENT) { // ignore non-existing files
        perror("Error: Cannot unlink file for local Unix socket");
        throw UnlinkFailedException("Cannot unlink file " + _file_name + " for local Unix socket.");
    }
    
    iresult = bind(_socket.getFD(), (struct sockaddr*)&name,
                   offsetof(struct sockaddr_un, sun_path) + _file_name.length() + 1);
    
    if (iresult != 0) {
        perror("Error: Cannot bind local Unix socket");
        throw BindFailedException("Cannot bind local Unix socket.");
    }
}


static void sendToSocket(CSocketWrapper& _socket, MessageHeader::TypeType _type,
                         std::string const& _data)
{
    if (_data.length() > std::numeric_limits<MessageHeader::TypeType>::max()) {
        throw std::logic_error("Data too long.");
    }
    
    MessageHeader header = {
            static_cast<uint32_t>(_data.length()), // size
            _type                                  // type
    };
    
    ssize_t sent = send(_socket.getFD(), &header, sizeof(header), 0);
    
    if (sent < 0) {
        perror("Error: Cannot send message header");
        throw SendFailedException("Cannot send message header.");
    }
    
    sent = send(_socket.getFD(), _data.c_str(), _data.length(), 0);
    
    if (sent < 0) {
        perror("Error: Cannot send message");
        throw SendFailedException("Cannot send message.");
    }
}


static void receiveFromSocket(CSocketWrapper& _socket,
                              bool& _header_written, MessageHeader& _header,
                              std::function<void(MessageHeader::TypeType, std::string)> _func)
{
    ssize_t received;
    
    if (!_header_written) {
        received = recv(_socket.getFD(), &_header, sizeof(_header), 0);
        
        if (received < 0) {
            if (errno == EWOULDBLOCK) // no data is available
                return;
            perror("Error: Cannot receive message");
            throw RecvFailedException("Cannot receive message.");
        }
        
        if (received != sizeof(_header)) {
            throw ReceivedUnexpectedMessageException("Did not receive the entire header.");
        }
        
        _header_written = true;
    }
    
    if (_header.size > std::numeric_limits<size_t>::max() - 1) {
        throw ReceivedUnexpectedMessageException("Received message header with too large value for "
                                                 "size.");
    }
    
    std::unique_ptr<char> data = std::make_unique<char>(static_cast<size_t>(_header.size) + 1);
    
    received = recv(_socket.getFD(), data.get(), static_cast<size_t>(_header.size) + 1, 0);
    
    if (received < 0) {
        if (errno == EWOULDBLOCK) // no data is available
            return;
        perror("Error: Cannot receive message");
        throw RecvFailedException("Cannot receive message.");
    }
    
    _header_written = false;
    
    if (received != _header.size) {
        throw ReceivedUnexpectedMessageException("Received message of unexpected size.");
    }
    
    _func(_header.type, std::string(data.get(), _header.size));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// CSocketWrapper //////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

CSocketWrapper::CSocketWrapper(int _namespace, int _style, int _protocol)
        : m_sockfd(socket(_namespace, _style, _protocol))
{
    if (m_sockfd == -1) {
        perror("Error: Cannot create socket");
        throw SockFailedException("Cannot create socket.");
    }
}


CSocketWrapper::CSocketWrapper(CSocketWrapper&& _other)
        : m_sockfd(std::exchange(_other.m_sockfd, -1))
{
    // intentionally empty
}


CSocketWrapper::~CSocketWrapper()
{
    if (m_sockfd == -1)
        return;
    
    int iresult = ::close(m_sockfd);
    
    if (iresult != 0) {
        perror("Error: Cannot close connection socket");
    }
}


CSocketWrapper CSocketWrapper::accept(struct sockaddr* _addr, socklen_t* _length_ptr)
{
    int new_sockfd = ::accept(m_sockfd, _addr, _length_ptr);
    
    if (new_sockfd == -1) {
        perror("Error: Cannot accept connection to socket");
        throw AcceptFailedException("Cannot accept connection to socket.");
    }
    
    return CSocketWrapper(new_sockfd);
}


void CSocketWrapper::shutdown(int _how) noexcept
{
    int iresult = ::shutdown(m_sockfd, _how);
    
    if (iresult != 0) {
        perror("Error: Cannot shutdown socket");
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// FileDeleter /////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

FileDeleter::FileDeleter(std::string const& _file_name)
        : m_file_name(_file_name)
{
    // intentionally empty
}


FileDeleter::~FileDeleter()
{
    unlink(m_file_name.c_str()); // ignore errors
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// UnixReceiver ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

UnixReceiver::UnixReceiver(std::string const& _file_name)
        : m_socket_file_deleter(_file_name),
          m_socket(AF_LOCAL, SOCK_DGRAM, 0),
          m_buffered_header_available(false)
{
    bindUnixSocket(m_socket, _file_name);
    
    // make socket non-blocking:
    fcntl(m_socket.getFD(), F_SETFL, O_NONBLOCK); // ignore errors
}


UnixReceiver::~UnixReceiver() = default;


void UnixReceiver::receive(std::function<void(MessageHeader::TypeType, std::string)> _func)
{
    receiveFromSocket(m_socket, m_buffered_header_available, m_buffered_header, _func);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// UnixSender //////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

UnixSender::UnixSender(std::string const& _file_name, std::string const& _receiver_file_name)
        : m_socket_file_deleter(_file_name), m_socket(AF_LOCAL, SOCK_DGRAM, 0)
{
    bindUnixSocket(m_socket, _file_name);
    
    // connect to server:
    struct sockaddr_un name;
    name.sun_family = AF_LOCAL;
    
    if (_receiver_file_name.length() >= sizeof(name.sun_path)) {
        throw std::logic_error("Cannot bind local Unix socket: file name too long.");
    }
    
    memcpy(name.sun_path, _receiver_file_name.c_str(), _receiver_file_name.length());
    name.sun_path[_receiver_file_name.length()] = '\0';
    
    int iresult = connect(m_socket.getFD(), (struct sockaddr*)&name,
                          offsetof(struct sockaddr_un, sun_path) + _receiver_file_name.length()
                              + 1);
    
    if (iresult != 0) {
        if (errno == ENOENT)
            throw ConnectFailedNoSuchFileException("Cannot connect to server (receiver): No such "
                                                   "file or directory.");
        perror("Error: Cannot connect to server (receiver)");
        throw ConnectFailedException("Cannot connect to server (receiver).");
    }
}


UnixSender::~UnixSender() = default;

    
void UnixSender::send(MessageHeader::TypeType _type, std::string const& _data)
{
    sendToSocket(m_socket, _type, _data);
}


} // namespace codingtag
