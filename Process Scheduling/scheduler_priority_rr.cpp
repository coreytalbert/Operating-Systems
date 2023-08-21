/**
 * Assignment 3: CPU Scheduler
 * @file scheduler_priority_rr.cpp
 * @author Corey Talbert
 * @brief This Scheduler class implements the Priority Round Robin scheduling algorithm.
 * @version 0.1
 * @date 11/4/2022
 */
#include "scheduler_priority_rr.h"

/**
 * @brief Construct a new SchedulerRR object.
 * @param time_quantum The maximum burst time per turn.
 */
SchedulerPriorityRR::SchedulerPriorityRR( unsigned time_quantum ) : slice( time_quantum ) {}

/**
 * @brief Destroy the SchedulerRR object.
 */
SchedulerPriorityRR::~SchedulerPriorityRR()
{
    delete this->ready_queue ;
}

/**
 * @brief This function is called once before the simulation starts. It is used
 * to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerPriorityRR::init( std::vector<PCB> & process_list )
{
    // The process table is copied.
    this->process_list = process_list ;
    // The ready queue is initialized from the process table.
    ready_queue = new PriorityQueue( process_list ) ;
    // The stats string array is initialized as an empty c-string array.
    this->clearSchedulerStats() ;
    this->stats = new char * [ process_list.size() ] ;
    memset( this->stats , 0 , sizeof( char * ) * process_list.size() ) ;
}


/**
 * @brief This function simulates the scheduling of processes in the ready
 * queue. It stops when all processes are finished.
 */
void SchedulerPriorityRR::simulate()
{
    while ( not this->ready_queue->isEmpty() )
    {
        // The highest priority element is removed from the queue.
        PCB * current_task = ready_queue->pop() ;
        // If the current task is the only one left at its priority level, it
        // runs until it is complete. Otherwise, each process takes time equal 
        // to the lesser of the time slice or its remaining burst time.
        unsigned turn_time = current_task->burst_time - current_task->running_time ;
        if ( ready_queue->sizeAtPriority( current_task->priority ) > 0 and this->slice < turn_time )
            turn_time = this->slice ;
        printf( "Running Process %s for %u time units\n" , current_task->name.c_str() , turn_time ) ;
        // The waiting time of the process considers the scheduler's overall
        // elapsed time less the time this process has spent running up to now.
        // The process must have been waiting if it wasn't running! 
        current_task->waiting_time = this->elapsed_time - current_task->running_time ;
        // The process is allowed to run for an amount of time up to the 
        // scheduler's time slice.
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
        // If the process still has time remaining, it is moved to the end 
        // of its priority level list in the ready queue.
        else
            this->ready_queue->push( current_task ) ;
        // The process runs until it completes. Its burst time is added to the
        // scheduler's overall elapsed time.
        this->increaseElapsedTime( turn_time ) ;
    }
}

/******************************************************************************\
|* SchedulerPriorityRR::List defintions                                       *|
\******************************************************************************/

/**
 * @brief Default constructor. Creates an empty list with size 0.
 */
SchedulerPriorityRR::List::List() {}

/**
 * @brief Creates a list from a vector of PCBs.
 * @param vec The vector of processes.
 */
SchedulerPriorityRR::List::List( std::vector<PCB> & vec )
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
SchedulerPriorityRR::List::~List() { this->clear() ; }

/**
 * @brief Indicates if the the list is empty.
 * @return True if the list is empty, otherwise false.
 */
bool SchedulerPriorityRR::List::isEmpty() const
{
    return not this->size or this->head == nullptr ;
}

/**
 * @brief Gives the number of nodes in the list.
 * @return The number of nodes in the list if any, otherwise 0.
 */
bool SchedulerPriorityRR::List::getSize() const
{
    return this->size ;
}

/**
 * @brief Deletes all nodes in the list and sets the list size to 0. 
 */
void SchedulerPriorityRR::List::clear()
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
PCB * SchedulerPriorityRR::List::pop_front()
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
        delete delete_target ;
    }
    -- this->size ;
    return result ;
}

