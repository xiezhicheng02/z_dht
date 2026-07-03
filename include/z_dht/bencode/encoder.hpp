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
     * @throws       std::runtime_error  空输入
     * @throws       std::out_of_range   格式错误（缺少结束符、冒号等）
     *
     * 使用示例：
     * @code
     *   auto vals = decode("i42e4:spam");
     *   // vals[0] -> Integer(42), 编码 "i42e"
     *   // vals[1] -> String("spam"), 编码 "4:spam"
     * @endcode
     */
    std::vector<Value> decode(const std::string &input);

    /**
     * 从 bencode 编码字符串中解析出第一个顶层 Value，忽略后续多余数据。
     * 内部由 decode() 调用，也适合单独使用。
     *
     * @param input  bencode 编码字符串
     * @return       解析出的单个 Value
     * @throws       std::out_of_range   格式错误时抛出
     */
    Value decodeOne(const std::string &input);

    /**
     * 从 bencode 编码字符串中解析出第一个顶层 Value，忽略后续多余数据。
     * 内部由 decode() 调用，也适合单独使用。
     *
     * @param input  bencode 编码字符串
     * @return       解析出的单个 Value
     * @throws       std::out_of_range   格式错误时抛出
     */
    Value decodeOne(std::istream &in);

    /**
     * 将原始数据直接编码为 bencode 字符串，免去构造 Value 对象的步骤。
     * 内部通过构造临时 Value 对象完成编码。
     *
     * @param value  需要编码的原始数据，支持 String / Integer / List / Dictionary
     * @return       bencode 编码后的字符串
     *
     * 使用示例：
     * @code
     *   auto s = encodeOne(Value::String("hello"));   // "5:hello"
     *   auto n = encodeOne(Value::Integer(42));       // "i42e"
     * @endcode
     */
    std::string encodeOne(const std::variant<Value::String, Value::Integer, Value::List, Value::Dict> &value);
} // namespace z_dht::bencode
