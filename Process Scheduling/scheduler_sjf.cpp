/**
* Assignment 3: CPU Scheduler
 * @file scheduler_sjf.cpp
 * @author Corey Talbert
 * @brief This Scheduler class implements the shortest job first scheduling 
 * algorithm.
 * @version 0.1
 * @date 11/4/2022
 */

#include "scheduler_sjf.h"

/**
 * @brief Construct a new SchedulerSJF object
 */
SchedulerSJF::SchedulerSJF() {}

/**
 * @brief Destroy the SchedulerSJF object
 */
SchedulerSJF::~SchedulerSJF() {}

/**
 * @brief Sort the processes by ascending burst time.
 * @param process_list The ready queue of waiting processes.
 */
void SchedulerSJF::sort( std::vector<PCB> & process_list )
{
    for ( unsigned i = 0 ; i < process_list.size() - 1 ; ++i )
        for ( unsigned j = 0 ; j < process_list.size() - ( 1 + i ) ; ++j )
            if ( process_list[ j ].burst_time > process_list[ j + 1 ].burst_time )
            {
                PCB temp = process_list[ j ] ;
                process_list[ j ] = process_list[ j + 1 ] ;
                process_list[ j + 1 ] = temp ;
            }
}

/**
 * @brief This function is called once before the simulation starts.
 *        It is used to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerSJF::init( std::vector<PCB> & process_list )
{
    this->process_list = process_list ;
    //sort by burst time
    sort( this->process_list ) ;
    this->clearSchedulerStats() ;
    this->stats = new char * [ process_list.size() ] ;
    memset( this->stats , 0 , sizeof( char * ) * process_list.size() ) ;
}

/**
 * @brief This function simulates the scheduling of processes in the ready queue.
 *        It stops when all processes are finished.
 */
void SchedulerSJF::simulate()
{
    for ( std::vector<PCB>::iterator itr = process_list.begin() ; itr != process_list.end() ; ++itr )
    {
        PCB current_task = *itr ;
        printf( "Running Process %s for %u time units\n" , current_task.name.c_str() , current_task.burst_time ) ;
        // The waiting time of the process considers the scheduler's overall
        // elapsed time less the time this process has spent running up to now.
        // The process must have been waiting if it wasn't running! 
        current_task.waiting_time = this->elapsed_time - current_task.running_time ;
        // The process is allowed to run for its entire burst time. The process
        // running time should be updated to reflect that.
        current_task.running_time += current_task.burst_time ;
        // Also, since the task is now complete, its turnaround time can be 
        // calculated as the sum of its running time and waiting time.
        current_task.turnaround_time = current_task.running_time + current_task.waiting_time ;
        // The process's updated total wait time is added to the scheduler's
        // total of cumulative process waiting time.
        this->increaseAggregateWaitingTime( current_task.waiting_time ) ;
        // The scheduler tracks the cumulative turnaround times of all processes
        // to determine the overall average.
        this->increaseAggregateTurnaroundTime( current_task.turnaround_time ) ;
        // The process runs until it completes. Its burst time is added to the
        // scheduler's overall elapsed time.
        this->increaseElapsedTime( current_task.burst_time ) ;
        // The scheduler's counter of finished processes is updated. It's used
        // to calculate average turnaround and waiting times.
        this->incrementProcessesCompleted() ;
        // The final process stats are stored to an array held by the scheduler.
        this->saveStats( &current_task ) ;
    }

}
