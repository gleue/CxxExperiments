#include <functional>
#include <future>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool {

  using Task = std::packaged_task<void()>;

  std::mutex mutex;

  std::size_t maxThreads = 0;
  std::vector<std::jthread> threads{};

  std::stop_source stopAll;

  std::list<Task> tasks{};
  std::condition_variable_any taskAvailable;

public:

  explicit ThreadPool(std::size_t numThreads);

  ~ThreadPool();

  std::size_t numThreads() const { return threads.size(); }

  template<typename Func>
    requires std::invocable<Func>
  auto addTask(Func&& func) -> std::future<std::invoke_result_t<Func>> {
    using ReturnType = std::invoke_result_t<Func>;
    using TaskType = std::packaged_task<ReturnType()>;

    TaskType newTask(std::forward<Func>(func));
    auto future = newTask.get_future();

    std::scoped_lock<std::mutex> lock(mutex);

    tasks.emplace_back(std::move(newTask));
    taskAvailable.notify_one();
    return future;
  }
};
