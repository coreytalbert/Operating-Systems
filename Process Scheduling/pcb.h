/**
 * Assignment 3: Scheduling Algorithms
 * @file pcb.h
 * @author Corey Talbert
 * @brief This is the header file for the PCB class, a process control block.
 * @version 0.1
 * @date 11/4/2022
 */

#pragma once
#include <string>

/**
 * @brief A process control block (PCB) Process control block(PCB) is a data 
 * structure representing a process in the system.
 *       It contains the following fields:
 *       - process ID (PID)
 *       - process name
 *       - burst time
 *       - priority
 *      You may add more fields if you need.
 */
class PCB {
public:
    // The name of the process.
    std::string name;
    // The unique process ID.
    unsigned int id;
    // The priority of a process. Larger number represents higher priority.
    unsigned int priority;
    // The CPU burst time of the process.
    unsigned int burst_time;
    // The elapsed running time of the process.
    unsigned int running_time = 0 ;
    // The difference of the process's burst time and elapsed running time.
    unsigned int remaining_time = 0 ;
    // The total time spent waiting in the ready queue.
    unsigned int waiting_time = 0 ;
    // The total time from first entering the ready queue to completing,
    // or the sum of running time and waiting time.
    unsigned int turnaround_time = 0;

    /**
     * @brief Construct a new PCB object.
     * @param id: each process has a unique ID
     * @param priority: the priority of the process in the range 1-50. Larger 
     * numbers represent higher priority.
     * @param state the state of the process.
     */
    PCB( const std::string & name , const unsigned & id = 0 ,
        const unsigned & priority = 1 , const unsigned & burst_time = 0 )
        : name( name ) , id( id ) , priority( priority ) , burst_time( burst_time )
    {}

    /**
     * @brief Copy constructor.
     * @param old The PCB to be copied.
     */
    PCB( const PCB & old )
        : name( old.name ) , id( old.id ) , priority( old.priority ) ,
        burst_time( old.burst_time ) , running_time( old.running_time ) ,
        waiting_time( old.waiting_time ) , turnaround_time( old.turnaround_time )
    {}

    /**
     * @brief Destroy the PCB object.
     */
    ~PCB() {}

    /**
     * @brief Print the PCB object.
     */
    void print() const
    {
        printf("Process %u: %s has priority %u and burst time %u\n", 
            this->id, this->name.c_str(), this->priority, this->burst_time );
    }

    /**
     * @brief Creates a c-string containing the values of PCB member variables.
     * @return A pointer to a char array containing useful information about the
     * PCB state.
     */
    /*
    char * to_string() const
    {
        char * result = new char[ 80 ] ;
        snprintf( result , 80 , "%s (id=%u): priority=%u, burst=%u, run=%u, wait=%u, turn=%u" ,
            name.c_str() , id , priority , burst_time , running_time , waiting_time , turnaround_time ) ;
        return result ;
    }
    */
};