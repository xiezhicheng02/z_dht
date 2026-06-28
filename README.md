# z-dht

A small C++20 learning project for implementing the BitTorrent DHT protocol with coroutines and UDP.

## Layout

- `include/z_dht/coro`: coroutine primitives
- `include/z_dht/net`: UDP transport interfaces
- `include/z_dht/dht`: DHT domain types and engine
- `include/z_dht/bencode`: Bencode value type and encode/decode API
- `src/bencode`: bencode implementation
- `tests/bencode_test.cpp`: bencode unit tests
- `src`: executable and implementation files
- `docs/bencode.md`: detailed bencode module documentation

## Build

```sh
cmake -S . -B build
cmake --build build
./build/z_dht        # run the app
./build/z_dht_test   # run bencode tests
```

## Bencode 模块

Bencode 是 BitTorrent 协议使用的数据序列化格式，支持四种基本类型：字符串、整数、列表、字典。

### 快速示例

```cpp
#include "z_dht/bencode/encoder.hpp"
#include "z_dht/bencode/value.hpp"
using namespace z_dht::bencode;

// --- 编码 ---
Value v(Value::Dictionary{
    {"t", Value("aa")},
    {"q", Value("ping")},
    {"y", Value("q")},
});
// v.encode_str() => "d1:q4:ping1:t2:aa1:y1:qe"

// --- 解码 ---
auto vals = decode("i42e");
// vals[0] => Integer(42)

// --- 调试输出 ---
std::cout << v << std::endl;
// 输出格式化字典
```

### 头文件

| 头文件                                   | 内容               |
|------------------------------------------|--------------------|
| `include/z_dht/bencode/value.hpp`        | Value 类型定义      |
| `include/z_dht/bencode/encoder.hpp`      | decode / encodeOne |

### API 速查

| 方法 / 函数                          | 说明                      |
|--------------------------------------|---------------------------|
| `Value(Integer/String/List/Dict)`    | 构造 Value，同时编码       |
| `.encode_str()`                      | 返回 bencode 编码字符串    |
| `.data()`                            | 返回原始 variant 数据      |
| `decode(input)`                      | 解析多值，返回 vector      |
| `decodeOne(input)`                   | 解析单个值                 |
| `encodeOne(variant)`                 | 原始数据 → bencode 字符串  |
| `operator<<(os, value)`              | 格式化调试输出             |

详细文档见 [docs/bencode.md](docs/bencode.md)。

## Suggested Milestones

1. Replace the UDP stub with a real non-blocking socket.
2. Add a small event loop that resumes coroutine tasks on socket readiness.
3. Implement bencode encode/decode for DHT messages.
4. Add `ping`, `find_node`, `get_peers`, and `announce_peer` message handling.
5. Add routing table buckets and bootstrap node discovery.
