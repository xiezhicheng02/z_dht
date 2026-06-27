#include "z_dht/bencode/encoder.hpp"

#include <string>
#include <stack>
#include <stdexcept>
/**
* Bencode（B编码）是 BitTorrent (BT) 协议中使用的一种轻量级、高效的数据序列化格式，专门用于在网络中传输和存储结构化数据（如 .torrent 种子文件）。
它的设计哲学是“简洁即真理”，没有像 JSON 或 XML 那样复杂的语法，而是通过单个前缀字符来标识数据类型，具有无歧义、确定性和紧凑的特点。Bencode 仅支持四种基本数据类型：
1. 字符串（Bytes/String）
字符串的编码规则是：先写字符串的长度，然后是一个冒号 :，最后跟上实际的字符串内容。
格式：<长度>:<内容>
示例：4:spam 表示字符串 "spam"；6:python 表示字符串 "python"。
2. 整数（Integer）
整数以 i 开头，后跟十进制数字，最后以 e 结束。
格式：i<数字>e
示例：i42e 表示整数 42；i-3e 表示整数 -3；i0e 表示 0。注意，像 i-0e 或 i03e（前导零）这样的格式是无效的。
3. 列表（List）
列表以 l 开头，包含多个 Bencode 编码的元素，最后以 e 结束。列表中的元素可以是任意合法的 Bencode 类型，支持无限嵌套。
格式：l<元素1><元素2>...e
示例：l4:spam4:eggse 表示列表 ["spam", "eggs"]；l4:spami123ee 表示混合类型列表 ["spam", 123]。
4. 字典（Dictionary）
字典以 d 开头，包含键值对，最后以 e 结束。字典的键必须是字符串，且必须严格按照字典序（字母顺序）升序排列，值可以是任意合法的 Bencode 类型。
格式：d<键1><值1><键2><值2>...e
示例：d3:foo3:bar4:spami42ee 表示字典 {"foo": "bar", "spam": 42}。
核心设计特点
紧凑性：去除了所有空格、换行、引号和逗号等冗余符号，非常适合低带宽的 P2P 网络环境。
确定性（Deterministic Encoding）：由于强制规定了字典键的排序规则，无论使用何种编程语言或库生成，相同内容的字典编码后的字节流是完全一致的。这一特性对于 BitTorrent 协议至关重要，因为客户端需要对 info 字典进行 SHA-1 哈希计算以生成唯一的 infohash（磁力链接的核心标识）。
 */
namespace z_dht::bencode {



    std::vector<Value> dencode(const std::string &input) {
        if (input.empty()) {
            throw std::runtime_error("Cannot decode an empty Bencode string");
        }

        std::vector<Value> result;
        int len = 0;
        while (true) {
            auto v = Value(input);
            result.push_back(v);
            //判断字符串是否解析完成
            len += v.encode_str().length();
            if (len >= input.length()) {
                break;
            }
        }
        return result;
    }
} // namespace z_dht::bencode
