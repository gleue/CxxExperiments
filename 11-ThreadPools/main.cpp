#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMetaObject>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "ThreadPool.h"

#include <atomic>
#include <chrono> 
#include <iostream>
#include <sstream>
#include <string>
#include <thread>


ThreadPool threadPool(std::jthread::hardware_concurrency());

std::atomic_bool cancelTask = false;
QProgressBar *progressBar = nullptr;
QLabel *resultLabel = nullptr;

void onStartButtonClicked(QPushButton* startButton, QPushButton* cancelButton)
{
    const int iterations = 100;

    // Disable the button to prevent multiple clicks
    startButton->setEnabled(false);
    progressBar->setRange(0, iterations);
    progressBar->setValue(0);
    resultLabel->setText("Task started, please wait...");

    threadPool.addTask([=]() -> std::string {
        std::clog << "Task started in thread: " << std::this_thread::get_id() << std::endl;

        // Simulate a long-running task
        auto remainingIterations = iterations;
        for (; !cancelTask && remainingIterations >= 1; remainingIterations--) {
            // Simulate work
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            QMetaObject::invokeMethod(progressBar, [=]() {
                progressBar->setValue(iterations - remainingIterations);
            }, Qt::QueuedConnection);
        }

        bool cancelled = cancelTask;
        cancelTask = false;

        // Prepare and log result
        std::stringstream resultStream;
        resultStream << "Task " << (cancelled ? "cancelled" : "finished") << " with " << remainingIterations << " iterations remaining.";
        auto result = resultStream.str();
        std::clog << result << " in thread: " << std::this_thread::get_id() << std::endl;

        // Reset UI
        QMetaObject::invokeMethod(progressBar, [=]() {
            if (cancelled) progressBar->setValue(0);
            startButton->setEnabled(true);
            cancelButton->setEnabled(false);
            resultLabel->setText(QString::fromStdString(result));
        }, Qt::QueuedConnection);

        return result;
    });

    // Enable the cancel button
    cancelButton->setEnabled(true);
}

void onCancelButtonClicked(QPushButton* cancelButton)
{
    cancelButton->setEnabled(false); // Disable the button to prevent multiple clicks
    cancelTask = true; // Set the flag to cancel the task
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    QWidget *centralWidget = new QWidget(&mainWindow);
    QVBoxLayout *vLayout = new QVBoxLayout(centralWidget);

    resultLabel = new QLabel("Result will be displayed here", centralWidget);
    vLayout->addWidget(resultLabel);

    progressBar = new QProgressBar(centralWidget);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    progressBar->setFormat("Completed %v of %m iterations (%p%)");
    vLayout->addWidget(progressBar);

    // Create a horizontal layout for the buttons
    QHBoxLayout *hLayout = new QHBoxLayout();
    QPushButton *cancelButton = new QPushButton("Cancel task", centralWidget);
    cancelButton->setEnabled(false); // Initially disabled
    QObject::connect(cancelButton, &QPushButton::clicked, [=](){ onCancelButtonClicked(cancelButton); });
    QPushButton *startButton = new QPushButton("Start task", centralWidget);
    QObject::connect(startButton, &QPushButton::clicked, [=](){ onStartButtonClicked(startButton, cancelButton); });
    hLayout->addWidget(startButton);
    hLayout->addWidget(cancelButton);
    vLayout->addLayout(hLayout);

    centralWidget->setLayout(vLayout);
    mainWindow.setCentralWidget(centralWidget);
    mainWindow.setWindowTitle("Qt Thread Pool Example");
    mainWindow.resize(400, 200);
    mainWindow.show();

    return app.exec();
}
