
#include "SimpleUdpTransport.h"
#include "SimpleUdpTransportImpl.h"
#ifdef WIN32
#include "SimpleUdpTransportImpl_Win32.h"
#else
#include "SimpleUdpTransportImpl_Linux.h"
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#endif

#include <vector>


SimpleUdpTransport::SimpleUdpTransport() {
#ifdef WIN32
    udpTransportImpl = new SimpleUdpTransportImpl_Win32();
#else
    udpTransportImpl = std::make_unique<SimpleUdpTransport_Linux>;
#endif
}

SimpleUdpTransport::~SimpleUdpTransport() {
  udpTransportImpl = nullptr;
}

bool SimpleUdpTransport::SendRtp(const std::vector<uint8_t> &&data) {
  return udpTransportImpl->SendRtp(std::move(data));
  return true;
}

bool SimpleUdpTransport::SendRtcp(const std::vector<uint8_t> &&data) {
  return udpTransportImpl->SendRtp(std::move(data));
}

std::error_code SimpleUdpTransport::ListenOn(uint16_t port) {
  return udpTransportImpl->ListenOn(port);
}

void SimpleUdpTransport::RtpReceiveCallback(OnPacketCallback callback) {
    udpTransportImpl->RtpReceiveCallback(callback);
}
void SimpleUdpTransport::RtcpReceiveCallback(OnPacketCallback callback) {
    udpTransportImpl->RtcpReceiveCallback(callback);
}
void SimpleUdpTransport::StateChangedCallback(OnTransportStateChanged callback) {
    udpTransportImpl->StateChangedCallback(callback);
}

const rtp::transport::State SimpleUdpTransport::CurrentState() const { return udpTransportImpl->CurrentState(); }