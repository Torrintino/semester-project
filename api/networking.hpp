// api/networking.hpp

#ifndef CODINGTAG_HARDWARE_API_NETWORKING_HPP_HEADER_GUARD_
#define CODINGTAG_HARDWARE_API_NETWORKING_HPP_HEADER_GUARD_ 1

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>

#include <sys/socket.h>


namespace codingtag {

#define DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(super, x) \
    class x : public super \
    { \
    public: \
        x(std::string const& _msg) : super(_msg) { } \
        virtual ~x() = default; \
    };

DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, AcceptFailedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, BindFailedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, ConnectFailedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(ConnectFailedException, ConnectFailedNoSuchFileException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(ConnectFailedException, ConnectFailedConnectionRefusedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, ReceivedUnexpectedMessageException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, RecvFailedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, SockFailedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, SendFailedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(SendFailedException, SendFailedConnectionError)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(SendFailedConnectionError, SendFailedConnectionRefusedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(SendFailedConnectionError, SendFailedNotConnectedException)
DEFINE_SIMPLE_DERIVATE_OF_EXCEPTION(std::runtime_error, UnlinkFailedException)


class CSocketWrapper final // to make RAII work (preventig memory leaks)
{
public:
    CSocketWrapper(int _namespace, int _style, int _protocol);
    CSocketWrapper(CSocketWrapper const& _other) = delete;
    CSocketWrapper(CSocketWrapper&& _other);
    CSocketWrapper& operator=(CSocketWrapper const& _other) & = delete;
    CSocketWrapper& operator=(CSocketWrapper&& _other) &;
    ~CSocketWrapper();
    
    int getFD() const { return m_sockfd; }
    CSocketWrapper accept(struct sockaddr* _addr, socklen_t* _length_ptr);
    void shutdown(int _how) const noexcept;
    
    bool operator<(CSocketWrapper const& _other) const { return m_sockfd < _other.m_sockfd; }
    
private:
    CSocketWrapper(int _sockfd) : m_sockfd(_sockfd) { }
    int m_sockfd;
};


// deletes file _file_name if it gets destructed.
class FileDeleter final
// to make RAII work (preventig open files in case of an exception)
{
public:
    FileDeleter(std::string const& _file_name);
    FileDeleter(FileDeleter const& _other) = delete;
    FileDeleter(FileDeleter&& _other) = delete;
    FileDeleter& operator=(FileDeleter const& _other) & = delete;
    FileDeleter& operator=(FileDeleter&& _other) & = delete;
    ~FileDeleter();
    
private:
    std::string const m_file_name;
};


struct MessageHeader
{
    typedef uint32_t TypeType;
    
    uint32_t size;
    TypeType type; 
};


class UnixReceiver final
{
public:
    UnixReceiver(std::string const& _file_name);
    UnixReceiver(UnixReceiver const& _other) = delete;
    UnixReceiver(UnixReceiver&& _other) = delete;
    UnixReceiver& operator=(UnixReceiver const& _other) & = delete;
    UnixReceiver& operator=(UnixReceiver&& _other) & = delete;
    ~UnixReceiver();
    
    void receive(std::function<void(MessageHeader::TypeType, std::string)> _func);
    
private:
    FileDeleter const m_socket_file_deleter;
    CSocketWrapper m_socket;
    MessageHeader m_buffered_header;
    bool m_buffered_header_available;
};


class UnixSender final
{
public:
    UnixSender(std::string const& _file_name, std::string const& _receiver_file_name);
    UnixSender(UnixSender const& _other) = delete;
    UnixSender(UnixSender&& _other) = delete;
    UnixSender& operator=(UnixSender const& _other) & = delete;
    UnixSender& operator=(UnixSender&& _other) & = delete;
    ~UnixSender();
    
    void send(MessageHeader::TypeType _type, std::string const& _data);
    
private:
    FileDeleter const m_socket_file_deleter;
    CSocketWrapper m_socket;
};


} // namespace codingtag


#endif // CODINGTAG_HARDWARE_API_NETWORKING_HPP_HEADER_GUARD_
