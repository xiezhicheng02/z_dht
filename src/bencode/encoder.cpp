#include "z_dht/bencode/encoder.hpp"

#include <cctype>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace z_dht::bencode {
    /**
     * 从 bencode 编码字符串中解析出所有顶层 Value。
     *
     * Bencode 规范（BitTorrent）：
     *   - 字符串：<长度>:<内容>，如 "4:spam"
     *   - 整数：  i<数字>e，如 "i42e"
     *   - 列表：  l<元素...>e，如 "l4:spami42ee"
     *   - 字典：  d<键><值>...e，键必须是字符串且按字典序排列
     */
    std::vector<Value> decode(const std::string &input) {
        if (input.empty()) {
            throw std::runtime_error("无法解析空的 bencode 字符串");
        }

        std::vector<Value> result;
        std::size_t len = 0;
        while (true) {
            auto v = decodeOne(input.substr(len));
            result.push_back(v);
            len += v.encode_str().length();
            if (len >= input.length()) {
                break;
            }
        }
        return result;
    }

    /**
     * 从 bencode 字符串的起始位置解析一个完整的 Value，返回该 Value。
     * 解析后可通过 encode_str().length() 获知实际消耗的字节数。
     */
    Value decodeOne(const std::string &input) {
        auto c = input[0];
        if (c == 'i') {
            // --- 整数: i<数字>e ---
            std::size_t p = 1;
            while (true) {
                if (p >= input.length()) {
                    throw std::out_of_range("整数格式错误：缺少结束符 e");
                }
                c = input[p];
                if (c == 'e') {
                    auto str = input.substr(1, p - 1);
                    return Value(std::stoll(str));
                }
                p++;
            }
        } else if (std::isdigit(static_cast<unsigned char>(c))) {
            // --- 字符串: <长度>:<内容> ---
            std::size_t p = 1;
            while (true) {
                if (p >= input.length()) {
                    throw std::out_of_range("字符串格式错误：缺少冒号分隔符");
                }
                c = input[p];
                if (c == ':') {
                    auto header = input.substr(0, p);
                    auto len = std::stoull(header);
                    auto str = input.substr(p + 1, len);
                    return Value(str);
                }
                p++;
            }
        } else if (c == 'l') {
            // --- 列表: l<元素...>e ---
            std::size_t p = 1;
            std::vector<Value> result;
            while (true) {
                if (p >= input.length()) {
                    throw std::out_of_range("列表格式错误：缺少结束符 e");
                }
                c = input[p];
                if (c == 'e') {
                    return Value(result);
                }
                auto v = decodeOne(input.substr(p));
                result.push_back(v);
                p += v.encode_str().length();
            }
        } else if (c == 'd') {
            // --- 字典: d<键><值>...e，键必须是字符串 ---
            std::size_t p = 1;
            std::map<Value::String, Value> result;
            while (true) {
                if (p >= input.length()) {
                    throw std::out_of_range("字典格式错误：缺少结束符 e");
                }
                c = input[p];
                if (c == 'e') {
                    return Value(result);
                }
                auto first = decodeOne(input.substr(p));
                p += first.encode_str().length();
                auto second = decodeOne(input.substr(p));
                p += second.encode_str().length();
                auto key = std::get<Value::String>(first.data());
                result.insert({key, second});
            }
        } else {
            throw std::out_of_range("不支持的 bencode 前缀字符: " + std::string(1, c));
        }
    }


    std::string encodeOne(std::variant<Value::String, Value::Integer, Value::List, Value::Dictionary> input) {
        if (std::holds_alternative<Value::String>(input)) {
            auto data = std::get<Value::String>(input);
            return Value(data).encode_str();
        } else if (std::holds_alternative<Value::Integer>(input)) {
            auto data = std::get<Value::Integer>(input);
            return Value(data).encode_str();
        } else if (std::holds_alternative<Value::List>(input)) {
            auto data = std::get<Value::List>(input);
            return Value(data).encode_str();
        } else if (std::holds_alternative<Value::Dictionary>(input)) {
            auto data = std::get<Value::Dictionary>(input);
            return Value(data).encode_str();
        } else {
            throw std::runtime_error("传入的是无法编码的类型");
        }
    }
}

} // namespace z_dht::bencode
