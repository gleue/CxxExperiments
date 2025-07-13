#pragma once

#include <QObject>

#include <algorithm>
#include <atomic>
#include <mutex>

enum class ProgressStatus {
    NotStarted,
    InProgress,
    Completed,
    Cancelled
};

class Progress: public QObject {

    Q_OBJECT

    std::atomic<ProgressStatus> status = ProgressStatus::NotStarted;

    std::atomic_int total = 0;   // Total number of items to process
    std::atomic_int current = 0; // Current progress count

    mutable std::mutex mutex;

public:

    ProgressStatus getStatus() const { return status.load(); }

    /// @brief Checks if the progress is idle (not started).
    bool isIdle() const { return status.load() == ProgressStatus::NotStarted; }
   
    /// @brief Starts progress if it has not already started
    bool start() { return updateStatus(ProgressStatus::NotStarted, ProgressStatus::InProgress); }
    /// @brief Checks if the progress has been started.
    bool isInProgress() const { return status.load() == ProgressStatus::InProgress; }

    /// @brief Completes progress if it is currently in progress
    bool complete() { return updateStatus(ProgressStatus::InProgress, ProgressStatus::Completed); }
    /// @brief Checks if the progress has been completed.
    bool isCompleted() const { return status.load() == ProgressStatus::Completed; }
    
    /// @brief Cancels progress if it is currently in progress
    bool cancel() { return updateStatus(ProgressStatus::InProgress, ProgressStatus::Cancelled); }
    /// @brief Checks if the progress has been cancelled.
    bool isCancelled() const { return status.load() == ProgressStatus::Cancelled; }

    /// @brief  Get the total number of items to process
    int getTotal() const { return total.load(); }
    /// @brief Set the total number of items to process
    void setTotal(int totalItems);

    /// @brief Get the current progress count
    int getCurrent() const { return current; }

    /// @brief Set the current progress count ensuring it does not exceed total
    void setCurrent(int currentItems);

    /// @brief Increment the current progress by one
    void increment() { setCurrent(current + 1); }

    /// @brief Get the current progress as a value between 0.0 and 1.0
    double getProgress() const {
        return (total > 0) ? (static_cast<double>(current) / total) : 0.0;
    }

signals:

    // Signal to notify that status has changed
    void statusChanged(ProgressStatus newStatus);
    
    // Signal to notify that progress has changed
    void progressChanged(double newProgress);

private:

    bool updateStatus(ProgressStatus expected, ProgressStatus newStatus);
};
