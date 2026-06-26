#include "z_dht/net/udp_socket.hpp"

namespace z_dht::net {

bool UdpSocket::open() {
    opened_ = true;
    return opened_;
}

bool UdpSocket::bind(std::uint16_t port) {
    return opened_ && port != 0;
}

bool UdpSocket::send_to(const std::string& host, std::uint16_t port, const void* data, std::size_t size) {
    return opened_ && !host.empty() && port != 0 && data != nullptr && size > 0;
}

void UdpSocket::close() {
    opened_ = false;
}

} // namespace z_dht::net
