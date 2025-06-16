#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>

class Shelf {

  std::size_t count = 0;

  mutable std::mutex mutex;
  std::condition_variable_any condition;

  std::atomic_bool closed = false;

public:

  /// @brief Adds one item to the shelf and wakens waiting consumer threads.
  /// @return true if item could be removed, false if shelf has been closed.
  bool push();

  /// @brief Removes one item from the shelf potentially waiting for an item if the shelf is empty.
  /// @return true if item could be removed, false if shelf has been closed.
  bool pop(std::stop_token token);

  /// @brief Returns the number of items on the shelf.
  /// @return The number of items
  std::size_t getCount() const;

  /// @brief Wakes any waiting threads and prohibits further use.
  void close();
};
