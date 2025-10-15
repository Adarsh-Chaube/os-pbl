#include <bits/stdc++.h>
using namespace std;

struct PCB {
    int id, at, bt, st = 0, ct = 0, wt = 0, tat = 0;
    PCB(int i, int a, int b) { 
        id = i; 
        at = a; 
        bt = b; 
    }
};

void sjf(vector<PCB>& p) {
    int n = p.size(), t = 0, idle = 0, completed = 0;
    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1, minbt = INT_MAX;

        // Find the process with the minimum burst time that has arrived
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= t && p[i].bt < minbt) {
                idx = i;
                minbt = p[i].bt;
            }
        }

        if (idx == -1) { 
            t++; 
            continue; 
        }

        if (t < p[idx].at) {
            idle += p[idx].at - t;
            t = p[idx].at;
        }

        p[idx].st = t;
        p[idx].ct = t + p[idx].bt;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;

        t += p[idx].bt;
        done[idx] = true;
        completed++;
    }

    double util = (double)(t - idle) / t * 100;
    cout << "SJF CPU Utilization: " << util << "%\n";
}

void print(vector<PCB>& p) {
    double tw = 0, tt = 0;
    cout << "ID\tAT\tBT\tWT\tTAT\n";
    for (auto &x : p) { 
        cout << x.id << "\t" << x.at << "\t" << x.bt << "\t" << x.wt << "\t" << x.tat << "\n"; 
        tw += x.wt; 
        tt += x.tat; 
    }
    cout << "Avg WT: " << tw / p.size() << ", Avg TAT: " << tt / p.size() << "\n";
}

int main() {
    vector<PCB> p = {PCB(1, 0, 5), PCB(2, 1, 3), PCB(3, 2, 8), PCB(4, 3, 6)};
    sjf(p); 
    print(p);
}

