#include "z_dht/bencode/value.hpp"

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace z_dht::bencode {
    Value::Value(const std::variant<Integer, String, List, Dict> &input) {
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
        } else if (std::holds_alternative<Dict>(input)) {
            this->data_ = input;
            auto str = std::get<Dict>(input);
            std::string s = "d";
            for (auto &[k, v]: str) {
                s = s + std::to_string(k.length()) + ":" + k + v.encode_str();
            }
            this->encode_str_ = s + "e";
        } else {
            throw std::out_of_range("不支持的 bencode 数据类型");
        }
    }

    std::ostream &operator<<(std::ostream &os, const Value &v) {
        auto data = v.data();
        if (std::holds_alternative<Value::String>(data)) {
            os << "\"" << std::get<Value::String>(data) << "\"";
        } else if (std::holds_alternative<Value::Integer>(data)) {
            os << std::get<Value::Integer>(data);
        } else if (std::holds_alternative<Value::List>(data)) {
            auto list = std::get<Value::List>(data);
            os << "[";
            for (std::size_t i = 0; i < list.size(); i++) {
                if (i > 0) {
                    os << ", ";
                }
                os << list[i];
            }
            os << "]";
        } else if (std::holds_alternative<Value::Dict>(data)) {
            auto dict = std::get<Value::Dict>(data);
            os << "{" << std::endl;
            std::size_t i = 0;
            for (auto &value: dict) {
                i++;
                os << "  \"" << value.first << "\": " << value.second;
                if (i < dict.size()) {
                    os << "," << std::endl;
                } else {
                    os << std::endl;
                }
            }
            os << "}";
        } else {
            throw std::runtime_error("不支持的 value 类型");
        }
        return os;
    }
} // namespace z_dht::bencode
