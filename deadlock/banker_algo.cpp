#include <iostream>
using namespace std;

const int N = 5; // number of processes
const int M = 3; // number of resource types

void computeNeed(int Max[N][M], int Allocation[N][M], int Need[N][M]) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            Need[i][j] = Max[i][j] - Allocation[i][j];
}

bool safetyCheck(int Available[M], int Max[N][M], int Allocation[N][M],
                 int Need[N][M], int safeSeq[N]) {
    int Work[M];
    bool Finish[N] = {false};

    for (int j = 0; j < M; ++j)
        Work[j] = Available[j];

    int count = 0;
    while (count < N) {
        bool found = false;
        for (int i = 0; i < N; ++i) {
            if (!Finish[i]) {
                bool canRun = true;
                for (int j = 0; j < M; ++j) {
                    if (Need[i][j] > Work[j]) {
                        canRun = false;
                        break;
                    }
                }
                if (canRun) {
                    for (int j = 0; j < M; ++j)
                        Work[j] += Allocation[i][j];
                    safeSeq[count++] = i;
                    Finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found)
            return false;
    }
    return true;
}

bool requestResources(int p, int Request[M],
                      int Available[M], int Max[N][M], int Allocation[N][M]) {
    int Need[N][M];
    computeNeed(Max, Allocation, Need);

    for (int j = 0; j < M; ++j)
        if (Request[j] > Need[p][j]) {
            cout << "Request exceeds process " << p << " need.\n";
            return false;
        }

    for (int j = 0; j < M; ++j)
        if (Request[j] > Available[j]) {
            cout << "Insufficient available resources. Process " << p << " must wait.\n";
            return false;
        }

    for (int j = 0; j < M; ++j) {
        Available[j]      -= Request[j];
        Allocation[p][j]  += Request[j];
        Need[p][j]        -= Request[j];
    }

    int safeSeq[N];
    bool safe = safetyCheck(Available, Max, Allocation, Need, safeSeq);

    if (safe) {
        cout << "Request granted. System remains in a safe state.\n";
        cout << "Safe sequence: ";
        for (int i = 0; i < N; ++i)
            cout << "P" << safeSeq[i] << (i + 1 < N ? " -> " : "\n");
        return true;
    } else {
        for (int j = 0; j < M; ++j) {
            Available[j]      += Request[j];
            Allocation[p][j]  -= Request[j];
            Need[p][j]        += Request[j];
        }
        cout << "Request denied. Granting it would lead to an unsafe state.\n";
        return false;
    }
}

int main() {
    int Available[M] = {3, 3, 2};

    int Max[N][M] = {
        {7, 5, 3},  // P0
        {3, 2, 2},  // P1
        {9, 0, 2},  // P2
        {2, 2, 2},  // P3
        {4, 3, 3}   // P4
    };

    int Allocation[N][M] = {
        {0, 1, 0},  // P0
        {2, 0, 0},  // P1
        {3, 0, 2},  // P2
        {2, 1, 1},  // P3
        {0, 0, 2}   // P4
    };

    int Need[N][M];
    computeNeed(Max, Allocation, Need);

    int safeSeq[N];
    bool safe = safetyCheck(Available, Max, Allocation, Need, safeSeq);

    cout << "Initial safety check: " << (safe ? "SAFE" : "UNSAFE") << "\n";
    if (safe) {
        cout << "Safe sequence: ";
        for (int i = 0; i < N; ++i)
            cout << "P" << safeSeq[i] << (i + 1 < N ? " -> " : "\n");
    }

    cout << "\nTrying request for P1: [1,0,2]\n";
    int Request[M] = {1, 0, 2};
    requestResources(1, Request, Available, Max, Allocation);

    return 0;
}
