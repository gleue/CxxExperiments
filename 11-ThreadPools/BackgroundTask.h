#pragma once

#include "BackgroundTask.h"

#include <functional>
#include <memory>
#include <mutex>

class Progress;
class ThreadPool;

/**
 * Represents a background task that can be executed in a thread pool.
 * It manages its own progress and can be cancelled.
 */
class BackgroundTask {

  std::shared_ptr<Progress> progress;
  std::condition_variable done;

  mutable std::mutex mutex;

public:

  BackgroundTask() {
    progress = std::make_shared<Progress>();
  }

  std::shared_ptr<Progress> getProgress() const { return progress; }

  /**
   * Runs the task in the provided thread pool using the specified function.
   *
   * @param threadFunction The function to execute in the background.
   * @param threadPool The thread pool to run the task in.
   * @return True if the task was successfully started, false otherwise.
   */
  bool run(std::function<void(BackgroundTask&)> threadFunction, ThreadPool& threadPool);

  /// @brief Checks if the task is done (either completed or cancelled).
  bool isDone() const;
  /// @brief Checks if the task is completed.
  bool isCompleted() const;
  /// @brief Checks if the task has been cancelled.
  bool isCancelled() const;

  /// @brief Cancels the task if it is currently in progress.
  bool cancel();

  /// @brief Waits for completion or cancellation of the task.
  void wait();
};
