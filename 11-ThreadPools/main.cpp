#include <QApplication>
#include <QLCDNumber>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "ThreadPool.h"

#include <chrono> 
#include <iostream>

ThreadPool threadPool(std::jthread::hardware_concurrency());
QLCDNumber *lcdNumber = nullptr;

void onButtonClicked()
{
    static int taskCount = 0;
    
    ++taskCount;
    threadPool.addTask([=]() -> int {
        // Simulate a long-running task
        std::clog << "Task started in thread: " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::clog << "Task finished in thread: " << std::this_thread::get_id() << std::endl;
        
        QMetaObject::invokeMethod(lcdNumber, []() {
            lcdNumber->display(taskCount);
        }, Qt::QueuedConnection);
        
        return taskCount;
    });
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    QWidget *centralWidget = new QWidget(&mainWindow);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    lcdNumber = new QLCDNumber(4, centralWidget);
    layout->addWidget(lcdNumber);

    QPushButton *button = new QPushButton("Start task", centralWidget);
    QObject::connect(button, &QPushButton::clicked, &onButtonClicked);
    layout->addWidget(button);

    centralWidget->setLayout(layout);
    mainWindow.setCentralWidget(centralWidget);
    mainWindow.setWindowTitle("Qt Thread Pool Example");
    mainWindow.resize(400, 200);
    mainWindow.show();

    return app.exec();
}
