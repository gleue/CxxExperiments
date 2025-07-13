#include "ThreadPool.h"

ThreadPool::ThreadPool(std::size_t numThreads) : maxThreads(numThreads) {
  for (std::size_t i = 0; i < maxThreads; ++i) {
    // Create new thread and add it to the pool
    threads.emplace_back([this]() {
      for (auto token = stopAll.get_token(); !token.stop_requested();) {
        Task nextTask;
        {
          // Use this scope to unlock mutex after task extraction
          std::unique_lock lock(mutex);

          if (tasks.empty()) {
            // Wait for available tasks or stop request
            taskAvailable.wait(lock, [this, token]() {
              return !tasks.empty() || token.stop_requested();
              });
          }
          // If stop requested terminate the thread
          if (token.stop_requested()) return;

          // Extract the next task
          nextTask = std::move(tasks.front());
          tasks.pop_front();
        }
        // Execute the task
        nextTask();
      }
      });
  }
}

ThreadPool::~ThreadPool() {
  stopAll.request_stop();
  taskAvailable.notify_all();
}
