#pragma once

#include "z_dht/dht/node.hpp"

namespace z_dht::dht {

class Engine {
public:
    void start();
    void stop();

private:
    bool running_ = false;
};

} // namespace z_dht::dht
