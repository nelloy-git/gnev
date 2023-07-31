#pragma once

#include <functional>
#include <thread>

class Worker {
public:
    Worker();
    Worker(const Worker&) = delete;
    Worker(const Worker&&) = delete;
    ~Worker();

    void push(const std::function<void()>& cmd) const;

private:
    class State;

    std::shared_ptr<State> _state;
    std::thread _worker_thread;

    static void _Update(const std::shared_ptr<State> state);
};