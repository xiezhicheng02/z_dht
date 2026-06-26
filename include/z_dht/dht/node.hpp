#pragma once

#include <array>
#include <cstdint>

namespace z_dht::dht {

using NodeId = std::array<std::uint8_t, 20>;

struct Node {
    NodeId id{};
    std::uint32_t ipv4 = 0;
    std::uint16_t port = 0;
};

} // namespace z_dht::dht
