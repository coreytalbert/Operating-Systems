/**
 * Assignment 1: priority queue of processes
 * @file readyqueue.cpp
 * @author Corey Talbert
 * @brief Implementation of readyqueue.h as a bucket queue with separate chaining.
 * @date 09-19-2022
 */
#include "readyqueue.h"

/**
* @brief Construct an empty list. 
*/
ReadyQueue::List::List() {}

/** 
 * @brief Destroy a list. 
 */
ReadyQueue::List::~List() { this -> clear(); }

/** 
 * @brief Check if the list is empty.
 * @return bool: True if the list is empty, otherwise false.
 */
bool ReadyQueue::List::isEmpty() const { return not this -> size ; }

/** 
 * @brief Get the rq_size member.
 * @return unsigned: the number of PCB pointers in the list.
 */
unsigned ReadyQueue::List::getSize() const { return this -> size ; }

/**
 * @brief Call the display method on each PCB pointed to in the list.
 * @return void
 */
void ReadyQueue::List::display() const 
{
    //printf( "\nList @ 0x%p, size = %u:\t\n", this, size ) ;
    //unsigned i = 0 ; // Index counter for test output.
    // Call PCB::display() on each element in the list.
    for ( Node* itr = this -> head ; itr ; itr = itr -> next )
    {
        //printf("\t[%u]\t0x%p:\t", ++i, itr -> data ) ;
        putchar('\t') ;
        itr -> data -> display() ;
        putchar('\n') ;
        //puts( itr -> next ? " -> " : ""  ) ;
    }
}

/**
 * @brief Reset the list to an empty state.
 */
void ReadyQueue::List::clear()
{
    if ( not this -> isEmpty() )
    {
        // Delete each PCB in the list.
        for (   Node* itr = this -> head, *temp = nullptr ; 
                itr ;
                temp = itr, itr = itr -> next, delete temp ) ;
        // Reset member variables to represent an empty state.
        this -> head = this -> tail = nullptr ;
        this -> size = 0 ;
    }
}

/**
 * @brief Remove the front (head) node from the list.
 * @return PCB*: The payload member in the head node.
 */
PCB* ReadyQueue::List::popFront() 
{
    PCB* pop_data = nullptr ; 
    // Remove list head and update list state.
    if ( not this -> isEmpty() )
    {
        pop_data = this -> head -> data ;
        Node* temp = this -> head ;
        this -> head = this -> head -> next ;
        delete temp ;
        -- this -> size ;
    } 
    return pop_data ;
}

/**
 * @brief Add a node to the end of the list.
 * @param new_data The PCB pointer to be held by the node.
 */
void ReadyQueue::List::append( PCB* new_data )
{
    Node* new_node = new Node();
    new_node -> data = new_data ;
    new_node -> next = nullptr ;
    if ( this -> isEmpty() )
        this -> head = this -> tail = new_node ;
    else
    {
        this -> tail -> next = new_node ;
        this -> tail = this -> tail -> next ;
    }
    ++ this -> size ;
}


/**
 * @brief Constructor for the ReadyQueue class.
 */
ReadyQueue::ReadyQueue()
{}

/**
 * @brief Destructor
 */
ReadyQueue::~ReadyQueue() 
{
    for ( List& l : this -> rq )
    {
        l . clear() ;
    }
}

/**
 * @brief Add a PCB representing a process into the ready queue.
 * @param pcbPtr: the pointer to the PCB to be added
 */
void ReadyQueue::addPCB( PCB* pcbPtr ) 
{
    pcbPtr->setState( ProcState::READY ) ;
    unsigned new_priority = pcbPtr -> getPriority() ;
    rq[ new_priority ] . append( pcbPtr ) ;
    ++ this -> rq_size ;
    // If the new PCB has the highest priority in the queue, update top.
    if ( this -> top < new_priority )
    {
        this -> top = new_priority ;
    }
}

/**
 * @brief Remove and return the PCB with the highest priority from the queue
 * @return PCB*: the pointer to the PCB with the highest priority
 */
PCB* ReadyQueue::removePCB()
{
    PCB* result = nullptr ;
    if ( this -> rq_size > 0 )
    {
        result = rq[ top ] . popFront() ;
        result -> setState( ProcState::RUNNING ) ;
        -- this -> rq_size ;

        // UPDATE TOP
        // The readyqueue is empty.
        if ( rq_size == 0 ) 
        {
            top = 0 ; 
        }
        // There are no more PCBs at this priority.
        else if ( rq[ top ] . isEmpty() )
        {
            // Decrement top until next highest priority
            // non-empty PCB list is found.
            while ( top > 0 and rq[ --top ] . isEmpty() ) ;
        }
    }
    return result ;
}

/**
 * @brief Returns the number of elements in the queue.
 * @return int: the number of PCBs in the queue
 */
int ReadyQueue::size() 
{
    return rq_size ;
}

/**
 * @brief Display the PCBs in the queue.
 */
void ReadyQueue::displayAll() 
{
    puts("Display Processes in ReadyQueue:") ;
    for ( int i = MAX_PRIORITY ; i > MIN_PRIORITY - 1 ; --i )
    {
        if ( not rq[ i ] . isEmpty() ) 
        {
            rq[ i ] . display() ;
        }
    }
}