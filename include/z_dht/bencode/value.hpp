#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace z_dht::bencode {
    using namespace std;

    enum class Type {
        String, Integer, List, Dictionary
    };

    class Value {
    public:
        using Integer = std::int64_t;
        using String = std::string;
        using List = std::vector<Value>;
        using Dictionary = std::map<String, Value>;

    private:
        std::variant<Integer, String, List, Dictionary> data_ = String{};
        Type type_;

    public:
        [[nodiscard]] Type getType() const;

        string encode() const;

        explicit Value(Integer value);

        explicit Value(String value);

        explicit Value(const char *value);

        explicit Value(List value);

        explicit Value(Dictionary value);
    };
} // namespace z_dht::bencode
