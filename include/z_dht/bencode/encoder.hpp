#pragma once

#include "z_dht/bencode/value.hpp"

#include <string>

namespace z_dht::bencode {
    std::vector<Value> dencode(const std::string &input);
} // namespace z_dht::bencode
