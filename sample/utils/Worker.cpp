#include "utils/Worker.hpp"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

#include <iostream>

class Worker::State {
    using Cmd = std::function<void()>;
public:
    State() :
        alive(true){
    }

    ~State(){}

    void terminate(){
        alive = false;
        c.notify_one();
    }

    void push(const Cmd& t){
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    std::optional<Cmd> pop(){
        std::unique_lock<std::mutex> lock(m);
        while(q.empty()){
            if (!alive){
                return std::nullopt;
            }
            
            // release lock as long as the wait and reaquire it afterwards.
            c.wait(lock);
        }

        Cmd val = q.front();
        q.pop();
        return val;
    }

private:
    std::atomic<bool> alive;
    std::queue<Cmd> q;
    mutable std::mutex m;
    std::condition_variable c;
};

Worker::Worker() :
    _state(std::make_shared<Worker::State>()),
    _worker_thread(&Worker::_Update, _state){
}

Worker::~Worker(){
    _state->terminate();
    std::cout << "Joined worker" << std::endl;
    _worker_thread.join();
    std::cout << "Worker finished" << std::endl;
}

void Worker::push(const std::function<void()>& cmd) const {
    _state->push(cmd);
}

void Worker::_Update(const std::shared_ptr<State> state){
    while(true){
        auto cmd = state->pop();
        if (!cmd.has_value()){
            return;
        }
        cmd.value()();
    }
}

