#pragma once

#include <memory>

#ifdef WIN32

#else
#include <thread>
#include <sys/socket.h>
#endif

#include <RtpTransport/Transport.h>

class SimpleUdpTransportImpl;

class SimpleUdpTransport : public rtp::transport::Transport {
public:
    SimpleUdpTransport();

    bool SendRtp(const std::vector<uint8_t>&& data) override;
    bool SendRtcp(const std::vector<uint8_t>&& data) override;
    std::error_code ListenOn(uint16_t port);
    void RtpReceiveCallback(OnPacketCallback callback); 
    void RtcpReceiveCallback(OnPacketCallback callback);
    void StateChangedCallback(OnTransportStateChanged callback); 
    const rtp::transport::State CurrentState() const;
    virtual ~SimpleUdpTransport();
private:
    SimpleUdpTransportImpl* udpTransportImpl;
};


