#include "BackgroundTask.h"
#include "ProgressView.h"
#include "ThreadPool.h"

#include <QApplication>
#include <QGroupBox>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <atomic>
#include <chrono> 
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>

// Global thread pool and task pool
ThreadPool threadPool(std::jthread::hardware_concurrency());
std::list<BackgroundTask*> taskPool;

// Global random number generator
std::random_device randomSeed;
std::mt19937 rnGenerator(randomSeed());

/**
 * Creates a BackgroundTask with the specified number of iterations.
 * The task will run in the global thread pool.
 *
 * @param iterations The number of iterations for the task.
 * @return A tuple containing the BackgroundTask pointer and the function being executed.
 */
std::tuple<BackgroundTask*, std::function<void(BackgroundTask&)> > createTask(int iterations) {
    auto task = taskPool.emplace_back(new BackgroundTask());
    auto progress = task->getProgress();
    progress->setTotal(iterations);

    auto function = [iterations](BackgroundTask& task) -> void {
        std::clog << "Task started in thread: " << std::this_thread::get_id() << std::endl;

        // Simulate a long-running task
        auto remainingIterations = iterations;
        for (; !task.isCancelled() && remainingIterations >= 1; remainingIterations--) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            task.getProgress()->increment();
        }

        if (task.isCancelled()) {
            std::clog << "Task cancelled in thread: " << std::this_thread::get_id() << " with " << remainingIterations << " iterations remaining." << std::endl;
        } else {
            task.getProgress()->complete();
            std::clog << "Task completed in thread: " << std::this_thread::get_id() << std::endl;
        }
    };

    return std::make_tuple(task, function);
}

/**
 * Handles the task completion by removing it from the task pool and deleting the ProgressView.
 * 
 * @param progressView The ProgressView associated with the completed task.
 */
void onTaskDone(ProgressView* progressView)
{
    const auto progress = progressView->getProgress();

    if (auto task = std::find_if(taskPool.begin(), taskPool.end(), [=](auto task) { return task->getProgress() == progress; }); task != taskPool.end()) {
        taskPool.erase(task);
        delete *task;
    }

    progressView->deleteLater();
}

/**
 * Handles the start button click event to create and run a new BackgroundTask.
 * 
 * @param parent The parent widget for the ProgressView.
 * @param layout The layout to add the ProgressView to.
 */
void onStartButtonClicked(QWidget* parent, QLayout* layout)
{
    static std::uniform_int_distribution<std::size_t> dist(50.0, 150.0);
    const auto iterations = dist(rnGenerator);

    auto [task, function] = createTask(iterations);
    auto progressView = new ProgressView(task->getProgress(), parent);
    progressView->setLabelFormat("Completed %v of %m iterations (%p%)");
    progressView->setProgressRange(0, iterations);
    layout->addWidget(progressView);
    
    QObject::connect(progressView, &ProgressView::taskDone, [=](){ onTaskDone(progressView); });

    task->run(function, threadPool);
}

int main(int argc, char *argv[])
{
    // Main window setup
    QApplication app(argc, argv);
    QMainWindow mainWindow;
    QWidget *mainWidget = new QWidget(&mainWindow);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    // Create a group box to hold the active tasks
    QGroupBox *taskGroupBox = new QGroupBox(QObject::tr("Active tasks"), mainWidget);
    mainLayout->addWidget(taskGroupBox);
    QVBoxLayout *taskLayout = new QVBoxLayout(taskGroupBox);
    taskGroupBox->setLayout(taskLayout);

    // Create a button to start a new background task
    QPushButton *startButton = new QPushButton(QObject::tr("Start task"), mainWidget);
    QObject::connect(startButton, &QPushButton::clicked, [=](){ onStartButtonClicked(taskGroupBox, taskGroupBox->layout()); });
    mainLayout->addWidget(startButton);

    mainWindow.setCentralWidget(mainWidget);
    mainWindow.setWindowTitle(QObject::tr("Qt Thread Pool Example"));
    mainWindow.resize(400, 200);
    mainWindow.show();

    return app.exec();
}
