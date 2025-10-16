#include <iostream>
#include <thread>
#include "BinarySemaphore.h"
#include "CountingSemaphore.h"
using namespace std;

// ---------- CONFIG (change these) ----------
const int BUFFER_CAP           = 5;   // size of ring buffer
const int N_PRODUCERS          = 3;   // number of producer threads
const int N_CONSUMERS          = 2;   // number of consumer threads
const int ITEMS_PER_PRODUCER   = 6;   // items each producer will make
// ------------------------------------------

// shared ring buffer (plain int array)
int buffer_arr[BUFFER_CAP];
int in_idx  = 0;   // write position
int out_idx = 0;   // read position
int curr_sz = 0;   // for display only (not needed for correctness)

// semaphores
BinarySemaphore   mutex_sem(1);              // protects buffer modifications
CountingSemaphore empty_slots(BUFFER_CAP);   // free slots in buffer
CountingSemaphore full_slots(0);             // filled slots in buffer

// PRODUCER: each producer makes ITEMS_PER_PRODUCER items
void producer(int id) {
    for (int k = 0; k < ITEMS_PER_PRODUCER; ++k) {
        int item = id * 1000 + k; // identifiable item ID

        empty_slots.wait();  // wait if buffer is full
        mutex_sem.wait();    // enter critical section

        buffer_arr[in_idx] = item;
        in_idx = (in_idx + 1) % BUFFER_CAP;
        ++curr_sz;

        cout << "[P" << id << "] produced " << item
             << " | buf size: " << curr_sz << "\n";

        mutex_sem.signal();  // exit critical section
        full_slots.signal(); // one more filled slot
    }
}

// CONSUMER: each consumer removes a specific number of items
void consumer(int id, int to_consume) {
    for (int c = 0; c < to_consume; ++c) {
        full_slots.wait();   // wait if buffer is empty
        mutex_sem.wait();    // enter critical section

        int item = buffer_arr[out_idx];
        out_idx = (out_idx + 1) % BUFFER_CAP;
        --curr_sz;

        cout << "    [C" << id << "] consumed " << item
             << " | buf size: " << curr_sz << "\n";

        mutex_sem.signal();  // exit critical section
        empty_slots.signal();// one more free slot
    }
}

int main() {
    cout << "Producer-Consumer (simple array buffer, "
         << N_PRODUCERS << "P/" << N_CONSUMERS << "C)\n";
    cout << "Buffer cap: " << BUFFER_CAP
         << " | Items/producer: " << ITEMS_PER_PRODUCER << "\n\n";

    // total items to be consumed
    const int TOTAL = N_PRODUCERS * ITEMS_PER_PRODUCER;

    // split TOTAL across consumers as evenly as possible
    const int base = TOTAL / N_CONSUMERS;
    const int rem  = TOTAL % N_CONSUMERS;

    // spawn producers
    std::thread prod[N_PRODUCERS];
    for (int p = 0; p < N_PRODUCERS; ++p)
        prod[p] = std::thread(producer, p);

    // spawn consumers (first 'rem' consumers take one extra)
    std::thread cons[N_CONSUMERS];
    for (int c = 0; c < N_CONSUMERS; ++c) {
        int take = base + (c < rem ? 1 : 0);
        cons[c] = std::thread(consumer, c, take);
    }

    // join all
    for (int p = 0; p < N_PRODUCERS; ++p) prod[p].join();
    for (int c = 0; c < N_CONSUMERS; ++c) cons[c].join();

    cout << "\n All items produced and consumed correctly.\n";
    return 0;
}
