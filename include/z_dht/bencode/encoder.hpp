#pragma once

#include "z_dht/bencode/value.hpp"

#include <string>
#include <vector>

namespace z_dht::bencode {
    /**
     * 从 bencode 编码字符串中解析出所有顶层的 Value。
     * 输入可以包含多个连续 bencode 值（如 "i1e2:ab" 会解析出两个 Value）。
     *
     * @param input  bencode 编码字符串
     * @return       解析出的 Value 列表
     * @throws       std::runtime_error / std::out_of_range 输入格式错误时抛出
     *
     * 使用示例：
     * @code
     *   auto vals = dencode("i42e4:spam");
     *   // vals[0] -> Integer(42)
     *   // vals[1] -> String("spam")
     * @endcode
     */
    std::vector<Value> decode(const std::string &input);

    /**
     * 从 bencode 编码字符串中解析出第一个顶层 Value，忽略后续多余数据。
     * 内部由 dencode() 调用，也适合单独使用。
     *
     * @param input  bencode 编码字符串
     * @return       解析出的单个 Value
     */
    Value decodeOne(const std::string &input);


    std::string encodePOne(const std::variant<Value::String, Value::Integer, Value::List, Value::Dictionary> &value);


} // namespace z_dht::bencode
