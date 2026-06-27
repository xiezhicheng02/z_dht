#include "z_dht/bencode/value.hpp"

#include <stdexcept>
#include <utility>

namespace z_dht::bencode {
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
                s = s + k + v.encode_str();
            }
            this->encode_str_ = s + "e";
        } else {
            throw std::out_of_range("invalid input, 不支持的转换becode的数据类型");
        }
    }

    /**
     * 进行字符串的解析
     * @param value  需要解析的字符串， 只会解析出来一个Value
     */
    Value::Value(const std::string &input) {
        auto c = input[0];
        if (c == 'i') {
            //数字类型
            int p = 1;
            while (true) {
                //判断取值位置是否超出字符串的结尾，
                if (p > input.length()) {
                    throw std::out_of_range("invalid input, 数字类型无法解析，没有找到结束符号");
                }
                //获取当前位置的字符
                c = input[p];
                if (c == 'e') {
                    //找到结束符号， 提取出来内容
                    auto str = input.substr(1, p - 1);
                    this->data_ = std::stoll(str);
                    this->encode_str_ = input.substr(0, p + 1);
                    return;
                } else {
                    //当前位置不是结束符号， 继续
                    p++;
                }
            }
        } else if (isdigit(c)) {
            //字符串类型
            int p = 1;
            while (true) {
                //判断取值位置是否超出字符串的结尾，
                if (p > input.length()) {
                    throw std::out_of_range("invalid input, 数字类型无法解析，没有找到结束符号");
                }
                //获取当前位置的字符
                c = input[p];
                if (c == ':') {
                    // 提取出来内容
                    auto header = input.substr(0, p);
                    auto len = std::stoll(header);
                    auto str = input.substr(p + 1, len);
                    this->data_ = str;
                    this->encode_str_ = input.substr(0, header.length() + len + 1);
                    return;
                } else {
                    //当前位置不是结束符号， 继续
                    p++;
                }
            }
        } else if (c == 'l') {
            //List类型
            int p = 1;
            std::vector<Value> result;
            while (true) {
                //判断取值位置是否超出字符串的结尾，
                if (p >= input.length()) {
                    throw std::out_of_range("invalid input, 数字类型无法解析，没有找到结束符号");
                }

                //获取当前位置的字符
                c = input[p];
                if (c == 'e') {
                    //找到结束符号， 提取出来内容
                    this->data_ = result;
                    this->encode_str_ = input.substr(0, p + 1);
                    return;
                } else {
                    auto v = Value(input.substr(p));
                    result.push_back(v);
                    p = p + v.encode_str().length();
                }
            }
        } else if (c == 'd') {
            //字典类型
            int p = 1;
            std::map<String, Value> result;
            while (true) {
                //判断取值位置是否超出字符串的结尾，
                if (p > input.length()) {
                    throw std::out_of_range("invalid input, 数字类型无法解析，没有找到结束符号");
                }
                //获取当前位置的字符
                c = input[p];
                if (c == 'e') {
                    //找到结束符号， 提取出来内容
                    this->data_ = result;
                    this->encode_str_ = input.substr(0, p + 1);
                    return;
                } else {
                    auto first = Value(input.substr(p));
                    p = p + first.encode_str().length();
                    auto second = Value(input.substr(p));
                    p = p + second.encode_str().length();
                    auto key = std::get<String>(first.data());
                    result.insert({key, second});
                }
            }
        } else {
            throw std::out_of_range("invalid input, 不符合要求的字符串，无法解析");
        }
    }
} // namespace z_dht::bencode
