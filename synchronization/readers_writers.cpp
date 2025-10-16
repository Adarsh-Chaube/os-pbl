#include <iostream>
#include <thread>
#include "BinarySemaphore.h"
using namespace std;

const int READERS = 3;
const int WRITERS = 2;
const int READS_PER_READER = 3;
const int WRITES_PER_WRITER = 2;


BinarySemaphore mutex_sem(1);   // protects readcount
BinarySemaphore rw_mutex(1);    // controls access to shared resource
int readcount = 0;              // number of active readers
int shared_data = 0;            // shared variable (the "resource")

void reader(int id) {
    for (int i = 1; i <= READS_PER_READER; ++i) {
        // entry section
        mutex_sem.wait();
        readcount++;
        if (readcount == 1)
            rw_mutex.wait();  // first reader locks resource
        mutex_sem.signal();

        // critical section (reading)
        cout << "Reader " << id << " reads data = " << shared_data << endl;

        // exit section
        mutex_sem.wait();
        readcount--;
        if (readcount == 0)
            rw_mutex.signal();  // last reader releases resource
        mutex_sem.signal();
    }
}

void writer(int id) {
    for (int i = 1; i <= WRITES_PER_WRITER; ++i) {
        rw_mutex.wait();  // lock resource for exclusive writing
        shared_data += 10;  // simulate data modification
        cout << " Writer " << id << " writes data = " << shared_data << endl;
        rw_mutex.signal();  // release resource
    }
}

int main() {
    cout << "Readers–Writers Problem (Reader Priority)\n";
    cout << "Readers: " << READERS << ", Writers: " << WRITERS << "\n\n";

    thread r[READERS];
    thread w[WRITERS];

    // spawn readers
    for (int i = 0; i < READERS; ++i)
        r[i] = thread(reader, i);

    // spawn writers
    for (int i = 0; i < WRITERS; ++i)
        w[i] = thread(writer, i);

    // join all
    for (int i = 0; i < READERS; ++i) r[i].join();
    for (int i = 0; i < WRITERS; ++i) w[i].join();

    cout << "\n All readers and writers finished without conflict.\n";
    return 0;
}
