#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace z_dht::bencode {

/**
 * Bencode 数据类型，支持字符串、整数、列表和字典四种 bencode 基本类型。
 *
 * Value 内部维护两个字段：
 *   - data_      : variant 存储原始数据
 *   - encode_str_: 构造时即完成编码，避免重复计算
 *
 * 构造完成后立即完成 bencode 编码，encode_str() 可直接返回编码结果。
 */
class Value {
public:
    using Integer = std::int64_t;
    using String = std::string;
    using List = std::vector<Value>;
    using Dictionary = std::map<String, Value>;

    Value() = default;

    /**
     * 从原始数据构造 Value，同时计算出 bencode 编码字符串。
     * @param input  variant 类型的数据，支持 Integer / String / List / Dictionary
     */
    explicit Value(const std::variant<Integer, String, List, Dictionary> &input);

    /**
     * @return 该 Value 对应的 bencode 编码字符串（构造时已缓存）
     */
    std::string encode_str() const {
        return encode_str_;
    }

    /**
     * @return 存储的原始数据 variant
     */
    std::variant<Integer, String, List, Dictionary> data() const {
        return data_;
    }

private:
    std::variant<Integer, String, List, Dictionary> data_;
    std::string encode_str_;
};

/**
 * 将 Value 以可读格式输出到流（非 bencode 格式，适合调试输出）。
 * 字符串带双引号、列表用 []、字典用 {} 换行缩进。
 */
std::ostream &operator<<(std::ostream &os, const Value &v);

} // namespace z_dht::bencode
