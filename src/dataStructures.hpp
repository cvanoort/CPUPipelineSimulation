/*
	Colin Van Oort
	CS 222: Computer Architecture
	Pipeline Simulation Project
	dataStructures.hpp
	Header file for the data structures used in the project
*/

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>


struct ProcessInfo{
	int idNum, operand1, operand2, result;
	bool stall;

	ProcessInfo(): idNum(0), operand1(-1), operand2(-1), result(-1), stall(false) {}
	ProcessInfo(int id, int op1, int op2, int r, bool s): idNum(id), operand1(op1), operand2(op2), result(r), stall(s){}
};


class PipelineStatusArray{
	public:
		PipelineStatusArray( int size );
		bool getStatus( int index );
		void addInstruction( ProcessInfo p);
		void setInstrEx( int instrNum );
		void setStall( int index, bool stall );
		bool getStall( int index );
		ProcessInfo getElement( int index );
		void advancePipeline( int resultCycle );
		bool isEmpty();
		void print( int cycleNum, int startExChart, int stopExChart );

	private:
		std::vector<ProcessInfo> m_pipelineStatus;
		int m_size,m_instrEx;
		std::vector<std::string> m_stageID;
};


struct RegisterInfo{
	int lastWrite, nextWrite;

	RegisterInfo(): lastWrite(0), nextWrite(0) {}
};


class RegisterStatusArray{
	public:
		RegisterStatusArray( int size );
		void setLast( int index, int instructNum );
		void setNext( int index, int instructNum );
		int getLast( int index );
		int getNext( int index );
		void writeBack( ProcessInfo p );
		bool decode( ProcessInfo p ); 
		void print();

	private:
		std::vector<RegisterInfo> m_registerStatus;
		int m_size;
};