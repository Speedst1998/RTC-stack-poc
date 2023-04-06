#pragma once

#include <RtpTransport/Transport.h>

class SimpleUdpTransportImpl {
    bool SendRtp(const std::vector<uint8_t>&& data) override;

    bool SendRtcp(const std::vector<uint8_t>&& data) override;

    std::error_code ListenOn(uint16_t port);
};
