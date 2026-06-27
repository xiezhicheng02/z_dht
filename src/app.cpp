#include "z_dht/app.hpp"

#include "z_dht/bencode/encoder.hpp"
#include "z_dht/bencode/value.hpp"

#include <iostream>

namespace z_dht {
    int App::run() {
        using namespace z_dht::bencode;
        std::cout << "z_dht scaffold ready\n";
        std::cout << "Next steps: coroutine scheduler, UDP transport, DHT engine\n";
        auto v = Value::Dictionary{{"id", Value("a")}};


        std::cout << encode(Value(v)) << std::endl;
        auto vv = dencode("l2:xzi129ee");
        for (const auto &value: vv) {
            std::cout << encode(value) << std::endl;
        }

        return 0;
    }
} // namespace z_dht
