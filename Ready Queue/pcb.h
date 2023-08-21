/**
 * Assignment 1: priority queue of processes
 * @file pcb.h
 * @author Corey Talbert
 * @brief This is the header file for the PCB class, a process control block.
 * @version 0.1
 * @date 09-19-2022
 */

#pragma once
#include <iostream>
using namespace std;

/**
 * @brief The highest possible priority of a process control block.
 */
const unsigned MAX_PRIORITY = 50 ;

/**
 * @brief The lowest possible priority of a process control block.
 */
const unsigned MIN_PRIORITY = 1 ;


// enum class of process state
// A process (PCB) in ready queue should be in READY state
enum class ProcState { NEW, READY, RUNNING, WAITING, TERMINATED };

/**
 * @brief A process control block (PCB) Process control block(PCB) is a data structure representing a process in the system.
   A process should have at least an ID and a state(i.e.NEW, READY, RUNNING, WAITING or TERMINATED).
   It may also have other attributes, such as scheduling information (e.g. priority)
 */
class PCB 
{
public:
    // The unique process ID
	unsigned int id;
    // The priority of a process valued between 1-50. Larger number represents higher priority
	unsigned int priority;
	// The current state of the process.
	// A process in the ReadyQueue should be in READY state
	ProcState state;

	/**
	 * @brief Construct a new PCB object
	 * @param id: each process has a unique ID
	 * @param priority: the priority of the process in the range 1-50. Larger number represents higher priority
	 * @param state the state of the process.
     */
    PCB(const unsigned int& id = 0, 
        const unsigned int& priority = MIN_PRIORITY, 
        const ProcState& state = ProcState::NEW) 
        : id( id ), priority( priority ), state( state ) 
    {}

    /**
     * @brief Destroy the PCB object.
     */
    ~PCB() {}

    /**
     * @brief Get the ID of the PCB.
     * @return unsigned int: the ID of the PCB
     */
    unsigned int getID() const 
    {
        return this -> id;
    }

    /**
     * @brief Get the priority of the PCB.
     * @return unsigned int: the priority of the PCB
     */
    unsigned int getPriority() const 
    {
        return this -> priority;
    }

    /**
     * @brief Get the state of the PCB.
     * @return ProcState: the state of the PCB
     */
    ProcState getState() const 
    {
        return this -> state;
    }

    /**
     * @brief Change the state of the PCB.
     * @param state
     */
    void setState( const ProcState& state ) 
    {
        this -> state = state ;
    }

    /**
     * @brief Change the priority of the PCB.
     * @param priority
     */
    void setPriority( const unsigned int& priority ) 
    {
        this -> priority = priority ;
    }

    /**
     * @brief Print the PCB.
     */
    void display() const {
        cout << "ID: " << id;
        cout << ", Priority: " << priority;
        cout << ", State: " ;
        //printf( "ID: %i, Priority: %i, State: ", id, priority ) ;
        switch(state) {
            case ProcState::NEW:
                printf( "NEW" ) ;
                break;
            case ProcState::READY:
                printf( "READY" ) ;
                break;
            case ProcState::RUNNING:
                printf( "RUNNING" ) ;
                break;
            case ProcState::WAITING:
                printf( "WAITING" ) ;
                break;
            case ProcState::TERMINATED:
                printf( "TERMINATED" ) ;
                break;
        } 
        //printf( "\n" ) ;
    }
};
