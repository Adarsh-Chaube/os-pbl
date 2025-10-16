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

void sjf(vector<PCB>& p, vector<int>& order) {
    int n = p.size(), t = 0, idle = 0, completed = 0;
    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1, minbt = INT_MAX;

        // Find process with minimum burst time that has arrived
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

        // Store execution order for Gantt chart
        order.push_back(p[idx].id);
    }

    double util = (double)(t - idle) / t * 100;
    cout << "\nSJF CPU Utilization: " << util << "%\n";
}

void print(vector<PCB>& p) {
    double tw = 0, tt = 0;
    cout << "\nID\tAT\tBT\tWT\tTAT\n";
    for (auto &x : p) { 
        cout << x.id << "\t" << x.at << "\t" << x.bt << "\t" << x.wt << "\t" << x.tat << "\n"; 
        tw += x.wt; 
        tt += x.tat; 
    }
    cout << "\nAvg WT: " << tw / p.size() << ", Avg TAT: " << tt / p.size() << "\n";
}

void displaySimpleGanttChartSJF(vector<PCB>& p, vector<int>& order) {
    cout << "\nGantt Chart (SJF):\n";
    for (size_t i = 0; i < order.size(); i++) {
        cout << "P" << order[i];
        if (i != order.size() - 1) cout << " -> ";
    }

    cout << "\n\nTimeline:\n";
    for (size_t i = 0; i < order.size(); i++) {
        int idx = order[i] - 1; // process id starts from 1
        cout << p[idx].st << "   ";
    }
    cout << p[order.back() - 1].ct << "\n";
}

int main() {
    vector<PCB> p = {PCB(1, 0, 5), PCB(2, 1, 3), PCB(3, 2, 8), PCB(4, 3, 6)};
    vector<int> order;
    sjf(p, order); 
    print(p);
    displaySimpleGanttChartSJF(p, order);
    return 0;
}
