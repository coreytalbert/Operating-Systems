/**
 * Assignment 3: CPU Scheduler
 * @file scheduler_rr.cpp
 * @author Corey Talbert
 * @brief This Scheduler class implements the round robin scheduling algorithm.
 * @version 0.1
 * @date 11/4/2022
 */

#include "scheduler_rr.h"

/**
 * @brief Construct a new SchedulerRR object.
 */
SchedulerRR::SchedulerRR( int time_quantum ) : slice( time_quantum ) {}

/**
 * @brief Destroy the SchedulerRR object.
 */
SchedulerRR::~SchedulerRR() { delete this->ready_queue ; }

/**
 * @brief This function is called once before the simulation starts. It is
 * used to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerRR::init( std::vector<PCB> & process_list )
{
    this->process_list = process_list ;
    this->ready_queue = new List( process_list ) ;
    this->clearSchedulerStats() ;
    this->stats = new char * [ process_list.size() ] ;
    memset( this->stats , 0 , sizeof( char * ) * process_list.size() ) ;
}

/**
 * @brief This function simulates the scheduling of processes in the ready
 * queue. It stops when all processes are finished.
 */
void SchedulerRR::simulate()
{
    while ( not this->ready_queue->isEmpty() )
    {
        PCB * current_task = this->ready_queue->pop_front() ;
        // Each process takes time equal to the lesser of the time slice or its
        // remaining burst time.
        unsigned turn_time = ( this->slice <= current_task->burst_time - current_task->running_time )
            ? this->slice
            : current_task->burst_time - current_task->running_time ;
        printf( "Running Process %s for %u time units\n" , current_task->name.c_str() , turn_time ) ;
        // The waiting time of the process considers the scheduler's overall
        // elapsed time less the time this process has spent running up to now.
        // The process must have been waiting if it wasn't running! 
        current_task->waiting_time = this->elapsed_time - current_task->running_time ;
        // The process's running time is increased by the turn time length.
        current_task->running_time += turn_time ;
        // If the task's running time is equal to its burst time, the process is
        // complete.
        if ( current_task->running_time >= current_task->burst_time )
        {
            // The process's turnaround time can be calculated as the sum of its
            // running time and waiting time.
            current_task->turnaround_time = current_task->running_time + current_task->waiting_time ;
            // The process's updated total wait time is added to the scheduler's
            // total of cumulative process waiting time.
            this->increaseAggregateWaitingTime( current_task->waiting_time ) ;
            // The scheduler tracks the cumulative turnaround times of all 
            // processes to determine the overall average.
            this->increaseAggregateTurnaroundTime( current_task->turnaround_time ) ;
            // The scheduler's counter of finished processes is updated. 
            // It's used to calculate average turnaround and waiting times.
            this->incrementProcessesCompleted() ;
            // The final process stats are stored to an array held by the 
            // scheduler.
            this->saveStats( current_task ) ;
        }
        // If the process still has time remaining, it is moved to the end of 
        // the ready queue.
        else
            this->ready_queue->push_back( current_task ) ;

        // The turn time is added to the scheduler's overall elapsed time.
        this->increaseElapsedTime( turn_time ) ;
    } // End of while loop.
}

/******************************************************************************\
|* SchedulerRR::List defintions                                               *|
\******************************************************************************/

/**
 * @brief Default constructor. Creates an empty list with size 0.
 */
SchedulerRR::List::List() {}

/**
 * @brief Creates a list from a vector of PCBs.
 * @param vec The vector of processes.
 */
SchedulerRR::List::List( std::vector<PCB> & vec )
{
    unsigned new_size = vec.size() ;
    for ( unsigned i = 0 ; i < new_size ; ++i )
    {
        PCB * new_data = &vec[ i ] ;
        this->push_back( new_data ) ;
    }
    this->size = new_size ;
}
    
/**
 * @brief Destructor.
 */
SchedulerRR::List::~List() { this->clear() ; }

/**
 * @brief Indicates if the the list is empty.
 * @return True if the list is empty, otherwise false.
 */
bool SchedulerRR::List::isEmpty() const { return not this->size ; }

/**
 * @brief Deletes all nodes in the list and sets the list size to 0. 
 */
void SchedulerRR::List::clear()
{
    if ( not this->isEmpty() )
    {
        Node * itr = this->head ;
        Node * temp ;
        while ( itr != nullptr )
        {
            temp = itr ;
            itr = itr->next ;
            delete temp ;
        }
        this->head = nullptr ;
        this->tail = nullptr ;
        this->size = 0 ;
    }
}

/**
 * @brief Removes the first (head) node of the list. 
 * @return The PCB pointer contained in the list head node.
 */
PCB * SchedulerRR::List::pop_front()
{
    PCB * result = nullptr ;
    Node * delete_target = nullptr ;
    if ( not this->isEmpty() )
    {
        result = this->head->data ;
        delete_target = this->head ;
        this->head = this->head->next ;
        if ( this->head != nullptr )
            this->head->prev = nullptr ;
        if ( delete_target != nullptr )
            delete delete_target ;
    }
    -- this->size ;
    return result ;
}

/**
 * @brief Appends a node containing a given PCB pointer to the list. 
 * @param new_data The PCB pointer to be appended to the list.
 */
void SchedulerRR::List::push_back( PCB * new_data )
{
    Node * new_node = new Node( new_data ) ;
    if ( this->isEmpty() )
    {
        this->head = new_node ;
        this->tail = new_node ;
    }
    else
    {
        this->tail->next = new_node ;
        new_node->prev = this->tail ;
        this->tail = new_node ;
    }
    ++ this->size ;
}