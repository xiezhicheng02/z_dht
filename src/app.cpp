#include "z_dht/app.hpp"

#include "z_dht/bencode/encoder.hpp"
#include "z_dht/bencode/value.hpp"

#include <iostream>

namespace z_dht {
    int App::run() {
        using namespace z_dht::bencode;

        auto str = "d3:keyl13:xzc2:is4:goodi19584678752eee";
        auto list = dencode(str);
        for (auto &value: list) {
            std::cout << value << std::endl;
        }

        auto v= Value({"string","12"})      ;
        std::cout << v << std::endl;
        return 0;
    }
} // namespace z_dht
