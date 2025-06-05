# Thread Pools (and Qt)

An exploration how to implement thread pools and use them in a Qt applications.

## Requirements

- C++ compiler supporting C++20
- [CMake](https://cmake.org) >= 4.0

## Building

- see [../README.md](../README.md)

## Running

Run the sample application:
```console
$ <build-dir>/11-ThreadPools/pool -t <number of tasks> -s <sleep period>
```

Run the sample Qt application:
```console
$ <build-dir>/11-ThreadPools/threadpools
```

## Resources

- [cppreference.com: Concurrency support library](https://cppreference.com/w/cpp/atomic.html)
- [BS::thread_pool](https://github.com/bshoshany/thread-pool)
- [QThreadPool](https://doc.qt.io/qt-6/qthreadpool.html)