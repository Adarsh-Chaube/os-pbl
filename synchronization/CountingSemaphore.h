#pragma once
#include <mutex>
#include <condition_variable>

class CountingSemaphore {
private:
    int value;
    std::mutex mtx;
    std::condition_variable cv;

public:
    explicit CountingSemaphore(int initial) : value(initial) {}

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        // no lambda; just a plain while-loop
        while (value == 0) {
            cv.wait(lock);
        }
        --value;
    }

    void signal() {
        std::lock_guard<std::mutex> lock(mtx);
        ++value;
        cv.notify_one();
    }
};