/**
 * @brief Appends a node containing a given PCB pointer to the list. 
 * @param new_data The PCB pointer to be appended to the list.
 */
void SchedulerPriorityRR::List::push_back( PCB * new_data )
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

/******************************************************************************\
|* SchedulerPriorityRR::PriorityQueue definitions                             *|
\******************************************************************************/

/**
 * @brief Default constructor.
 */
SchedulerPriorityRR::PriorityQueue::PriorityQueue() {}

/**
 * @brief Populates a queue with the PCBs contained in vec, with up to 
 * max_priority lists.
 * @param vec The process table from which the queue is initialized.
 * @param max_priority The highest possible priority of a PCB. The
 * number of lists in the queue is max_priority+1 for ease of indexing.
 */
SchedulerPriorityRR::PriorityQueue::PriorityQueue( std::vector<PCB> & vec , 
    const unsigned & max_priority )
    : queue( new List * [ max_priority + 1 ] )
{
    // Each index of the queue is set to null until it is needed to 
    // store PCBs.
    for ( unsigned i = 0 ; i < MAX_PRIORITY + 1 ; ++i )
        queue[ i ] = nullptr ;
    // The queue is initalized based on the given process table.
    for ( PCB & pcb : vec )
        this->push( &pcb ) ;
}

/**
 * @brief Destructor.
 */
SchedulerPriorityRR::PriorityQueue::~PriorityQueue()
{
    // Each list is destroyed.
    for ( unsigned i = 0 ; i < this->size ; ++i )
        if ( this->queue[ i ] != nullptr )
            this->queue[ i ]->~List() ;
    // The array space is deallocated.
    delete[] this->queue ;
    // The members are reset.
    this->size = 0 ;
    this->top = 0 ;
}

/**
 * @brief Indicates is the queue is empty.
 * @return True if the queue is empty, otherwise false.
 */
bool SchedulerPriorityRR::PriorityQueue::isEmpty() const { return not this->size ; }

/**
 * @brief Gives the number of PCBs in the queue with the given priority.
 * @param p The priority to search.
 * @return The number of PCBs in the queue with priority p if any,
 * otherwise 0.
 */
unsigned SchedulerPriorityRR::PriorityQueue::sizeAtPriority( unsigned p ) const
{
    return ( this->queue[p] != nullptr ) 
        ? this->queue[p]->getSize() 
        : 0 ;
}

/**
 * @brief Removes the earliest highest-priority element from the queue.
 * @return A pointer to the earliest, highest-priority element in the 
 * queue.
 */
PCB * SchedulerPriorityRR::PriorityQueue::pop()
{
    PCB * result = nullptr ;
    if ( not this->isEmpty() )
    {
        result = this->queue[ top ]->pop_front() ;
        // UPDATE TOP
        // The queue is empty.
        if ( -- this->size == 0 )
            top = 0 ;
        // There are no other PCBs at this priority.
        else if ( this->queue[ top ]->isEmpty() )
            // Decrement top until a non-empty list is found.
            while ( this->top > 0 )
            {
                -- this->top ;
                if ( this->queue[ top ] == nullptr )
                    continue ;
                else if ( not this->queue[ top ]->isEmpty() )
                    break ;
            }

    }
    return result ;
}

/**
 * @brief Add a PCB to the queue.
 * @param pcb A pointer to the PCB to be added.
 */
void SchedulerPriorityRR::PriorityQueue::push( PCB * pcb )
{
    if ( pcb != nullptr )
    {
        // Instantiate the list, if needed.
        if ( this->queue[ pcb->priority ] == nullptr )
            this->queue[ pcb->priority ] = new List() ;
        // Add the PCB to the list.
        this->queue[ pcb->priority ]->push_back( pcb ) ;
        // Update top.
        if ( pcb->priority > this->top )
            this->top = pcb->priority ;
        ++ this->size ;
    }
}
