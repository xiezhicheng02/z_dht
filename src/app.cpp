#include "z_dht/app.hpp"

#include "z_dht/bencode/encoder.hpp"
#include "z_dht/bencode/value.hpp"

#include <iostream>

namespace z_dht {
    int App::run() {
        using namespace z_dht::bencode;

        auto str = "d3:keyi123eei47e";
        auto list = dencode(str);
        for (auto &value: list) {
            std::cout << value << std::endl;
        }

        auto v = Value(126);
        std::cout << v << std::endl;

        auto v1 = Value("126");
        std::cout << v1 << std::endl;
        auto v2 = Value(Value::List{
            Value("aa"),
            Value(11)
        });
        std::cout << v2 << std::endl;

        auto v3 = Value(
            Value::Dictionary{
                {"a", Value(1)},
                {"b", Value(2)}
            }
        );

        std::cout << v3 << std::endl;

        return 0;
    }
} // namespace z_dht
