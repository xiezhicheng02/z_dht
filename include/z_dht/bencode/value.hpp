#pragma once

#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include <iostream>

namespace z_dht::bencode {
    class Value {
    public:
        using Integer = std::int64_t;
        using String = std::string;
        using List = std::vector<Value>;
        using Dictionary = std::map<String, Value>;

    private:
        std::variant<Integer, String, List, Dictionary> data_;
        std::string encode_str_;

    public:
        /**
         * 从传入的字符串中解析出来一个符合要求的Value对象，
         * 注意智慧解析出来一个， 不会多解析， 并且是从字符串的第一个字符开始解析
         * @param input  传入的字符串
         */
        Value(const std::string &input);

        /**
         * 传入的是特定的数据， 需要将其字符串解析出来并保存
         * @param input
         */
        Value(const std::variant<Integer, String, List, Dictionary> &input);

        /**
         *
         * @return 返回解析之后的字符串
         */
        std::string encode_str() const {
            return encode_str_;
        }

        /**
         *
         * @return 返回原始数据类型
         */
        std::variant<Integer, String, List, Dictionary> data() const {
            return data_;
        }
    };

    // 重载 << 运算符，实现自定义打印
    inline std::ostream &operator<<(std::ostream &os, const Value &v) {
        auto data = v.data();
        if (std::holds_alternative<Value::String>(data)) {
            os << "\"" << std::get<Value::String>(data) << "\"";
        } else if (std::holds_alternative<Value::Integer>(data)) {
            os << std::get<Value::Integer>(data);
        } else if (std::holds_alternative<Value::List>(data)) {
            auto list = std::get<Value::List>(data);
            os << "[";
            for (int i = 0; i < list.size(); i++) {
                os << list[i];
                if (i < list.size() - 1) {
                    os << ",";
                }
            }
            os << "]";
        } else if (std::holds_alternative<Value::Dictionary>(data)) {
            auto dict = std::get<Value::Dictionary>(data);
            os << "{" << std::endl;
            int i = 0;
            for (auto &value: dict) {
                i++;
                os << "  \"" << value.first << "\":" << value.second;
                if (i < dict.size()) {
                    os << "," << std::endl;
                } else {
                    os << std::endl;
                }
            }
            os << "}";
        } else {
            throw std::runtime_error("Unsupported value type");
        }

        return os;
    }
} // namespace z_dht::bencode
