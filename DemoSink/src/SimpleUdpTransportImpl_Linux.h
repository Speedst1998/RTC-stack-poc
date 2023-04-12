#pragma once

#include <RtpTransport/Transport.h>

class SimpleUdpTransport_Linux : public SimpleUdpTransportImpl {
public:
    SimpleUdpTransport_Linux();

    bool SendRtp(const std::vector<uint8_t>&& data) override;

    bool SendRtcp(const std::vector<uint8_t>&& data) override;

    std::error_code ListenOn(uint16_t port);

    virtual ~SimpleUdpTransport_Linux();

private:
    std::thread recv_thread;
    int fd = -1;

    void SocketThread();

    void ConnectTo(const sockaddr_storage &addr, const socklen_t addr_len);
};


