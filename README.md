# z-dht

A small C++20 learning project for implementing the BitTorrent DHT protocol with coroutines and UDP.

## Layout

- `include/z_dht/coro`: coroutine primitives
- `include/z_dht/net`: UDP transport interfaces
- `include/z_dht/dht`: DHT domain types and engine
- `src`: executable and implementation files

## Build

```sh
cmake -S . -B build
cmake --build build
./build/z_dht
```

## Suggested Milestones

1. Replace the UDP stub with a real non-blocking socket.
2. Add a small event loop that resumes coroutine tasks on socket readiness.
3. Implement bencode encode/decode for DHT messages.
4. Add `ping`, `find_node`, `get_peers`, and `announce_peer` message handling.
5. Add routing table buckets and bootstrap node discovery.
