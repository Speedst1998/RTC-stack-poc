#pragma once

#include <thread>
#include <sys/socket.h>

#include <RtpTransport/Transport.h>

class SimpleUdpTransportImpl;

class SimpleUdpTransport : public rtp::transport::Transport {
public:
    SimpleUdpTransport();

    bool SendRtp(const std::vector<uint8_t>&& data) override;

    bool SendRtcp(const std::vector<uint8_t>&& data) override;

    std::error_code ListenOn(uint16_t port);

    virtual ~SimpleUdpTransport();
private:
    std::unique_ptr<rtp::transport::Transport> udpTransportImpl;
};


