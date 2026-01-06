#include <bits/stdc++.h>
using namespace std;

struct Process {
	int pid;
	int arrival;
	int burst;
	int remaining;
	int priority;

	int start = -1;
	int completion = 0;
	int turnaround = 0;
	int waiting = 0;
	int response = 0;
};

struct Gantt {
	int pid;
	int start;
	int end;
};

const int CONTEXT_SWITCH = 1;

vector<Process> readProcesses() {
	int n;
	cin >> n;
	vector<Process> p(n);
	for(int i = 0; i < n; i++) {
		p[i].pid = i + 1;
		cin >> p[i].arrival >> p[i].burst >> p[i].priority;
		p[i].remaining = p[i].burst;
	}
	return p;
}

void finalizeMetrics(vector<Process>& p) {
	for(auto& x : p) {
		x.turnaround = x.completion - x.arrival;
		x.waiting = x.turnaround - x.burst;
		x.response = x.start - x.arrival;
	}
}

double avgTurnaround(vector<Process>& p) {
	double s = 0;
	for(auto& x : p) s += x.turnaround;
	return s / p.size();
}

double avgWaiting(vector<Process>& p) {
	double s = 0;
	for(auto& x : p) s += x.waiting;
	return s / p.size();
}

double avgResponse(vector<Process>& p) {
	double s = 0;
	for(auto& x : p) s += x.response;
	return s / p.size();
}

double fairness(vector<Process>& p) {
	double mean = avgWaiting(p);
	double var = 0;
	for(auto& x : p)
		var += (x.waiting - mean) * (x.waiting - mean);
	return sqrt(var / p.size());
}

void printGantt(vector<Gantt>& g) {
	cout << "\nGantt Chart:\n";
	for(auto& x : g)
		cout << "| P" << x.pid << " ";
	cout << "|\n0";
	for(auto& x : g)
		cout << setw(5) << x.end;
	cout << "\n";
}

double score(vector<Process>& p) {
	return 0.4 * avgTurnaround(p)
	     + 0.25 * avgWaiting(p)
	     + 0.25 * avgResponse(p)
	     + 0.1 * fairness(p);
}

/* Priority Scheduling Preemptive */
double PriorityScheduling(vector<Process> p) {
	vector<Gantt> g;
	int time = 0, done = 0, prev = -1;
	int n = p.size();

	while(done < n) {
		int idx = -1;
		int best = INT_MAX;

		for(int i = 0; i < n; i++) {
			if(p[i].arrival <= time && p[i].remaining > 0) {
				if(p[i].priority < best) {
					best = p[i].priority;
					idx = i;
				}
			}
		}

		if(idx == -1) {
			time++;
			continue;
		}

		if(prev != idx) {
			if(prev != -1) time += CONTEXT_SWITCH;
			g.push_back({p[idx].pid, time, time + 1});
		}

		if(p[idx].start == -1)
			p[idx].start = time;

		p[idx].remaining--;
		time++;
		g.back().end = time;

		if(p[idx].remaining == 0) {
			p[idx].completion = time;
			done++;
		}
		prev = idx;
	}

	finalizeMetrics(p);
	printGantt(g);
	return score(p);
}

/* Adaptive Round Robin */
double AdaptiveRR(vector<Process> p) {
	vector<Gantt> g;
	queue<int> q;
	int time = 0, done = 0, prev = -1;
	int n = p.size();
	vector<bool> inQ(n, false);

	while(done < n) {
		for(int i = 0; i < n; i++) {
			if(p[i].arrival <= time && p[i].remaining > 0 && !inQ[i]) {
				q.push(i);
				inQ[i] = true;
			}
		}

		if(q.empty()) {
			time++;
			continue;
		}

		int sum = 0, cnt = 0;
		queue<int> temp = q;
		while(!temp.empty()) {
			sum += p[temp.front()].remaining;
			cnt++;
			temp.pop();
		}
		int tq = max(1, sum / cnt);

		int idx = q.front();
		q.pop();
		inQ[idx] = false;

		if(prev != idx) {
			if(prev != -1) time += CONTEXT_SWITCH;
			g.push_back({p[idx].pid, time, time});
		}

		if(p[idx].start == -1)
			p[idx].start = time;

		int exec = min(tq, p[idx].remaining);
		p[idx].remaining -= exec;
		time += exec;
		g.back().end = time;

		if(p[idx].remaining > 0) {
			q.push(idx);
			inQ[idx] = true;
		} else {
			p[idx].completion = time;
			done++;
		}
		prev = idx;
	}

	finalizeMetrics(p);
	printGantt(g);
	return score(p);
}

int main() {
	auto processes = readProcesses();

	cout << "\nPriority Scheduling:\n";
	double ps = PriorityScheduling(processes);

	cout << "\nAdaptive Round Robin:\n";
	double rr = AdaptiveRR(processes);

	cout << "\nScores:\n";
	cout << "Priority Scheduler Score: " << ps << "\n";
	cout << "Adaptive RR Score: " << rr << "\n";

	if(ps < rr)
		cout << "Best Scheduler: Priority Scheduling\n";
	else
		cout << "Best Scheduler: Adaptive Round Robin\n";

	return 0;
}
