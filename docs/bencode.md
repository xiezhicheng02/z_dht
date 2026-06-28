# Bencode 模块

Bencode（B encoding）是 BitTorrent 协议使用的数据序列化格式，支持四种基本类型：
字符串、整数、列表、字典。

本模块提供编码（构造时完成）和解码功能。

---

## 类型定义

| 类型       | C++ 类型                           | Bencode 格式            |
|------------|-------------------------------------|--------------------------|
| 字符串     | `Value::String` (`std::string`)     | `<长度>:<内容>`          |
| 整数       | `Value::Integer` (`std::int64_t`)   | `i<数字>e`               |
| 列表       | `Value::List` (`std::vector<Value>`) | `l<元素...>e`            |
| 字典       | `Value::Dictionary` (`std::map<String, Value>`) | `d<键><值>...e` |

## 头文件

- `include/z_dht/bencode/value.hpp` — Value 类型定义
- `include/z_dht/bencode/encoder.hpp` — 解码接口

---

## 使用示例

### 1. 构造 Value 并编码

```cpp
#include "z_dht/bencode/value.hpp"
using namespace z_dht::bencode;

// 整数
Value v1(42);
std::cout << v1.encode_str() << std::endl;  // 输出: i42e

// 字符串
Value v2("hello");
std::cout << v2.encode_str() << std::endl;  // 输出: 5:hello

// 列表
Value v3(Value::List{Value(1), Value("a")});
std::cout << v3.encode_str() << std::endl;  // 输出: li1e1:ae

// 字典（键自动按字典序排序）
Value v4(Value::Dictionary{
    {"b", Value(2)},
    {"a", Value(1)}
});
std::cout << v4.encode_str() << std::endl;  // 输出: d1:ai1e1:bi2ee
```

### 2. 解码（解析 bencode 字符串）

```cpp
#include "z_dht/bencode/encoder.hpp"
using namespace z_dht::bencode;

// 解析单个值
auto v = decodeOne("i42e");
std::cout << v.data() << std::endl;  // 打印 42（整数）

// 解析多个连续的值
auto vals = decode("i1e2:abi42ee");
// vals[0] => 整数 1，  编码 "i1e"
// vals[1] => 列表 [2:ab, i42e]
```

### 3. 快捷编码（免构造 Value）

```cpp
#include "z_dht/bencode/encoder.hpp"
using namespace z_dht::bencode;

// 直接编码原始数据
auto s = encodeOne(Value::String("hello"));  // "5:hello"
auto n = encodeOne(Value::Integer(42));      // "i42e"
auto l = encodeOne(Value::List{Value(1)});   // "li1ee"
```

### 3. 调试输出

```cpp
Value v(Value::Dictionary{
    {"name", Value("z-dht")},
    {"port", Value(6881)}
});
std::cout << v << std::endl;
// 输出:
// {
//   "name": "z-dht",
//   "port": 6881
// }
```

### 4. 嵌套结构

```cpp
Value torrent(Value::Dictionary{
    {"announce", Value("http://tracker.example.com/announce")},
    {"info", Value(Value::Dictionary{
        {"name", Value("ubuntu.iso")},
        {"length", Value(699999744)}
    })}
});
std::cout << torrent.encode_str() << std::endl;
// d8:announce41:http://tracker.example.com/announce4:infod4:lengthi699999744e4:name10:ubuntu.isoee
```

---

## API 参考

### `Value` 类

| 方法                                 | 说明                           |
|--------------------------------------|--------------------------------|
| `Value(Integer)`                     | 构造整数 Value                 |
| `Value(String)`                      | 构造字符串 Value               |
| `Value(List)`                        | 构造列表 Value                 |
| `Value(Dictionary)`                  | 构造字典 Value                 |
| `std::string encode_str() const`     | 返回 bencode 编码后的字符串     |
| `auto data() const`                  | 返回原始 variant 数据           |

### 解码 / 编码函数

| 函数                                                                    | 说明                                   |
|-------------------------------------------------------------------------|----------------------------------------|
| `decode(const std::string&)`                                            | 解析所有顶层 Value，返回 vector         |
| `decodeOne(const std::string&)`                                         | 只解析第一个顶层 Value                  |
| `encodeOne(const std::variant<...>&)`                                   | 将原始数据直接编码为 bencode 字符串      |

---

## 异常

解码时遇到格式错误会抛出以下异常：

| 异常类型           | 触发场景                     |
|--------------------|------------------------------|
| `std::runtime_error` | 空输入                       |
| `std::out_of_range`  | 缺少结束符、冒号、前缀非法等 |
