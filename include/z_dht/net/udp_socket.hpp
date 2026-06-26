#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace z_dht::net {

class UdpSocket {
public:
    bool open();
    bool bind(std::uint16_t port);
    bool send_to(const std::string& host, std::uint16_t port, const void* data, std::size_t size);
    void close();

private:
    bool opened_ = false;
};

} // namespace z_dht::net
