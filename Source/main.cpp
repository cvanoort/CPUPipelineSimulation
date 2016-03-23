/*
	Colin Van Oort
	CS 222: Computer Architecture
	Pipeline Simulation Project
	main.cpp
*/
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>

#include "dataStructures.hpp"
#include "globals.hpp"

int main( int argc, char* argv[] ){
	std::cout << "BEGIN SIMULATION: Basic Pipeline simulation" << std::endl;

	if( argc != 6 ){
		std::cout << "ERROR: Incorrect number of command line arguments, 5 expected!" << std::endl;
		exit( EXIT_FAILURE );
	}
	
	// Print the command line arguments to make sure we have the right ones
	std::cout << "Operands Cycle: " << argv[1] << ", Results Cycle: " << argv[2] << ", Starting Instruction: " << argv[3] << ", Ending Instruction: " << argv[4] <<", Trace File: " << argv[5] << std::endl;

	int opCycle = atoi( argv[1] );
	int resCycle = atoi( argv[2] );
	int startExChart = atoi( argv[3] );
	int endExChart = atoi( argv[4] );
	// Create an input stream from the trace file and an output stream to the output file
	std::ifstream tracefile( argv[5] );
	
	if( !tracefile.is_open() ){
		std::cout << "ERROR: Trace file failed to open, please make sure to give a vaild path!";
		exit( EXIT_FAILURE );
	}
	
	else{
		std::cout << "Trace file opened successfully" << std::endl;
	}

	// Some variables used by the simulation loop
	std::string line;
	int op1,op2,result;
	int instructNum = 0;
	int cycleNum = 0;
	int stallNum = 0;
	PipelineStatusArray pipe = PipelineStatusArray( g_pipelineDepth );
	RegisterStatusArray regs = RegisterStatusArray( g_numRegs );

	// Main simulation loop
	while( std::getline(tracefile, line) ){
		instructNum++;

		std::istringstream iss(line);
		iss >> op1 >> op2 >> result;

		if( pipe.getStatus(resCycle) ){
			pipe.setInstrEx( pipe.getElement(resCycle).idNum );
		}

		pipe.advancePipeline( resCycle );

		if( pipe.getStatus(resCycle) ){
			regs.writeBack( pipe.getElement(resCycle) );
		}

		pipe.addInstruction( ProcessInfo(instructNum, op1, op2, result, false) );

		// check and set stalls
		pipe.setStall( opCycle, regs.decode(pipe.getElement(opCycle)) );

		pipe.print( cycleNum, startExChart, endExChart );

		cycleNum++;

		// Handle stalls
		while( pipe.getStall(opCycle) ){
			if( pipe.getStatus(resCycle) ){
				pipe.setInstrEx( pipe.getElement(resCycle).idNum );
			}

			pipe.advancePipeline( resCycle );

			if( pipe.getStatus(resCycle) ){
				regs.writeBack( pipe.getElement(resCycle) );
			}

			// check and set stalls
			pipe.setStall( opCycle, regs.decode(pipe.getElement(opCycle)) );

			pipe.print( cycleNum, startExChart, endExChart );

			cycleNum++;
			stallNum++;
		}
	}
	
	// Finish executing the instructions in the pipeline
	while( !pipe.isEmpty() ){
		if( pipe.getStatus(resCycle) ){
			pipe.setInstrEx( pipe.getElement(resCycle).idNum );
		}

		pipe.advancePipeline( resCycle );

		if( pipe.getStatus(resCycle) ){
			regs.writeBack( pipe.getElement(resCycle) );
		}

		// check and set stalls
		pipe.setStall( opCycle, regs.decode(pipe.getElement(opCycle)) );

		pipe.print( cycleNum, startExChart, endExChart );

		cycleNum++;

		// Handle stalls
		if( pipe.getStall(opCycle) ){
			stallNum++;
		}
	}

	double noPipeline = g_pipelineDepth*instructNum;
	double speedup = 100*(noPipeline/cycleNum);

	std::cout << "Analysis of the simulation: " << std::endl;
	std::cout << "Number of instructions executed: " << instructNum << std::endl;
	std::cout << "Expected Number of cycles when unpipelined: " << noPipeline << std::endl;
	std::cout << "Number of stall cycles: " << stallNum << std::endl;
	std::cout << "Number of cycles in the pipeline simulation: " << cycleNum << std::endl;
	std::cout << "Average number of stalls per instruction: " << double(stallNum)/instructNum << std::endl;
	std::cout << "Speedup: " << speedup << "%" << std::endl;

	std::cout << "End SIMULATION" << std::endl;
	
	tracefile.close();
	
	return 1;
} // End main