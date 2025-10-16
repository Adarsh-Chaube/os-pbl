#include <bits/stdc++.h>
using namespace std;

struct PCB {
    int id, at, bt, pr, st = 0, ct = 0, wt = 0, tat = 0;
    PCB(int i, int a, int b, int pri)    { 
        id = i;         
        at = a; 
        bt = b; 
        pr = pri; 
    }
};

void prioritySched(vector<PCB>& p, vector<int>& order) {
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

        // If no process has arrived yet, CPU is idle
        if (idx == -1) { 
            t++; 
            continue; 
        }

        // Adjust idle time if needed
        if (t < p[idx].at) {
            idle += p[idx].at - t;
            t = p[idx].at;
        }

        // Process execution
        p[idx].st = t;
        p[idx].ct = t + p[idx].bt;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;

        t += p[idx].bt;
        done[idx] = true;
        completed++;

        // Store execution order
        order.push_back(p[idx].id);
    }

    double util = (double)(t - idle) / t * 100;
    cout << "\nPriority CPU Utilization: " << util << "%\n";
}

void print(vector<PCB>& p) {
    double tw = 0, tt = 0;
    cout << "\nID\tAT\tBT\tPR\tWT\tTAT\n";
    for (auto &x : p) { 
        cout << x.id << "\t" << x.at << "\t" << x.bt << "\t" << x.pr 
             << "\t" << x.wt << "\t" << x.tat << "\n"; 
        tw += x.wt; 
        tt += x.tat; 
    }
    cout << "\nAvg WT: " << tw / p.size() << ", Avg TAT: " << tt / p.size() << "\n";
}

void displaySimpleGanttChart(vector<PCB>& p, vector<int>& order) {
    cout << "\nGantt Chart:\n";

    // Display process order
    for (size_t i = 0; i < order.size(); i++) {
        cout << "P" << order[i];
        if (i != order.size() - 1) cout << " -> ";
    }

    // Display timeline
    cout << "\n\nTimeline:\n";
    // Find process by ID to get completion times
    int start = p[order[0] - 1].st;
    cout << start;
    for (auto id : order) {
        int end = p[id - 1].ct;
        cout << "   " << end;
    }
    cout << "\n";
}

int main() {
    vector<PCB> p = {
        PCB(1, 0, 5, 2), 
        PCB(2, 1, 3, 1), 
        PCB(3, 2, 8, 4), 
        PCB(4, 3, 6, 3)
    };

    vector<int> order;
    prioritySched(p, order); 
    print(p);
    displaySimpleGanttChart(p, order);
    return 0;
}
    
