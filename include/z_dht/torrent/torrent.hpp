//
// Created by xiezc on 2026/6/28.
//
#include "z_dht/bencode/value.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>


namespace z_dht::torrent {
    using namespace z_dht::bencode;

    struct InfoFiles {
        std::uint64_t length;
        std::string path;
    };

    struct SignalInfo {
        std::variant<std::byte> content;
        std::string name;
        std::uint64_t length;
        std::uint64_t piece_length;
        std::string pieces;
        std::uint64_t private_;
    };

    struct MultiInfo {
        std::variant<std::byte> content;
        std::string name;
        std::uint64_t piece_length;
        std::string pieces;
        std::vector<InfoFiles> files;
        std::uint64_t private_;
    };

    class Torrent {
        std::string announce;
        std::vector<std::string> announce_list;
        std::size_t create_data;
        std::string create_by;
        std::string comment;

        std::byte info_hash[20];
        std::variant<SignalInfo, MultiInfo> info;

    public:
        Torrent(Value::Dict &dict);
    };


    const Torrent decode(std::istream &in);
}
