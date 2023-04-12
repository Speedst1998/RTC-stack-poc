
#include "SimpleUdpTransportImpl.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#include <thread>

class SimpleUdpTransportImpl_Win32 : public SimpleUdpTransportImpl
{
public:
    SimpleUdpTransportImpl_Win32();
    virtual ~SimpleUdpTransportImpl_Win32();
    bool SendRtp(const std::vector<uint8_t>&& data) override;
    bool SendRtcp(const std::vector<uint8_t>&& data) override;
    std::error_code ListenOn(uint16_t port) override;

private:
    void ConnectTo(const sockaddr_storage& addr, const socklen_t addr_len); 
    SOCKET socket_;
    std::atomic<bool> listening_;
    void SocketThread();
    std::thread listeningThread_;
};