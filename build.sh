#	A build script for the Pipeline simulation project

g++ -std=c++11 src/dataStructures.cpp src/main.cpp

./a.out 1 4 1 10 traceFiles/gcc-1k-regs.trace > output/gcc-1k-regs.output

./a.out 1 4 80 105 traceFiles/gcc-10k-regs-a.trace > output/gcc-10k-regs-a.output

./a.out 1 4 1 10 traceFiles/gcc-10k-regs-b.trace > output/gcc-10k-regs-b.output

./a.out 1 4 1 10 traceFiles/gcc-10k-regs-c.trace > output/gcc-10k-regs-c.output

./a.out 1 4 1 10 traceFiles/gcc-10k-regs-d.trace > output/gcc-10k-regs-d.output

./a.out 1 4 1 10 traceFiles/random-regs-10K.trace > output/random-regs-10K.output