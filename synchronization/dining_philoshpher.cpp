#include <iostream>
#include <thread>
#include "BinarySemaphore.h"
#include "CountingSemaphore.h"
using namespace std;


const int N_PHILOSOPHERS = 5;   // number of philosophers
const int EAT_ROUNDS     = 3;   // how many times each philosopher eats

// forks[i] is the fork between philosopher i and (i+1)%N
BinarySemaphore forks_sem[N_PHILOSOPHERS] = {
    BinarySemaphore(1), BinarySemaphore(1), BinarySemaphore(1),
    BinarySemaphore(1), BinarySemaphore(1) // adjust if N changes
};

// Butler limits "entry to pick forks" to N-1 philosophers
CountingSemaphore butler(N_PHILOSOPHERS - 1);

// tiny CPU delay 
inline void tiny_delay(int loops) {
    volatile int x = 0;
    for (int i = 0; i < loops; ++i) x += i;
}

void philosopher(int id) {
    int left  = id;                              
    int right = (id + 1) % N_PHILOSOPHERS;       

    for (int round = 1; round <= EAT_ROUNDS; ++round) {
        // THINK
        cout << "P" << id << " is thinking\n";
        tiny_delay(1000000);

        butler.wait();
        
        forks_sem[left].wait();
        cout << "P" << id << " picked LEFT fork " << left << "\n";

        forks_sem[right].wait();
        cout << "P" << id << " picked RIGHT fork " << right << "\n";

        // EAT
        cout << " P" << id << " is EATING (round " << round << ")\n";
        tiny_delay(2000000);

        // Put down forks 
        forks_sem[right].signal();
        cout << "P" << id << " put RIGHT fork " << right << "\n";

        forks_sem[left].signal();
        cout << "P" << id << " put LEFT fork " << left << "\n";

        // Leave the dining area (let someone else try)
        butler.signal();

        // Back to thinking before next round
        tiny_delay(1000000);
    }

    cout << " P" << id << " finished all eating rounds\n";
}

int main() {
    cout << "Dining Philosophers with Butler (N-1 rule)\n";
    cout << "Philosophers: " << N_PHILOSOPHERS
         << " | Eat rounds each: " << EAT_ROUNDS << "\n\n";

    // spawn threads
    thread th[N_PHILOSOPHERS];
    for (int i = 0; i < N_PHILOSOPHERS; ++i) th[i] = thread(philosopher, i);

    // join
    for (int i = 0; i < N_PHILOSOPHERS; ++i) th[i].join();

    cout << "\n All philosophers have eaten and finished without deadlock.\n";
    return 0;
}
