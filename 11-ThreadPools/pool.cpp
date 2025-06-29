#include "ThreadPool.h"

#include "../shared/mygetopt.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <latch>
#include <mutex>
#include <ranges>
#include <string>
#include <sstream>
#include <thread>
#include <vector>


const int defaultNumberOfTasks = std::thread::hardware_concurrency() << 1; // double the number of threads
const int defaultSleepInterval = 4;

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
            case 't':
                numberOfTasks = std::clamp(atoi(optarg), 1, 100);
                break;
            case 's':
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
std::atomic_int taskCount = 0;

void printTaskInfo(int taskId, const std::string& message) {
    std::scoped_lock lock(clogmutex);
    std::clog << "Task " << taskId << " (on thread " << std::this_thread::get_id() << ") " << message << "\n";
}

template<typename T>
void printTaskResult(int taskId, std::future<T>& future)
{
    try {
        auto result = future.get();
        std::clog << "Task " << taskId << " completed with result: " << result << " ms\n";
    } catch (const std::exception& e) {
        std::cerr << "Task " << taskId << " failed: " << e.what() << "\n";
    }
}

auto addIntTask(ThreadPool& threadPool, std::latch& latch, std::size_t seconds = 4) -> std::future<int>
{
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

auto addStringTask(ThreadPool& threadPool, std::latch& latch, std::size_t seconds = 4) -> std::future<std::string>
{
    return threadPool.addTask([seconds, taskId=taskCount++, &latch]() -> std::string {
        printTaskInfo(taskId, "started");
        auto begin = std::chrono::steady_clock::now();

        // Simulate a long-running task by sleeping for a specified number of seconds
        std::this_thread::sleep_for(std::chrono::seconds(seconds));

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        printTaskInfo(taskId, "finished");

        latch.count_down();
        std::stringstream ss;
        ss << duration.count() << std::ends;
        return ss.str();
    });
}

int main(int argc, char *argv[])
{
    auto index = parseArguments(argc, argv);
    if (argc - optind != 0) {
        usage(argv[0]);
        return 1;
    }

    auto maxThreads = std::thread::hardware_concurrency();
    std::clog << "Creating thread pool with " << maxThreads << " threads \n";
    ThreadPool threadPool(maxThreads);

    std::latch barrier(numberOfTasks);
    std::vector<std::future<int>> intFutures;
    std::vector<std::future<std::string>> stringFutures;
    for (std::size_t i = 0; i < numberOfTasks; ++i) {
        std::scoped_lock lock(clogmutex);
        if (i % 2 == 0) {
            std::clog << "Adding int task " << i << " to the pool\n";
            intFutures.emplace_back(addIntTask(threadPool, barrier, sleepInterval));
        } else {            
            std::clog << "Adding string task " << i << " to the pool\n";
            stringFutures.emplace_back(addStringTask(threadPool, barrier, sleepInterval));
        }
    }

    std::clog << "Waiting for tasks to complete...\n";
    barrier.wait();
    std::clog << "All tasks have finished\n";

    for (std::size_t i = 0; i < numberOfTasks; ++i) {
        if (i % 2 == 0) {
            printTaskResult(i, intFutures[i >> 1]);
        } else {
            printTaskResult(i, stringFutures[i >> 1]);
        }
    }
    
    std::clog << "Destroying thread pool\n";
}
 