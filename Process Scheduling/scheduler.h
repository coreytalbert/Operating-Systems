/**
* Assignment 3: CPU Scheduler
 * @file scheduler.h
 * @author Corey Talbert
 * @brief This is the header file for the base Scheduler class. Specific schedulers, e.g. FCFS, SJF and RR, inherit
 *        this base class.
 * @version 0.1
 * @date 11/4/2022
 */
#pragma once
#include <cstring>
#include <vector>
#include "pcb.h"

const unsigned MAX_LINE = 80 ;

/**
 * @brief This is the base class for the scheduler.
 */
class Scheduler
{
protected:
    // An array of c-strings of timing statistics for each process run by the scheduler.
    char ** stats = nullptr ;
    // The index of the next empty char array in member stats.
    unsigned stats_index = 0 ;
    // The sum of all completed processes' turnaround times.
    unsigned int aggregate_turnaround_time = 0 ;
    // The average time for a process to complete.
    float average_turnaround_time = 0 ;
    // The cumulative time of all processes spent waiting.
    unsigned int aggregate_waiting_time = 0 ;
    // The average time a process spends waiting.
    float average_waiting_time = 0 ;
    // The total elapsed time spent by the CPU running processes.
    unsigned int elapsed_time = 0 ;
    // The total number of processes that have finished.
    unsigned int processes_completed = 0 ;
    // The table of processes.
    std::vector<PCB> process_list ;

    /**
     * @brief Construct a new Scheduler object
     */
    Scheduler() {}

    /**
     * @brief Destroy the Scheduler object
     */
    virtual ~Scheduler()
    {
        this->clearSchedulerStats() ;
    }

    /**
     * @brief Erase the scheduler statistics stored in member variable stats.
     */
    void clearSchedulerStats()
    {
        if ( this->stats != nullptr )
        {
            // Each c-string is deleted.
            for ( unsigned i = 0 ; i < this->stats_index ; ++i )
                if ( stats[ i ] != nullptr )
                    delete stats[ i ] ;
            // The array space is deallocated.
            delete[] stats ;
        }
    }

    /**
     * @brief Saves information about the given PCB pointed to by current_task
     * to the member string array stats. Meant to be used upon process
     * completion.
     * @param current_task The task whose information is stored.
     */
    void saveStats( PCB * current_task )
    {
        // Clear the string at the current index, if any. This shouldn't happen.
        if ( this->stats[ stats_index ] != nullptr )
            delete[] this->stats[ stats_index ] ;
        this->stats[ stats_index ] = new char[ MAX_LINE ] ;
        // Zero the line.
        memset( this->stats[ stats_index ] , 0 , MAX_LINE ) ;
        // Save the process's name, turnaround time, and waiting time.
        snprintf( this->stats[ stats_index ] , MAX_LINE , "%s turn-around time = %u, waiting time = %u" ,
            current_task->name.c_str() , current_task->turnaround_time , current_task->waiting_time ) ;
        // stats_index is updated to indicate the next empty slot in array stats.
        ++ stats_index ;
    }

    /**
     * @brief Increases the scheduler's elapsed_time member variable by n.
     * @param n The amount to increase elapsed_time.
     */
    void increaseElapsedTime( const unsigned & n )
    {
        this->elapsed_time += n ;
    }

    /**
     * @brief Increases the scheduler's aggregate_turnaround_time member
     * variable by n.
     * @param n The amount to increase aggregate_turnaround_time.
     */
    void increaseAggregateTurnaroundTime( const unsigned & n )
    {
        this->aggregate_turnaround_time += n ;
        if ( this->processes_completed != 0 )
            average_turnaround_time = ( float ) this->aggregate_turnaround_time / this->processes_completed ;
    }

    /**
     * @brief Increases the scheduler's aggregate_waiting_time member variable
     * by n.
     * @param n The amount to increase aggregate_waiting_time.
     */
    void increaseAggregateWaitingTime( const unsigned & n )
    {
        this->aggregate_waiting_time += n ;
        if ( this->processes_completed != 0 )
            average_waiting_time = ( float ) this->aggregate_waiting_time / this->processes_completed ;
    }

    /**
     * @brief Increases the scheduler's count of completed processes by one.
     */
    void incrementProcessesCompleted()
    {
        ++ this->processes_completed ;
        if ( this->processes_completed != 0 )
        {
            average_turnaround_time = ( float ) this->aggregate_turnaround_time / this->processes_completed ;
            average_waiting_time = ( float ) this->aggregate_waiting_time / this->processes_completed ;
        }
    }

public:
    /**
     * @brief This function is called once before the simulation starts. It is
     * used to initialize the scheduler.
     * @param process_list The list of processes in the simulation.
     */
    virtual void init( std::vector<PCB> & process_list ) = 0 ;

    /**
     * @brief This function simulates the scheduling of processes in the ready
     * queue. It stops when all processes are finished.
     */
    virtual void simulate() = 0 ;

    /**
     * @brief This function is called once after the simulation ends. It is used
     * to print out the results of the simulation.
     */
    void print_results()
    {
        for ( unsigned i = 0 ; i < this->stats_index ; ++i )
            ( this->stats[ i ] != nullptr )
                ? puts( this->stats[ i ] )
                : printf( "No statistics available for process %u.\n" , i ) ;

        printf( "Average turn-around time = %.6g, Average waiting time = %.6g\n" ,
            this->average_turnaround_time , this->average_waiting_time ) ;
    }

};