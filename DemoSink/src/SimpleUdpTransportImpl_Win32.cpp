
#include "SimpleUdpTransportImpl_Win32.h"
#include <iostream>
#include <sstream>
#define WIN32_LEAN_AND_MEAN

#include "winsock2.h"
#include "ws2tcpip.h"
#include <algorithm>
#pragma comment(lib, "Ws2_32.lib")

SimpleUdpTransportImpl_Win32::SimpleUdpTransportImpl_Win32() : socket_(INVALID_SOCKET), listening_(false) {
	std::cout << "SimpleUdpTransportImpl_Win32() Startup " << std::endl;
	//Startup WSA Sockets
	int iResult;
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << " WSAStartup failed " << iResult << std::endl;
		return;
	}

}

SimpleUdpTransportImpl_Win32::~SimpleUdpTransportImpl_Win32()
{
	closesocket(socket_);
	listening_ = false;
	if (listeningThread_.joinable()) {
		listeningThread_.join();
	}
	WSACleanup();
}

bool SimpleUdpTransportImpl_Win32::SendRtp(const std::vector<uint8_t>&& data) {
	std::cout << "Win32 SendRtp " << std::endl;
	std::vector<char> charData;
	std::transform(data.begin(), data.end(), std::back_inserter(charData), [](uint8_t i) { return static_cast<char>(i); });
	const char* p = charData.data();
	size_t len = charData.size();
	while (len > 0) {
		auto sent = send(socket_, p, len, 0);
		p += sent;
		len -= sent;
	}
	return true;
}

bool SimpleUdpTransportImpl_Win32::SendRtcp(const std::vector<uint8_t>&& data) {
	return SendRtp(std::move(data));
}

std::error_code SimpleUdpTransportImpl_Win32::ListenOn(uint16_t port) {
	std::cout << "Win32 ListenOn(" << port << ")" << " to be implemented " << std::endl;
	//setup socket
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	//hints.ai_protocol = SOCK_STREAM; // IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;
	std::ostringstream ss;
	ss << port << std::ends;
	int iResult = getaddrinfo(NULL, "27015", &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed: " << iResult << std::endl;
		WSACleanup();
		return std::error_code(1, std::system_category());
	}
	socket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (socket_ == INVALID_SOCKET) {
		std::cout << "Error at socket : " << WSAGetLastError() << std::endl;
		WSACleanup();
		return std::error_code(1, std::system_category());
	}
	struct sockaddr_in serverAddr;
	short myport = 27015;

	// Bind the socket to any address and the specified port.
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(myport);
	// OR, you can do serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(socket_, (SOCKADDR*)&serverAddr, sizeof(serverAddr))) {
		printf("bind failed with error %d\n", WSAGetLastError());
		return std::error_code(1, std::system_category());
	}
	auto lastError = WSAGetLastError();

	listening_ = false;
	if (listeningThread_.joinable()) {
		listeningThread_.join();
	}
	listeningThread_ = std::thread(&SimpleUdpTransportImpl_Win32::SocketThread, this);

	return std::error_code();
}

void SimpleUdpTransportImpl_Win32::SocketThread(){
	
	listening_ = true;

	while (listening_) {
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		fd_set set;
		FD_ZERO(&set);
		FD_SET(socket_, &set);

		auto r = ::select(socket_ + 1, &set, nullptr, nullptr, &tv);
		if (r == -1) {
			ReportError(std::error_code(errno, std::system_category()));
			continue;
		}
		if (!FD_ISSET(socket_, &set)) {
			continue;
		}
		u_long bytes_available;
		if (ioctlsocket(socket_, FIONREAD, &bytes_available) == -1) {
			ReportError(std::error_code(errno, std::system_category()));
			continue;
		}

		if (bytes_available == 0) {
			SetNewState(rtp::transport::State::kTransportClosed);
			continue;
		}

		struct sockaddr_storage addr;
		socklen_t addrlen = sizeof(addr);
		std::vector<char> data(bytes_available);
		auto bytes_read = ::recvfrom(socket_, data.data(), bytes_available, 0, (sockaddr*)&addr, &addrlen);

		switch (bytes_read) {
		case -1:ReportError(std::error_code(errno, std::system_category()));
		case 0: SetNewState(rtp::transport::State::kTransportClosed);
			break;
		default:
			if (CurrentState() != rtp::transport::State::kTransportReady) {
				ConnectTo(addr, addrlen);
			}

			data.resize(bytes_read);
			std::vector<uint8_t> integerData;
			std::transform(data.begin(), data.end(), std::back_inserter(integerData), [](const char c) { return static_cast<uint8_t>(c); });
			OnData(std::move(integerData));
		}
	}
}

void SimpleUdpTransportImpl_Win32::ConnectTo(const sockaddr_storage& addr, const socklen_t addr_len)
{
	if (connect(socket_, (const sockaddr*)&addr, addr_len) == -1) {
		ReportError(std::error_code(errno, std::system_category()));
		return;
	}
	SetNewState(rtp::transport::State::kTransportReady);
}
