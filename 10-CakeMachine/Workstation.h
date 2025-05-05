#pragma once

#include "Shelf.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

template<std::size_t NumWorkers, typename Timer>
class Workstation {

    std::string name{};

    std::atomic_uint count = 0;

    std::shared_ptr<Shelf> input;
    std::shared_ptr<Shelf> output;

    std::vector<std::shared_ptr<std::jthread>> workers;

public:

    /// @brief Creates a workstation connected to input and output shelves.
    /// @param named The workstation name
    /// @param in An optional input shelf
    /// @param out An optional output
    /// @param timer A callable returning the duration to process a single item 
    Workstation(std::string named, std::shared_ptr<Shelf> in, std::shared_ptr<Shelf> out, Timer timer) : name{std::move(named)}, input{in}, output{out} {
        for (std::size_t i = 0; i < NumWorkers; i++) {
            auto t = std::make_shared<std::jthread>([&](std::stop_token token){
                while(!token.stop_requested()) {
                    if (input && !input->pop(token)) break;
                    ++count;
                    std::this_thread::sleep_for(std::invoke(timer));
                    if (output) {
                        if (!output->push()) break;
                        --count;
                    }
                }
            });
            workers.emplace_back(t);
        }
    }

    /// @brief Stops all worker threads.
    void stop() {
        for (auto worker: workers) worker->request_stop();
    }

    /// @brief Returns the number items currently being worked on.
    /// @return Number of items
    std::size_t getCount() const { return count.load(); }

    /// @brief Outputs the workstation name and number of worked upon items.
    /// @param os The stream to output to
    /// @param ws The workstation to output data about
    /// @return The output stream
    friend std::ostream& operator<< (std::ostream& os, const Workstation& ws) {
        os << ws.name << ": " << ws.getCount();
        return os;
    }
};
