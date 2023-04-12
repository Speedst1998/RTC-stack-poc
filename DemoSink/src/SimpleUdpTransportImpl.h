#pragma once

#include <RtpTransport/Transport.h>


        class SimpleUdpTransportImpl : public rtp::transport::Transport {
        public:
            bool SendRtp(const std::vector<uint8_t>&& data) = 0;
            bool SendRtcp(const std::vector<uint8_t>&& data) = 0;
            virtual std::error_code ListenOn(uint16_t port) = 0;
        };
