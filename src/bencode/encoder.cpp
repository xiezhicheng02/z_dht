#include "z_dht/bencode/encoder.hpp"

#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>

namespace z_dht::bencode {
    namespace {
        class Parser {
        public:
            explicit Parser(const std::string &input) : input_(input) {
            }

            bool done() const {
                return position_ == input_.size();
            }

            Value parse_value() {
                if (done()) {
                    throw std::runtime_error("Unexpected end of bencode input");
                }

                const char current = input_[position_];
                if (current == 'i') {
                    return parse_integer();
                }
                if (current == 'l') {
                    return parse_list();
                }
                if (current == 'd') {
                    return parse_dictionary();
                }
                if (std::isdigit(static_cast<unsigned char>(current))) {
                    return Value{parse_string_text()};
                }

                throw std::runtime_error("Invalid bencode value prefix");
            }

        private:
            Value parse_integer() {
                ++position_;
                const auto begin = position_;
                while (!done() && input_[position_] != 'e') {
                    ++position_;
                }
                if (done()) {
                    throw std::runtime_error("Unterminated bencode integer");
                }

                const auto text = input_.substr(begin, position_ - begin);
                ++position_;
                return Value{std::stoll(text)};
            }

            std::string parse_string_text() {
                const auto begin = position_;
                while (!done() && input_[position_] != ':') {
                    if (!std::isdigit(static_cast<unsigned char>(input_[position_]))) {
                        throw std::runtime_error("Invalid bencode string length");
                    }
                    ++position_;
                }
                if (done()) {
                    throw std::runtime_error("Missing bencode string separator");
                }

                const auto length = std::stoull(input_.substr(begin, position_ - begin));
                ++position_;
                if (position_ + length > input_.size()) {
                    throw std::runtime_error("Bencode string exceeds input length");
                }

                auto text = input_.substr(position_, length);
                position_ += length;
                return text;
            }

            Value parse_list() {
                ++position_;
                Value::List values;
                while (!done() && input_[position_] != 'e') {
                    values.emplace_back(parse_value());
                }
                consume_end("Unterminated bencode list");
                return Value{std::move(values)};
            }

            Value parse_dictionary() {
                ++position_;
                Value::Dictionary values;
                while (!done() && input_[position_] != 'e') {
                    auto key = parse_string_text();
                    values.emplace(std::move(key), parse_value());
                }
                consume_end("Unterminated bencode dictionary");
                return Value{std::move(values)};
            }

            void consume_end(const char *message) {
                if (done()) {
                    throw std::runtime_error(message);
                }
                ++position_;
            }

            const std::string &input_;
            std::size_t position_ = 0;
        };
    } // namespace

    const std::string encode(const Value &value) {
        return value.encode();
    }

    std::vector<Value> dencode(const std::string &input) {
        if (input.empty()) {
            throw std::runtime_error("Cannot decode an empty bencode string");
        }

        Parser parser{input};
        std::vector<Value> result;
        while (!parser.done()) {
            result.emplace_back(parser.parse_value());
        }
        return result;
    }
} // namespace z_dht::bencode
