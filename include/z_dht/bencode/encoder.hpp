#pragma once

#include "z_dht/bencode/value.hpp"

#include <string>

namespace z_dht::bencode {
    /**
     * 编码传入的数据， 进行bencode编码
     * @param value  需要编码的对象
     * @return  返回编码之后的字符串
     */
    const std::string encode(const Value &value);

    /**
     * 进行解码操作
     * @param input  需要解析的字符串
     * @return 返回解码之后的对象
     */
    std::vector<Value> dencode(const std::string &input);
} // namespace z_dht::bencode
