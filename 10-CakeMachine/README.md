# Cake Machine

A Kata about multithreading & concurrency inspired by https://github.com/aimenux/CakeMachineKata.

## Performance estimations

- Sequential processing: [5 - 8]s + 10s + 2s = [17 - 20]s per cake --> [3 - 3.5] cakes per minute
- Parallel processing:
  - Preparation: [5 - 8]s per cake --> [7.5 - 12] cakes per minute --> @3 workers: [22.5 - 36] cakes per minute
  - Baking: 10s per cake --> 6 cakes per minute --> @5 workers: 30 cakes per minute
  - Packaging: 2s per cake --> 30 cakes per minute --> @2 workers: 60 cakes per minute
  - TOTAL: min([22.5-36], 30, 60) cakes per minute --> [22.5 - 30] per minute

## Requirements

- C++ compiler supporting C++20
- [CMake](https://cmake.org) >= 3.10

## Building

- see [../README.md](../README.md)

## Running

- Run the solution
  ```console
  $ <build-dir>/10-CakeMachine/cake_machine [report-interval]
  ```
  As requested in the Kata, summations have to be printed periodically (by default every 10s). The optional `report-interval` argument may be used to specify a different interval length in seconds.

## Resources

- [The original Kata](https://github.com/aimenux/CakeMachineKata)
- [cppreference.com: `std::atomic`](https://en.cppreference.com/w/cpp/atomic/atomic)
- [cppreference.com: `std::chrono::duration`](https://en.cppreference.com/w/cpp/chrono/duration)
- [cppreference.com: `std::condition_variable`](https://en.cppreference.com/w/cpp/thread/condition_variable)
- [cppreference.com: `std::jthread`](https://en.cppreference.com/w/cpp/thread/jthread)
- [cppreference.com: Pseudo-random number generation](https://en.cppreference.com/w/cpp/numeric/random)
- [cppreference.com: `std::this_thread::sleep_for`](https://en.cppreference.com/w/cpp/thread/sleep_for)
