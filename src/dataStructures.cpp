/*
	Colin Van Oort
	CS 222: Computer Architecture
	Pipeline Simulation Project
	dataStructures.cpp
	Contains the implemenation of the data structures used in the project
*/

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "dataStructures.hpp"
#include "globals.hpp"


// PipelineStatusArray Methods

PipelineStatusArray::PipelineStatusArray( int size ){
	m_size = size;
	m_instrEx = 0;
	m_stageID.push_back("IF");
	m_stageID.push_back("ID");
	m_stageID.push_back("EX");
	m_stageID.push_back("ME");
	m_stageID.push_back("WB");

	for( int i=0 ; i<m_size ; ++i ){
		m_pipelineStatus.push_back(ProcessInfo());
	}
} // End PipelineStatusArray Constructor


bool PipelineStatusArray::getStatus( int index ){
	if( index < m_size ){
		if( m_pipelineStatus[index].idNum != 0 )
			return true;
		else
			return false;
	} 

	else{
		std::cout << "ERROR: index out of bounds on pipelineStatus structure" << std::endl;
		exit( EXIT_FAILURE );
	}
} // End getStatus


ProcessInfo PipelineStatusArray::getElement(int index){
	if( index < m_size ){
		return m_pipelineStatus[index];
	} 

	else{
		std::cout << "ERROR: index out of bounds on pipelineStatus structure" << std::endl;
		exit( EXIT_FAILURE );
	}
} // End getElement


void PipelineStatusArray::addInstruction( ProcessInfo p ){
	if( m_pipelineStatus[0].idNum == 0 ){
		m_pipelineStatus[0] = p;
	}
	else{
		std::cout << "ERROR: IF is busy!" << std::endl;
		exit( EXIT_FAILURE );
	}
} // End setStatus


void PipelineStatusArray::setInstrEx( int instrNum ){
	m_instrEx = instrNum;
} // End setInstrEx


void PipelineStatusArray::setStall( int index, bool stall ){
	if( index < m_size ){
		m_pipelineStatus[index].stall = stall;
	}
} // End setStall


bool PipelineStatusArray::getStall( int index ){
	return m_pipelineStatus[index].stall;
} // End getStall


void PipelineStatusArray::advancePipeline( int resultCycle ){
	m_pipelineStatus[ resultCycle ] = ProcessInfo();
	for( int i=m_size-1;i>0;i-- ){
		if( !m_pipelineStatus[i-1].stall && m_pipelineStatus[i].idNum == 0 ){
			m_pipelineStatus[i] = m_pipelineStatus[i-1];
			m_pipelineStatus[i-1] = ProcessInfo();	
		}	
	}
} // End advancePipeline

bool PipelineStatusArray::isEmpty(){
	bool empty = true;
	for( int i=0;i<m_size-1;i++ ){
		if(m_pipelineStatus[i].idNum != 0 ){
			empty = false;
		}
	}
	return empty;
} // End isEmpty


void PipelineStatusArray::print( int cycleNum, int startExChart, int stopExChart ){
	if( (m_pipelineStatus[0].idNum >= startExChart) && (m_instrEx <= stopExChart) && (startExChart > 0) ){
		for(int i=0;i<=m_instrEx-startExChart;i++){
			std::cout << "   ";
		}

		for( int i=m_size-1 ; i>=0 ; i-- ){
			if( m_pipelineStatus[i].idNum != 0 ){
				if( (m_pipelineStatus[i].idNum >= startExChart) && (m_pipelineStatus[i].idNum <= stopExChart) ){
					if( m_pipelineStatus[i].stall ){
						std::cout << "** ";
						i = 0;
					}
					else{
						std::cout << m_stageID[i] << " ";
					}	
				}			
			}
		}
		std::cout << std::endl;
	}
} // End print


RegisterStatusArray::RegisterStatusArray( int size ){
	m_size = size;

	for( int i=0 ; i<m_size ; ++i){
		m_registerStatus.push_back(RegisterInfo());
	}
} // End RegisterStatusArray constructor


void RegisterStatusArray::setLast( int index, int instructNum ){
	
	m_registerStatus[index].lastWrite = instructNum;

} // End setLast


void RegisterStatusArray::setNext( int index, int instructNum ){

	m_registerStatus[index].nextWrite = instructNum;

} // End setNext


int RegisterStatusArray::getLast( int index ){
	return m_registerStatus[index].lastWrite;
} // End getLast


int RegisterStatusArray::getNext( int index ){

	return m_registerStatus[index].nextWrite;

} // End getNext


void RegisterStatusArray::writeBack( ProcessInfo p ){
	if( m_registerStatus[p.result].nextWrite == p.idNum ){
		m_registerStatus[p.result].lastWrite = p.idNum;
		m_registerStatus[p.result].nextWrite = 0;
	}
} // End writeBack


bool RegisterStatusArray::decode( ProcessInfo p ){	
	bool stall = false;

	// if one of p's operands has a process waiting to write to it, return true to stall
	if( p.operand1 != -1 ){
		if( m_registerStatus[p.operand1].nextWrite !=0 && m_registerStatus[p.operand1].nextWrite < p.idNum ){
			stall = true;
		}
	}
	if( p.operand2 != -1 ){
		if( m_registerStatus[p.operand2].nextWrite !=0 && m_registerStatus[p.operand2].nextWrite < p.idNum ){
			stall = true;
		}
	}

	// set the instruction number of process p to be the nextWrite of the result register of p
	if( p.result != -1 && !stall){
		m_registerStatus[p.result].nextWrite = p.idNum;
	}

	return stall;
} // End decode


void RegisterStatusArray::print(){
	std::cout << "Register Status: " << std::endl;

	for( int i=0 ; i<m_size ; i++ ){

		// Don't output register information unless it's non-zero
		if( m_registerStatus[i].lastWrite != 0 || m_registerStatus[i].nextWrite != 0){
			std::cout << "  Register " << i << ": Last Write - " << m_registerStatus[i].lastWrite << ", Next Write - " << m_registerStatus[i].nextWrite;
		}
	}
	std::cout << std::endl;
} // End print