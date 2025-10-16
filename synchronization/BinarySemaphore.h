#pragma once
#include <mutex>
#include <condition_variable>

class BinarySemaphore {
private:
    int value;
    std::mutex mtx;
    std::condition_variable cv;

public:
    explicit BinarySemaphore(int initial = 1) : value(initial) {}

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{ return value > 0; });
        value--;
    }

    void signal() {
        std::lock_guard<std::mutex> lock(mtx);
        value++;
        cv.notify_one();
    }
};
