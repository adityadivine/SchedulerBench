# SchedulerBench

SchedulerBench is a CPU process scheduling simulator written in C++. The goal of this project is to explore how different CPU scheduling algorithms behave under the same workload, instead of assuming a single algorithm is always optimal.

The simulator schedules processes based on their arrival time, burst time, and priority, while tracking key performance metrics such as waiting time, turnaround time, response time, and fairness.

## Features

* Simulates multiple CPU scheduling algorithms
* Supports arrival time, burst time, and priority-based processes
* Includes context switch overhead for realistic scheduling behavior
* Implements adaptive time quantum for Round Robin scheduling
* Generates Gantt charts to visualize CPU execution
* Compares schedulers using performance, responsiveness, and fairness metrics

## Scheduling Algorithms Implemented

* First Come First Serve (FCFS)
* Shortest Job First (SJF)
* Shortest Remaining Time First (SRTF)
* Preemptive Priority Scheduling
* Adaptive Round Robin Scheduling

## Evaluation Metrics

Each scheduler is evaluated using a combination of:

* Average turnaround time
* Average waiting time
* Average response time
* Fairness based on waiting time variation

A combined score is calculated to objectively compare scheduler performance.

## Input Format

```
n
arrival_time burst_time priority
arrival_time burst_time priority
...
```

## How to Run

Compile and run using a standard C++ compiler:

```
g++ scheduler.cpp -o scheduler
./scheduler
```

## Purpose

This project was built as a learning and experimentation tool for understanding CPU scheduling in operating systems. It goes beyond basic textbook examples by incorporating realistic constraints and comparing algorithms across multiple dimensions.
