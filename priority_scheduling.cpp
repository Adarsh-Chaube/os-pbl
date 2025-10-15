#include <bits/stdc++.h>
using namespace std;

struct PCB {
    int id, at, bt, pr, st = 0, ct = 0, wt = 0, tat = 0;
    PCB(int i, int a, int b, int pri) { 
        id = i; 
        at = a; 
        bt = b; 
        pr = pri; 
    }
};

void prioritySched(vector<PCB>& p) {
    int n = p.size(), t = 0, idle = 0, completed = 0;
    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1, highPr = INT_MAX;

        // Find the process with the highest priority that has arrived
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= t && p[i].pr < highPr) {
                idx = i;
                highPr = p[i].pr;
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
    cout << "Priority CPU Utilization: " << util << "%\n";
}

void print(vector<PCB>& p) {
    double tw = 0, tt = 0;
    cout << "ID\tAT\tBT\tPR\tWT\tTAT\n";
    for (auto &x : p) { 
        cout << x.id << "\t" << x.at << "\t" << x.bt << "\t" << x.pr << "\t" << x.wt << "\t" << x.tat << "\n"; 
        tw += x.wt; 
        tt += x.tat; 
    }
    cout << "Avg WT: " << tw / p.size() << ", Avg TAT: " << tt / p.size() << "\n";
}

int main() {
    vector<PCB> p = {PCB(1, 0, 5, 2), PCB(2, 1, 3, 1), PCB(3, 2, 8, 4), PCB(4, 3, 6, 3)};
    prioritySched(p); 
    print(p);
}
