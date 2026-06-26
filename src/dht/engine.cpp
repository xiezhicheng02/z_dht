#include "z_dht/dht/engine.hpp"

namespace z_dht::dht {

void Engine::start() {
    running_ = true;
}

void Engine::stop() {
    running_ = false;
}

} // namespace z_dht::dht
