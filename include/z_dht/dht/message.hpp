#pragma once

#include <string>

namespace z_dht::dht {

struct Message {
    std::string transaction_id;
    std::string method;
};

} // namespace z_dht::dht
