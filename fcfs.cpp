#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct PCB {
    int id, at, bt, st = 0, ct = 0, wt = 0, tat = 0;
    PCB(int i, int a, int b) { 
        id = i; 
        at = a; 
        bt = b; 
    }
};

void fcfs(vector<PCB>& p) {
    // Sort processes by arrival time
    sort(p.begin(), p.end(), [](PCB a, PCB b){ return a.at < b.at; });

    int t = 0, idle = 0;

    for (auto &x : p) {
        if (t < x.at) {
            idle += x.at - t;
            t = x.at;
        }
        x.st = t;
        x.ct = t + x.bt;
        x.tat = x.ct - x.at;
        x.wt = x.tat - x.bt;
        t += x.bt;
    }

    double util = (double)(t - idle) / t * 100;
    cout << "CPU Utilization: " << util << "%\n";
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
    vector<PCB> p = { PCB(1, 0, 5), PCB(2, 1, 3), PCB(3, 2, 8), PCB(4, 3, 6) };
    fcfs(p);
    print(p);
    return 0;
}
