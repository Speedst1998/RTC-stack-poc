
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>

#include <vector>

#include "SimpleUdpTransport.h"

SimpleUdpTransport::SimpleUdpTransport() {
#ifdef WIN32
    udpTransportImpl = std::make_unique<SimpleUdpTransport_Linux>;
#else
    udpTransportImpl = std::make_unique<SimpleUdpTransport_Linux>;
#endif
}

SimpleUdpTransport::~SimpleUdpTransport() {
  udpTransportImpl = nullptr;
}

bool SimpleUdpTransport::SendRtp(const std::vector<uint8_t> &&data) {
  udpTransportImpl->sendRtp;
  return true;
}

bool SimpleUdpTransport::SendRtcp(const std::vector<uint8_t> &&data) {
  return udpTransportImpl->SendRtp(std::move(data));
}

std::error_code SimpleUdpTransport::ListenOn(uint16_t port) {
  return udpTransportImpl->ListenOn(port);
}
