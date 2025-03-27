#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

// Process structure
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};

// Function to calculate metrics
void calculateMetrics(vector<Process>& processes) {
    int currentTime = 0;
    for (auto& p : processes) {
        // Completion time is max of current time and arrival time + burst time
        currentTime = max(currentTime, p.arrivalTime) + p.burstTime;
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
    }
}

// Function to display Gantt Chart
void displayGanttChart(const vector<Process>& processes) {
    cout << "\nGantt Chart:\n";
    for (const auto& p : processes) {
        cout << "|   P" << p.id << "   ";
    }
    cout << "|\n";

    int currentTime = 0;
    for (const auto& p : processes) {
        cout << setw(4) << currentTime << setw(8);
        currentTime = max(currentTime, p.arrivalTime) + p.burstTime;
    }
    cout << currentTime << "\n";
}

// Function to display the results
void displayResults(const vector<Process>& processes, const string& algorithmName) {
    cout << "\n--- " << algorithmName << " Scheduling Results ---\n";
    cout << setw(10) << "Process" << setw(15) << "Arrival Time" << setw(15)
         << "Burst Time" << setw(20) << "Completion Time" << setw(15)
         << "Turnaround Time" << setw(15) << "Waiting Time" << "\n";

    for (const auto& p : processes) {
        cout << setw(10) << "P" << p.id << setw(15) << p.arrivalTime
             << setw(15) << p.burstTime << setw(20) << p.completionTime
             << setw(15) << p.turnaroundTime << setw(15) << p.waitingTime << "\n";
    }

    double avgWaitingTime = 0, avgTurnaroundTime = 0;
    for (const auto& p : processes) {
        avgWaitingTime += p.waitingTime;
        avgTurnaroundTime += p.turnaroundTime;
    }
    avgWaitingTime /= processes.size();
    avgTurnaroundTime /= processes.size();

    cout << "\nAverage Waiting Time: " << avgWaitingTime << "\n";
    cout << "Average Turnaround Time: " << avgTurnaroundTime << "\n";

    displayGanttChart(processes);
}

// Function for FCFS scheduling
void fcfs(vector<Process>& processes) {
    // Sort by arrival time
    sort(processes.begin(), processes.end(),
         [](const Process& a, const Process& b) { return a.arrivalTime < b.arrivalTime; });

    calculateMetrics(processes);
    displayResults(processes, "FCFS");
}

// Function for SJF scheduling
void sjf(vector<Process>& processes) {
    // Sort by arrival time first, then burst time
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return (a.arrivalTime == b.arrivalTime) ? (a.burstTime < b.burstTime)
                                                : (a.arrivalTime < b.arrivalTime);
    });

    vector<Process> readyQueue;
    int currentTime = 0;

    while (!processes.empty()) {
        // Move all processes that have arrived into the ready queue
        for (auto it = processes.begin(); it != processes.end();) {
            if (it->arrivalTime <= currentTime) {
                readyQueue.push_back(*it);
                it = processes.erase(it);
            } else {
                ++it;
            }
        }

        // If ready queue is empty, move to the next process
        if (readyQueue.empty()) {
            currentTime = processes[0].arrivalTime;
            continue;
        }

        // Sort the ready queue by burst time
        sort(readyQueue.begin(), readyQueue.end(),
             [](const Process& a, const Process& b) { return a.burstTime < b.burstTime; });

        // Schedule the first process in the ready queue
        Process& nextProcess = readyQueue[0];
        currentTime = max(currentTime, nextProcess.arrivalTime) + nextProcess.burstTime;
        nextProcess.completionTime = currentTime;
        nextProcess.turnaroundTime = nextProcess.completionTime - nextProcess.arrivalTime;
        nextProcess.waitingTime = nextProcess.turnaroundTime - nextProcess.burstTime;

        processes.push_back(nextProcess); // Add back for output display
        readyQueue.erase(readyQueue.begin());
    }

    displayResults(processes, "SJF");
}

// Main function
int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    cout << "Enter Arrival Time and Burst Time for each process:\n";
    for (int i = 0; i < n; ++i) {
        processes[i].id = i + 1;
        cout << "Process P" << i + 1 << ":\n";
        cout << "Arrival Time: ";
        cin >> processes[i].arrivalTime;
        cout << "Burst Time: ";
        cin >> processes[i].burstTime;
    }

    vector<Process> fcfsProcesses = processes;
    vector<Process> sjfProcesses = processes;

    // Run FCFS
    fcfs(fcfsProcesses);

    // Run SJF
    sjf(sjfProcesses);

    return 0;
}
