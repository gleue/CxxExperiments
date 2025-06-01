#include "ThreadPool.h"

ThreadPool::ThreadPool(std::size_t numThreads): maxThreads(numThreads) {
    for (std::size_t i = 0; i < maxThreads; ++i) {
        threads.emplace_back([this, i]() {
            while (!stopAll.stop_requested()) {
                Task nextTask;
                {
                    std::unique_lock<std::mutex> lock(mutex);

                    if (tasks.empty()) taskAvailable.wait(lock, [this]() { return !tasks.empty() || stopAll.stop_requested(); });
                    if (stopAll.stop_requested()) return;

                    nextTask = std::move(tasks.front()) ;
                    tasks.pop_front();
                }

                nextTask();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    stopAll.request_stop();
    taskAvailable.notify_all();
}
