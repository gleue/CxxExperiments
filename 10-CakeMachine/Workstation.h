#pragma once

#include "Shelf.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

template<std::size_t NumWorkers, typename Timer>
class Workstation {

    std::string name{};

    std::atomic_uint count = 0;

    std::shared_ptr<Shelf> input;
    std::shared_ptr<Shelf> output;

public:

    Workstation(std::string named, std::shared_ptr<Shelf> in, std::shared_ptr<Shelf> out, Timer timer) : name{std::move(named)}, input{in}, output{out} {
        for (std::size_t i = 0; i < NumWorkers; i++) {
            auto t = std::jthread([&]{
                while(true) {
                    if (input) input->pop(1);
                    ++count;
                    std::this_thread::sleep_for(timer());
                    if (output) {
                        output->push(1);
                        --count;
                    }
                }
            });
            t.detach();
        }
    }

    std::size_t getCount() const { return count.load(); }

    friend std::ostream& operator<< (std::ostream& os, const Workstation& ws) {
        os << ws.name << ": " << ws.getCount();
        return os;
    }
};
