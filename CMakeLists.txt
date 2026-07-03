cmake_minimum_required(VERSION 4.2)

project(z_dht LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
        z_dht
        src/main.cpp
        src/app.cpp
        src/bencode/encoder.cpp
        src/bencode/value.cpp
        src/dht/engine.cpp
        src/net/udp_socket.cpp
        src/torrent/torrent.cpp
)

target_include_directories(z_dht PRIVATE include)

# --- tests ---
add_executable(z_dht_test
        tests/bencode_test.cpp
        src/bencode/encoder.cpp
        src/bencode/value.cpp
)

target_include_directories(z_dht_test PRIVATE include)
