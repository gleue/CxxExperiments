#include "BackgroundTask.h"

#include "Progress.h"
#include "ThreadPool.h"

bool BackgroundTask::run(std::function<void(BackgroundTask&)> threadFunction, ThreadPool& threadPool) {
    if (!progress->start()) return false;

    threadPool.addTask([this, threadFunction]() {
        threadFunction(*this);
        if (!progress->isCancelled()) progress->complete();
    });
    return true;
}

bool BackgroundTask::isDone() const {
    std::lock_guard<std::mutex> lock(mutex);
    return progress->isCompleted() || progress->isCancelled();
}

bool BackgroundTask::isCompleted() const {
    std::lock_guard<std::mutex> lock(mutex);
    return progress->isCompleted();
}

bool BackgroundTask::isCancelled() const {
    std::lock_guard<std::mutex> lock(mutex);
    return progress->isCancelled();
}

bool BackgroundTask::cancel() {
    return progress->cancel();
}

void BackgroundTask::wait() {
    std::unique_lock<std::mutex> lock(mutex);
    done.wait(lock, [this]() { return isDone(); });
}
