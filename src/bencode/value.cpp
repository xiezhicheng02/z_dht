#include "z_dht/bencode/value.hpp"

#include <utility>

namespace z_dht::bencode {
    Value::Value(Integer value) : data_(value), type_(Type::Integer) {
    }

    Value::Value(String value) : data_(std::move(value)), type_(Type::String) {
    }

    Value::Value(const char *value) : data_(String{value}), type_(Type::String) {
    }

    Value::Value(List value) : data_(std::move(value)), type_(Type::List) {
    }

    Value::Value(Dictionary value) : data_(std::move(value)), type_(Type::Dictionary) {
    }


    string Value::encode() const {
        if (type_ == Type::Integer) {
            // 使用 std::get 安全地从 variant 中提取 Integer
            std::string s = std::to_string(std::get<Integer>(data_));
            return "i" + s + "e";
        }
        if (type_ == Type::String) {
            // 使用 std::get 安全地从 variant 中提取 String
            const auto &s = std::get<String>(data_);
            // 必须将长度先转为 string，否则会导致指针运算错误
            return std::to_string(s.length()) + ":" + s;
        }

        if (type_ == Type::List) {
            const auto list = get<List>(data_);
            String s = "l";
            for (const auto &value: list)
                s = s + value.encode();
            return s + "e";
        }

        if (type_ == Type::Dictionary) {
            const auto dict = get<Dictionary>(data_);
            String s = "d";
            for (const auto &[fst, snd]: dict) {
                String key = std::to_string(fst.length()) + ":" + fst;
                s = s + key + snd.encode();
            }
            return s + "e";
        }
        return "";
    }

    Type Value::getType() const {
        return type_;
    }

} // namespace z_dht::bencode
