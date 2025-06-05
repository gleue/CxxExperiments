#include "ThreadPool.h"

#include <atomic>
#include <chrono>
#include <getopt.h>
#include <iostream>
#include <latch>
#include <ranges>


constexpr int defaultNumberOfTasks = 16;
constexpr int defaultSleepInterval = 4;

auto numberOfTasks = defaultNumberOfTasks;
auto sleepInterval = defaultSleepInterval;

void usage(const char* argv0) {
    std::cerr << "Usage\n\n  " << argv0 << " [options]\n\n";
    std::cerr << "Creates a thread pool and adds a number of long running tasks to it.\n\n";
    std::cerr << "Options\n\n";
    std::cerr << "  -h | --help          = Print this help\n" \
                 "  -t | --tasks number  = Number of tasks to run on the pool\n" \
                 "  -s | --sleep seconds = Seconds for each task to take\n\n";
    std::cerr << std::endl;
}

int parseArguments(int argc, char* argv[]) {

    static const option longOptions[] = {
        { "help", 0, nullptr, 'h'},
        { "tasks", 1, nullptr, 't' },
        { "sleep", 1, nullptr, 's' },
        { nullptr, 0, nullptr, 0 },
    };

    int opt = -1;

    while ((opt = getopt_long(argc, argv, "ht:s:", longOptions, nullptr)) > 0) {
        switch (opt) {
            case 'h':
                usage(argv[0]);
                exit(0);
            case 'c':
                numberOfTasks = std::clamp(atoi(optarg), 1, 100);
                break;
            case 'r':
                sleepInterval = std::clamp(atoi(optarg), 1, 60);
                break;
            case '?':
                usage(argv[0]);
                exit(1);
        }
    }

    return optind;
}

std::mutex clogmutex;

void printTaskInfo(int taskId, const std::string& message) {
    std::scoped_lock lock(clogmutex);
    std::clog << "Task " << taskId << " (" << std::this_thread::get_id() << ") " << message << "\n";
}

auto addTask(ThreadPool& threadPool, std::latch& latch, std::size_t seconds = 5) -> std::future<int>
{
    static std::atomic_int taskCount = 0;

    return threadPool.addTask([seconds, taskId=taskCount++, &latch]() -> int {
        printTaskInfo(taskId, "started");
        auto begin = std::chrono::steady_clock::now();

        // Simulate a long-running task by sleeping for a specified number of seconds
        std::this_thread::sleep_for(std::chrono::seconds(seconds));

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        printTaskInfo(taskId, "finished");

        latch.count_down();
        return duration.count();
    });
}

int main(int argc, char *argv[])
{
    auto index = parseArguments(argc, argv);

    if (argc - optind != 0) {
        usage(argv[0]);
        return 1;
    }

    std::clog << "Creating thread pool\n";
    ThreadPool threadPool(std::thread::hardware_concurrency());

    std::latch barrier(numberOfTasks);
    std::vector<std::future<int>> futures;
    for (std::size_t i = 0; i < numberOfTasks; ++i) {
        std::scoped_lock lock(clogmutex);
        std::clog << "Adding task " << i + 1 << " to the pool\n";
        futures.emplace_back(addTask(threadPool, barrier, sleepInterval));
    }

    std::clog << "Waiting for tasks to complete...\n";
    barrier.wait();
    std::clog << "All tasks have finished\n";

    for (auto [index, future]: std::views::enumerate(futures)) {
        try {
            auto result = future.get();
            std::clog << "Task " << index << " completed in: " << std::chrono::milliseconds(result) << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Task failed: " << e.what() << "\n";
        }
    }
    std::clog << "Destroying thread pool\n";
}
 