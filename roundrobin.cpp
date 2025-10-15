#include <bits/stdc++.h>
using namespace std;

struct PCB {
    int id, at, bt, st = 0, ct = 0, wt = 0, tat = 0, rem;
    PCB(int i, int a, int b) { 
        id = i; 
        at = a; 
        bt = b; 
        rem = b; 
    }
};

void roundRobin(vector<PCB>& p, int q) {
    int n = p.size(), t = 0, idle = 0, completed = 0;
    queue<int> rq; 
    vector<bool> inQueue(n, false);

    while (completed < n) {
        // Add processes that have arrived to the ready queue
        for (int i = 0; i < n; i++) 
            if (!inQueue[i] && p[i].at <= t && p[i].rem > 0) { 
                rq.push(i); 
                inQueue[i] = true; 
            }

        if (rq.empty()) { 
            t++; 
            continue; 
        }

        int i = rq.front(); 
        rq.pop();
        PCB &pr = p[i];

        if (pr.rem == pr.bt) 
            pr.st = t;

        int ex = min(q, pr.rem);

        if (t < pr.at) {
            idle += pr.at - t;
            t = pr.at;
        }

        t += ex;
        pr.rem -= ex;

        // Add newly arrived processes to the ready queue
        for (int j = 0; j < n; j++) 
            if (!inQueue[j] && p[j].at <= t && p[j].rem > 0) { 
                rq.push(j); 
                inQueue[j] = true; 
            }

        if (pr.rem > 0) 
            rq.push(i); 
        else { 
            pr.ct = t; 
            pr.tat = pr.ct - pr.at; 
            pr.wt = pr.tat - pr.bt; 
            completed++; 
        }
    }

    double util = (double)(t - idle) / t * 100;
    cout << "RR CPU Utilization: " << util << "%\n";
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
    roundRobin(p, 2);
    print(p);
}
