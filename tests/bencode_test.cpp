#include "z_dht/bencode/encoder.hpp"
#include "z_dht/bencode/value.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

static int total  = 0;
static int passed = 0;

#define TEST(name)                                        \
    do {                                                   \
        total++;                                           \
        try {                                               \
            name();                                         \
            passed++;                                       \
            std::cout << "  PASS  " << #name << "\n";       \
        } catch (const std::exception &e) {                 \
            std::cout << "  FAIL  " << #name                \
                      << " (" << e.what() << ")\n";         \
        }                                                    \
    } while (0)

#define ASSERT_EQ(a, b)                                            \
    do {                                                           \
        if ((a) != (b)) {                                          \
            std::ostringstream _msg;                                \
            _msg << "line " << __LINE__ << ": expected '" << (b)    \
                 << "', got '" << (a) << "'";                       \
            throw std::runtime_error(_msg.str());                  \
        }                                                          \
    } while (0)

#define ASSERT_THROW(expr, exc)                                    \
    do {                                                           \
        bool _caught = false;                                       \
        try { (void)(expr); } catch (const exc &) { _caught = true; } \
        if (!_caught) {                                             \
            throw std::runtime_error(                               \
                "line " + std::to_string(__LINE__) +                \
                ": expected exception " #exc);                      \
        }                                                           \
    } while (0)

using namespace z_dht::bencode;

// ===== 编码测试 =====

static void encode_integer() {
    ASSERT_EQ(Value(0).encode_str(),           "i0e");
    ASSERT_EQ(Value(42).encode_str(),          "i42e");
    ASSERT_EQ(Value(-1).encode_str(),          "i-1e");
    ASSERT_EQ(Value(999999999).encode_str(),   "i999999999e");
}

static void encode_string() {
    ASSERT_EQ(Value("").encode_str(),          "0:");
    ASSERT_EQ(Value("spam").encode_str(),       "4:spam");
    ASSERT_EQ(Value("hello world").encode_str(),"11:hello world");
}

static void encode_list() {
    Value empty(Value::List{});
    ASSERT_EQ(empty.encode_str(), "le");

    Value nums(Value::List{Value(1), Value(2), Value(3)});
    ASSERT_EQ(nums.encode_str(), "li1ei2ei3ee");

    Value mixed(Value::List{Value("a"), Value(1)});
    ASSERT_EQ(mixed.encode_str(), "l1:ai1ee");
}

static void encode_dict() {
    Value empty(Value::Dictionary{});
    ASSERT_EQ(empty.encode_str(), "de");

    Value simple(Value::Dictionary{{"a", Value(1)}});
    ASSERT_EQ(simple.encode_str(), "d1:ai1ee");
}

static void encode_dict_key_order() {
    // std::map 保证字典序，所以 {"b":2, "a":1} 编码为 "a" 在前
    Value v(Value::Dictionary{
        {"b", Value(2)},
        {"a", Value(1)}
    });
    ASSERT_EQ(v.encode_str(), "d1:ai1e1:bi2ee");
}

static void encode_nested() {
    Value v(Value::List{
        Value("top"),
        Value(Value::List{Value(1), Value(2)})
    });
    ASSERT_EQ(v.encode_str(), "l3:topli1ei2eee");
}

// ===== 解码测试 =====

static void decode_integer() {
    Value v = decodeOne("i42e");
    ASSERT_EQ(std::get<Value::Integer>(v.data()), 42);
    ASSERT_EQ(v.encode_str(), "i42e");
}

static void decode_string() {
    Value v = decodeOne("4:spam");
    ASSERT_EQ(std::get<Value::String>(v.data()), "spam");
    ASSERT_EQ(v.encode_str(), "4:spam");
}

static void decode_empty_string() {
    Value v = decodeOne("0:");
    ASSERT_EQ(std::get<Value::String>(v.data()), "");
}

static void decode_list() {
    Value v = decodeOne("li1ei2ei3ee");
    auto list = std::get<Value::List>(v.data());
    ASSERT_EQ(list.size(), 3U);
    ASSERT_EQ(std::get<Value::Integer>(list[0].data()), 1);
    ASSERT_EQ(std::get<Value::Integer>(list[1].data()), 2);
    ASSERT_EQ(std::get<Value::Integer>(list[2].data()), 3);
}

static void decode_dict() {
    Value v = decodeOne("d3:fooi42e3:bar4:spame");
    auto dict = std::get<Value::Dictionary>(v.data());
    ASSERT_EQ(dict.size(), 2U);
    auto foo = dict.find("foo");
    ASSERT_EQ(foo != dict.end(), true);
    ASSERT_EQ(std::get<Value::Integer>(foo->second.data()), 42);
    auto bar = dict.find("bar");
    ASSERT_EQ(bar != dict.end(), true);
    ASSERT_EQ(std::get<Value::String>(bar->second.data()), "spam");
}

static void decode_nested() {
    Value v = decodeOne("d3:keyli1ei2eee");
    auto dict = std::get<Value::Dictionary>(v.data());
    auto it = dict.find("key");
    ASSERT_EQ(it != dict.end(), true);
    auto list = std::get<Value::List>(it->second.data());
    ASSERT_EQ(list.size(), 2U);
}

static void decode_multiple() {
    auto vals = decode("i1e3:abc");
    ASSERT_EQ(vals.size(), 2U);
    ASSERT_EQ(std::get<Value::Integer>(vals[0].data()), 1);
    ASSERT_EQ(std::get<Value::String>(vals[1].data()), "abc");
}

// ===== 边界和错误测试 =====

static void decode_empty_throws() {
    ASSERT_THROW(decode(""), std::runtime_error);
}

static void decode_unterminated_integer() {
    ASSERT_THROW(decodeOne("i42"), std::out_of_range);
}

static void decode_unterminated_list() {
    ASSERT_THROW(decodeOne("li1e"), std::out_of_range);
}

static void decode_unterminated_dict() {
    ASSERT_THROW(decodeOne("d3:fooi1e"), std::out_of_range);
}

static void decode_no_separator() {
    ASSERT_THROW(decodeOne("4abc"), std::out_of_range);
}

static void decode_invalid_prefix() {
    ASSERT_THROW(decodeOne("x"), std::out_of_range);
}

// ===== 调试输出测试 =====

static void print_integer() {
    std::ostringstream os;
    os << Value(42);
    ASSERT_EQ(os.str(), "42");
}

static void print_string() {
    std::ostringstream os;
    os << Value("hello");
    ASSERT_EQ(os.str(), "\"hello\"");
}

static void print_list() {
    std::ostringstream os;
    os << Value(Value::List{Value(1), Value("x")});
    ASSERT_EQ(os.str(), R"([1, "x"])");
}

static void print_dict() {
    std::ostringstream os;
    os << Value(Value::Dictionary{{"k", Value("v")}});
    auto s = os.str();
    ASSERT_EQ(s.find('{'), 0);
    ASSERT_EQ(s.find("k") != std::string::npos, true);
    ASSERT_EQ(s.find("v") != std::string::npos, true);
    ASSERT_EQ(s.find('}') != std::string::npos, true);
}

// ===== main =====

int main() {
    std::cout << "=== bencode 测试 ===\n\n";

    // 编码
    TEST(encode_integer);
    TEST(encode_string);
    TEST(encode_list);
    TEST(encode_dict);
    TEST(encode_dict_key_order);
    TEST(encode_nested);

    // 解码
    TEST(decode_integer);
    TEST(decode_string);
    TEST(decode_empty_string);
    TEST(decode_list);
    TEST(decode_dict);
    TEST(decode_nested);
    TEST(decode_multiple);

    // 边界 / 异常
    TEST(decode_empty_throws);
    TEST(decode_unterminated_integer);
    TEST(decode_unterminated_list);
    TEST(decode_unterminated_dict);
    TEST(decode_no_separator);
    TEST(decode_invalid_prefix);

    // 调试输出
    TEST(print_integer);
    TEST(print_string);
    TEST(print_list);
    TEST(print_dict);

    std::cout << "\n=== 结果: " << passed << "/" << total << " 通过 ===\n";
    return total == passed ? 0 : 1;
}
