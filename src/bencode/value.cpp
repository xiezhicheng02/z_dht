#include "z_dht/bencode/value.hpp"

#include <stdexcept>
#include <utility>

namespace z_dht::bencode {
    /**
     *
     * @param input  成熟的Value的值的类型
     */
    Value::Value(const std::variant<Integer, String, List, Dictionary> &input) {
        if (std::holds_alternative<String>(input)) {
            this->data_ = input;
            auto str = std::get<String>(input);
            auto len = str.length();
            this->encode_str_ = std::to_string(len) + ":" + str;
        } else if (std::holds_alternative<Integer>(input)) {
            this->data_ = input;
            auto str = std::get<Integer>(input);
            this->encode_str_ = "i" + std::to_string(str) + "e";
        } else if (std::holds_alternative<List>(input)) {
            this->data_ = input;
            auto str = std::get<List>(input);
            std::string s = "l";
            for (auto &v: str) {
                s = s + v.encode_str();
            }
            this->encode_str_ = s + "e";
        } else if (std::holds_alternative<Dictionary>(input)) {
            this->data_ = input;
            auto str = std::get<Dictionary>(input);
            std::string s = "d";
            for (auto &[k,v]: str) {
                s = s + std::to_string(k.length()) + ":" + k + v.encode_str();
            }
            this->encode_str_ = s + "e";
        } else {
            throw std::out_of_range("invalid input, 不支持的转换becode的数据类型");
        }
    }



} // namespace z_dht::bencode
