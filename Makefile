# Makefile for CPU Scheduler

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = scheduler
OBJS = main.o scheduler.o fcfs.o sjf.o stcf.o rr.o mlfq.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c process.h scheduler.h
	$(CC) $(CFLAGS) -c main.c

scheduler.o: scheduler.c process.h scheduler.h
	$(CC) $(CFLAGS) -c scheduler.c

fcfs.o: fcfs.c process.h scheduler.h
	$(CC) $(CFLAGS) -c fcfs.c

sjf.o: sjf.c process.h scheduler.h
	$(CC) $(CFLAGS) -c sjf.c

stcf.o: stcf.c process.h scheduler.h
	$(CC) $(CFLAGS) -c stcf.c

rr.o: rr.c process.h scheduler.h
	$(CC) $(CFLAGS) -c rr.c

mlfq.o: mlfq.c process.h scheduler.h
	$(CC) $(CFLAGS) -c mlfq.c

clean:
	rm -f $(TARGET) $(OBJS) *.json *.png

# Test targets
test-fcfs:
	./$(TARGET) fcfs -f processes.txt -o fcfs_output.json

test-sjf:
	./$(TARGET) sjf -f processes.txt -o sjf_output.json

test-stcf:
	./$(TARGET) stcf -f processes.txt -o stcf_output.json

test-rr:
	./$(TARGET) rr 4 -f processes.txt -o rr_output.json

test-mlfq:
	./$(TARGET) mlfq -f processes.txt -o mlfq_output.json

test-all:
	./$(TARGET) all -f processes.txt

# Generate random processes
test-random:
	./$(TARGET) all -r 10 -o random_output.json

# Visualize results
visualize:
	python3 visualize.py output.json

compare:
	python3 visualize.py -c output_FCFS.json output_SJF.json output_STCF.json output_RR.json output_MLFQ.json

.PHONY: all clean test-fcfs test-sjf test-stcf test-rr test-mlfq test-all test-random visualize compare