#include <iostream>
#include <ostream>

#include "z_dht/app.hpp"
#include "z_dht/bencode/encoder.hpp"

int main() {
    z_dht::App app;
    app.run();

    return 0;
}
