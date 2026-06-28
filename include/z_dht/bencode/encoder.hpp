#pragma once

#include "z_dht/bencode/value.hpp"

#include <string>

namespace z_dht::bencode {
    std::vector<Value> dencode(const std::string &input);

    /**
     * 从给定的字符串中解析出来一个Value
     * @param input 需要解析的字符串
     * @return 解析出来的一个value值
     */
    Value dencodeOne(const std::string &input);
} // namespace z_dht::bencode
