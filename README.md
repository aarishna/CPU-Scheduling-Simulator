# CPU Scheduling Simulator

A comprehensive C implementation of five CPU scheduling algorithms with automated Gantt chart visualization.

## Features

- **Five Scheduling Algorithms:**
  - FCFS (First Come First Served) - Non-preemptive
  - SJF (Shortest Job First) - Non-preemptive
  - STCF (Shortest Time-to-Completion First) - Preemptive
  - Round Robin - Preemptive with configurable time quantum
  - MLFQ (Multi-Level Feedback Queue) - Preemptive with 3 priority levels

- **Comprehensive Metrics:**
  - Average Waiting Time
  - Average Turnaround Time
  - Average Response Time
  - CPU Utilization
  - Throughput

- **Visualization:**
  - Automated Gantt chart generation using Python/Matplotlib
  - Algorithm comparison charts
  - JSON output for further analysis

## Project Structure

```
├── main.c              # Main program
├── scheduler.c         # Utility functions
├── scheduler.h         # Scheduler header
├── process.h           # Process structure definitions
├── fcfs.c              # FCFS algorithm
├── sjf.c               # SJF algorithm
├── stcf.c              # STCF algorithm
├── rr.c                # Round Robin algorithm
├── mlfq.c              # MLFQ algorithm
├── visualize.py        # Python visualization script
├── processes.txt       # Sample input file
├── Makefile            # Build configuration
└── README.md           # This file
```

## Installation

### Requirements

- GCC compiler (or any C99-compatible compiler)
- Python 3.x
- Matplotlib library for Python

```bash
# Install Python dependencies
pip install matplotlib numpy
```

## Building

```bash
# Compile the project
make

# Clean build files
make clean
```

## Usage

### Input File Format

The input file should contain:
- First line: Number of processes
- Following lines: `PID ArrivalTime BurstTime Priority`

Example (`processes.txt`):
```
5
1 0 10 3
2 1 5 1
3 2 8 2
4 3 6 4
5 4 4 5
```

### Running Algorithms

```bash
# Run FCFS
./scheduler fcfs -f processes.txt

# Run SJF
./scheduler sjf -f processes.txt

# Run STCF
./scheduler stcf -f processes.txt

# Run Round Robin with time quantum 4
./scheduler rr 4 -f processes.txt

# Run MLFQ
./scheduler mlfq -f processes.txt

# Run all algorithms for comparison
./scheduler all -f processes.txt
```

### Using Random Process Generation

```bash
# Generate 10 random processes
./scheduler all -r 10

# Generate 20 random processes with specific algorithm
./scheduler rr 4 -r 20
```

### Visualization

```bash
# Visualize single algorithm results
python3 visualize.py output.json

# Compare multiple algorithms
python3 visualize.py -c output_FCFS.json output_SJF.json output_STCF.json output_RR.json output_MLFQ.json
```

### Using Makefile Targets

```bash
# Test individual algorithms
make test-fcfs
make test-sjf
make test-stcf
make test-rr
make test-mlfq

# Test all algorithms
make test-all

# Test with random processes
make test-random

# Visualize results
make visualize

# Compare all algorithms
make compare
```

## Algorithm Details

### FCFS (First Come First Served)
- **Type:** Non-preemptive
- **Complexity:** O(n)
- **Best for:** Batch processing systems
- **Characteristics:** Simple, fair arrival order, can cause convoy effect

### SJF (Shortest Job First)
- **Type:** Non-preemptive
- **Complexity:** O(n²)
- **Best for:** Minimizing average waiting time
- **Characteristics:** Optimal for average waiting time, can cause starvation

### STCF (Shortest Time-to-Completion First)
- **Type:** Preemptive (Preemptive SJF)
- **Complexity:** O(n²)
- **Best for:** Minimizing average turnaround time
- **Characteristics:** Optimal for average turnaround time, more context switches

### Round Robin
- **Type:** Preemptive
- **Complexity:** O(n)
- **Best for:** Time-sharing systems
- **Characteristics:** Fair, configurable quantum, good response time
- **Default Quantum:** 4 time units

### MLFQ (Multi-Level Feedback Queue)
- **Type:** Preemptive
- **Complexity:** O(n log n)
- **Best for:** Mixed workloads (CPU-bound and I/O-bound)
- **Characteristics:** 
  - 3 priority queues (Q0, Q1, Q2)
  - Q0: Time quantum = 8, highest priority
  - Q1: Time quantum = 16, medium priority
  - Q2: FCFS, lowest priority
  - Priority boost every 100 time units to prevent starvation

## Performance Analysis

### Metrics Explanation

1. **Waiting Time:** Time a process spends in ready queue
2. **Turnaround Time:** Total time from arrival to completion
3. **Response Time:** Time from arrival to first execution
4. **CPU Utilization:** Percentage of time CPU is executing processes
5. **Throughput:** Number of processes completed per time unit

### Expected Performance

- **FCFS:** Simple but poor for short jobs
- **SJF:** Best average waiting time, but can starve long jobs
- **STCF:** Best average turnaround time, higher overhead
- **Round Robin:** Good response time, fair scheduling
- **MLFQ:** Balances response and turnaround time

## Example Output

```
=== Process Table ===
PID | Arrival | Burst | Completion | Turnaround | Waiting | Response
----+---------+-------+------------+------------+---------+---------
P1  |    0    |  10   |     10     |     10     |    0    |    0
P2  |    1    |   5   |     15     |     14     |    9    |    9
...

=== Scheduling Metrics ===
Average Waiting Time:     8.40
Average Turnaround Time:  14.60
Average Response Time:    7.20
CPU Utilization:          97.14%
Throughput:               0.1429 processes/unit time
Total Time:               35
```

## Testing Different Workloads

### CPU-Bound Workload
```
# Long burst times, minimal I/O
5
1 0 50 1
2 5 45 2
3 10 40 3
4 15 35 4
5 20 30 5
```

### I/O-Bound Workload
```
# Short burst times, frequent I/O
5
1 0 5 1
2 2 3 2
3 4 4 3
4 6 2 4
5 8 3 5
```

### Mixed Workload
```
# Combination of CPU and I/O bound
6
1 0 40 1
2 5 3 2
3 10 25 3
4 15 5 4
5 20 30 5
6 25 4 6
```

## Advanced Features

### Modular Design
- O(n log n) priority-based scheduling using efficient queues
- Extensible architecture for adding new algorithms
- Separated concerns (scheduling logic, metrics, visualization)

### JSON Output
All results are exported to JSON format for:
- Further analysis in other tools
- Integration with web dashboards
- Automated testing and benchmarking

## Troubleshooting

### Compilation Errors
```bash
# Ensure all files are present
ls *.c *.h

# Check GCC version
gcc --version

# Compile with verbose output
make clean && make
```

### Python Visualization Issues
```bash
# Install/upgrade matplotlib
pip install --upgrade matplotlib numpy

# Test Python installation
python3 -c "import matplotlib; print(matplotlib.__version__)"
```

## Contributing

To add a new scheduling algorithm:
1. Create a new file `algorithm.c`
2. Implement the function: `void algorithm_schedule(Process *processes, int n, GanttChart *chart)`
3. Add the function declaration to `scheduler.h`
4. Update `main.c` to include the new algorithm
5. Update the Makefile

## License

This project is open-source and available for educational purposes.

## Author

CPU Scheduling Simulator - A comprehensive educational tool for understanding operating system scheduling algorithms.
