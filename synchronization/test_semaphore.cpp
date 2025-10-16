#include <iostream>
#include <thread>
#include <vector>
#include "BinarySemaphore.h"
using namespace std;

// configuration
static const int THREADS = 8;       // number of threads (2, 4, 8, 16...)
static const int ITERS   = 250'000; // iterations per thread

long long counter = 0;              // shared variable
BinarySemaphore mutex_sem(1);       // binary semaphore (1 = unlocked)

// worker function that each thread runs
void increment_worker(int id) {
    for (int i = 0; i < ITERS; ++i) {
        mutex_sem.wait();           // entry section (wait for access)
        ++counter;                  // critical section (shared update)
        mutex_sem.signal();         // exit section (release access)
    }
}

int main() {
    cout << "Starting " << THREADS << " threads..." << endl;

    vector<thread> threads;
    threads.reserve(THREADS);

    // create threads
    for (int i = 0; i < THREADS; ++i)
        threads.emplace_back(increment_worker, i);

    // wait for all threads to complete
    for (auto &t : threads)
        t.join();

    cout << "\nExpected: " << 1LL * THREADS * ITERS << endl;
    cout << "Actual:   " << counter << endl;

    if (counter == 1LL * THREADS * ITERS)
        cout << "✅ Synchronization successful! (No race condition)" << endl;
    else
        cout << "❌ Race condition detected!" << endl;

    return 0;
}
