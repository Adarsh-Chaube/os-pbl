#include <iostream>
using namespace std;

const int N = 5; // processes
const int M = 3; // resource types

void detectAndRecover(int Allocation[N][M], int Request[N][M], int Available[M], bool doRecovery) {
    int Work[M];
    bool Finish[N];

    for (int j = 0; j < M; ++j) Work[j] = Available[j];

    for (int i = 0; i < N; ++i) {
        bool holdsSomething = false;
        for (int j = 0; j < M; ++j)
            if (Allocation[i][j] != 0) { holdsSomething = true; break; }
        Finish[i] = !holdsSomething; // if process holds nothing → not part of deadlock
    }

    bool progress = true;
    while (progress) {
        progress = false;
        for (int i = 0; i < N; ++i) {
            if (!Finish[i]) {
                bool canFinish = true;
                for (int j = 0; j < M; ++j) {
                    if (Request[i][j] > Work[j]) { canFinish = false; break; }
                }
                if (canFinish) {
                    for (int j = 0; j < M; ++j) Work[j] += Allocation[i][j];
                    Finish[i] = true;
                    progress = true;
                }
            }
        }
    }

    bool anyDeadlock = false;
    for (int i = 0; i < N; ++i) {
        if (!Finish[i]) {
            if (!anyDeadlock) {
                cout << "Deadlock detected among processes:\n";
                anyDeadlock = true;
            }
            cout << "P" << i << "\n";
        }
    }

    if (!anyDeadlock) {
        cout << "No deadlock detected.\n";
        return;
    }

    if (!doRecovery) return;

    // Simple recovery: terminate each deadlocked process and release its allocation.
    cout << "\nRecovering by terminating deadlocked processes and reclaiming resources...\n";
    for (int i = 0; i < N; ++i) {
        if (!Finish[i]) {
            cout << "Terminating P" << i << " and releasing: [";
            for (int j = 0; j < M; ++j) {
                cout << Allocation[i][j] << (j + 1 < M ? " " : "");
                Available[j] += Allocation[i][j];
                Allocation[i][j] = 0;
                Request[i][j] = 0;
            }
            cout << "]\n";
            Finish[i] = true;
        }
    }

    cout << "Updated Available: [";
    for (int j = 0; j < M; ++j) cout << Available[j] << (j + 1 < M ? " " : "");
    cout << "]\n";
}

int main() {
    // Example dataset (tweak to demo deadlock/no-deadlock).
    // Allocation: resources currently held by each process
    int Allocation[N][M] = {
        {0, 1, 0},  // P0
        {2, 0, 0},  // P1
        {3, 0, 3},  // P2
        {2, 1, 1},  // P3
        {0, 0, 2}   // P4
    };

    // Request: additional resources each process still needs
    int Request[N][M] = {
        {0, 0, 0},  // P0
        {2, 0, 2},  // P1
        {0, 0, 0},  // P2
        {1, 0, 0},  // P3
        {0, 0, 2}   // P4
    };

    // Available: free instances of each resource type
    int Available[M] = {0, 0, 0};

    cout << "Deadlock Detection (matrix-based)\n";
    detectAndRecover(Allocation, Request, Available, /*doRecovery=*/true);
    return 0;
}
