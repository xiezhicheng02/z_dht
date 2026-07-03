#include "z_dht/torrent/torrent.hpp"

#include "z_dht/bencode/value.hpp"
#include "z_dht/bencode/encoder.hpp"
#include <string>
#include <iostream>
#include <memory>

//
// Created by xiezc on 2026/6/28.
//
namespace z_dht::torrent {
    // 从输入流中解析一个 Value
    Torrent::Torrent(Value::Dict &dict) {
        this->announce = std::get<std::string>(dict["announce"].data());
        auto list = std::get<Value::List>(dict["announce-list"].data());
        for (auto &value: list) {
            auto data = value.data();
            this->announce_list.push_back(std::get<Value::String>(data));
        }
        this->create_data = std::get<Value::Integer>(dict["create data"].data());
        this->comment = std::get<std::string>(dict["comment"].data());
        this->create_by = std::get<std::string>(dict["created by"].data());
        auto info = std::get<Value::Dict>(dict["info"].data());
        auto length = std::get<Value::Integer>(info["length"].data());
        if (length != NULL) {
            //多文件
            auto &signalInfo = std::get<SignalInfo>(this->info);
            signalInfo.name = std::get<Value::String>(info["name"].data());
            signalInfo.length = length;
            signalInfo.private_ = std::get<Value::Integer>(info["private"].data());
            signalInfo.pieces = std::get<Value::String>(info["pieces"].data());
            signalInfo.piece_length = std::get<Value::Integer>(info["piece length"].data());
        } else {
            auto &multiInfo = std::get<MultiInfo>(this->info);
            multiInfo.name = std::get<Value::String>(info["name"].data());
            multiInfo.private_ = std::get<Value::Integer>(info["private"].data());
            multiInfo.pieces = std::get<Value::String>(info["pieces"].data());
            multiInfo.piece_length = std::get<Value::Integer>(info["piece length"].data());
            auto files = std::get<Value::List>(info["files"].data());
            for (auto &value: files) {
                auto data = value.data();
                auto dict = std::get<Value::Dict>(data);
                InfoFiles file;
                file.path = std::get<Value::String>(dict["path"].data());
                file.length = std::get<Value::Integer>(dict["length"].data());
                multiInfo.files.push_back(file);
            }
        }
    }
}
