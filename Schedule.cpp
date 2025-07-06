#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
using namespace std;

struct Process {
    string pid;
    int arrival, burst, remaining, priority;
    int completion, waiting, turnaround, start;
    bool done = false;
};

// Sort by arrival time
bool arrivalCmp(Process a, Process b) {
    return a.arrival < b.arrival;
}

// FCFS
void FCFS(vector<Process> p) {
    cout << "\n=== FCFS Scheduling ===\n";
    sort(p.begin(), p.end(), arrivalCmp);
    int time = 0;
    float totalWT = 0, totalTAT = 0;

    cout << "Gantt Chart: ";
    for (auto& pr : p) {
        time = max(time, pr.arrival);
        pr.start = time;
        time += pr.burst;
        pr.completion = time;
        pr.turnaround = pr.completion - pr.arrival;
        pr.waiting = pr.turnaround - pr.burst;

        totalWT += pr.waiting;
        totalTAT += pr.turnaround;

        cout << "[" << pr.pid << "] ";
    }

    cout << "\n\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto& pr : p) {
        cout << pr.pid << "\t" << pr.arrival << "\t" << pr.burst << "\t"
             << pr.completion << "\t" << pr.turnaround << "\t" << pr.waiting << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage TAT: " << totalTAT / p.size();
    cout << "\nAverage WT : " << totalWT / p.size() << "\n";
}

// SJF (Non-preemptive)
void SJF(vector<Process> p) {
    cout << "\n=== SJF Scheduling ===\n";
    int time = 0, completed = 0, n = p.size();
    float totalWT = 0, totalTAT = 0;
    vector<Process> order;

    while (completed < n) {
        int idx = -1, minBurst = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].arrival <= time && p[i].burst < minBurst) {
                minBurst = p[i].burst;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].start = time;
        time += p[idx].burst;
        p[idx].completion = time;
        p[idx].turnaround = p[idx].completion - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        p[idx].done = true;

        totalWT += p[idx].waiting;
        totalTAT += p[idx].turnaround;
        order.push_back(p[idx]);
        completed++;
    }

    cout << "Gantt Chart: ";
    for (auto& pr : order) cout << "[" << pr.pid << "] ";

    cout << "\n\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto& pr : order) {
        cout << pr.pid << "\t" << pr.arrival << "\t" << pr.burst << "\t"
             << pr.completion << "\t" << pr.turnaround << "\t" << pr.waiting << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage TAT: " << totalTAT / n;
    cout << "\nAverage WT : " << totalWT / n << "\n";
}

// Priority Scheduling (Non-preemptive)
void PriorityScheduling(vector<Process> p) {
    cout << "\n=== Priority Scheduling ===\n";
    int time = 0, completed = 0, n = p.size();
    float totalWT = 0, totalTAT = 0;
    vector<Process> order;

    while (completed < n) {
        int idx = -1, bestPriority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].arrival <= time && p[i].priority < bestPriority) {
                bestPriority = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].start = time;
        time += p[idx].burst;
        p[idx].completion = time;
        p[idx].turnaround = p[idx].completion - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        p[idx].done = true;

        totalWT += p[idx].waiting;
        totalTAT += p[idx].turnaround;
        order.push_back(p[idx]);
        completed++;
    }

    cout << "Gantt Chart: ";
    for (auto& pr : order) cout << "[" << pr.pid << "] ";

    cout << "\n\nPID\tAT\tBT\tPri\tCT\tTAT\tWT\n";
    for (auto& pr : order) {
        cout << pr.pid << "\t" << pr.arrival << "\t" << pr.burst << "\t"
             << pr.priority << "\t" << pr.completion << "\t"
             << pr.turnaround << "\t" << pr.waiting << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage TAT: " << totalTAT / n;
    cout << "\nAverage WT : " << totalWT / n << "\n";
}

// Round Robin
void RoundRobin(vector<Process> p, int quantum) {
    cout << "\n=== Round Robin (Q=" << quantum << ") ===\n";
    sort(p.begin(), p.end(), arrivalCmp);
    int time = 0, completed = 0, n = p.size();
    queue<int> q;
    vector<bool> inQueue(n, false);
    vector<Process> order;

    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    q.push(0);
    inQueue[0] = true;

    while (completed < n) {
        if (q.empty()) {
            time++;
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && !p[i].done && p[i].arrival <= time) {
                    q.push(i);
                    inQueue[i] = true;
                    break;
                }
            }
            continue;
        }

        int idx = q.front(); q.pop();

        if (p[idx].remaining > 0) {
            if (p[idx].start == -1) p[idx].start = time;

            int exec = min(quantum, p[idx].remaining);
            time += exec;
            p[idx].remaining -= exec;

            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && !p[i].done && p[i].arrival <= time) {
                    q.push(i);
                    inQueue[i] = true;
                }
            }

            if (p[idx].remaining == 0) {
                p[idx].completion = time;
                p[idx].turnaround = time - p[idx].arrival;
                p[idx].waiting = p[idx].turnaround - p[idx].burst;
                p[idx].done = true;
                completed++;
            } else {
                q.push(idx);
            }

            order.push_back(p[idx]);
        }
    }

    cout << "Gantt Chart: ";
    for (auto& pr : order) cout << "[" << pr.pid << "] ";

    float totalWT = 0, totalTAT = 0;
    cout << "\n\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto& pr : p) {
        totalWT += pr.waiting;
        totalTAT += pr.turnaround;
        cout << pr.pid << "\t" << pr.arrival << "\t" << pr.burst << "\t"
             << pr.completion << "\t" << pr.turnaround << "\t" << pr.waiting << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage TAT: " << totalTAT / n;
    cout << "\nAverage WT : " << totalWT / n << "\n";
}

// Main
int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        cout << "Enter PID, Arrival, Burst, Priority: ";
        cin >> p[i].pid >> p[i].arrival >> p[i].burst >> p[i].priority;
        p[i].start = -1;
    }

    FCFS(p);
    SJF(p);
    PriorityScheduling(p);

    int q;
    cout << "\nEnter quantum for Round Robin: ";
    cin >> q;
    RoundRobin(p, q);

    return 0;
}
