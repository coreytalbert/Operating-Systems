/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority_rr.h
 * @author Corey Talbert
 * @brief This Scheduler class implements the FCSF scheduling algorithm.
 * @version 0.1
 * @date 11/4/2022
 */
#ifndef ASSIGN3_SCHEDULER_PRIORITY_RR_H
#define ASSIGN3_SCHEDULER_PRIORITY_RR_H

#include "scheduler.h"

#define MAX_PRIORITY 10

/**
 * @brief This class implements a scheduler using a priority round-robin algorithm.
 */
class SchedulerPriorityRR : public Scheduler {
private:

    /**
     * @brief A linked list to hold PCBs of the same priority.
     */
    class List
    {
    private:

        /**
         * @brief A doubly-linked list node containing a PCB.
         */
        struct Node
        {
            Node * prev = nullptr ;
            Node * next = nullptr ;
            PCB * data = nullptr ;
            Node( PCB * new_data ) 
                : prev( nullptr ) , next( nullptr ) , data( new_data ) {}
        } ;
        // The first node of the list.
        Node * head = nullptr ;
        // The last node of the list.
        Node * tail = nullptr ;
        // The number of nodes in the list.
        unsigned size = 0 ;
    
    public:
        /**
         * @brief Default constructor. Creates an empty list with size 0.
         */
        List();
        
        /**
         * @brief Creates a list from a vector of PCBs.
         * @param vec The vector of processes.
         */
        List( std::vector<PCB> & vec ) ;
        
        /**
         * @brief Destructor.
         */
        ~List() ;
        
        /**
         * @brief Indicates if the the list is empty.
         * @return True if the list is empty, otherwise false.
         */
        bool isEmpty() const ;

        /**
         * @brief Gives the number of nodes in the list.
         * @return The number of nodes in the list if any, otherwise 0.
         */
        bool getSize() const ;
        
        /**
         * @brief Deletes all nodes in the list and sets the list size to 0. 
         */
        void clear() ;
        
        /**
         * @brief Removes the first (head) node of the list. 
         * @return The PCB pointer contained in the list head node.
         */
        PCB * pop_front() ;
        
        /**
         * @brief Appends a node containing a given PCB pointer to the list. 
         * @param new_data The PCB pointer to be appended to the list.
         */
        void push_back( PCB * new_data ) ;

    } ; // End of List

    /**
     * @brief This class implements a priority queue as an array of lists, where
     * each list holds PCBs with the same priority. The front or "top" element
     * of the queue is the queue is the PCB with the highest priority. If there
     * are multiple PCBs with the same priority, one removed is the one added to
     * the queue earliest.
     */
    class PriorityQueue
    {
    private:
        // The array of linked lists containing PCBs of the same priority.
        List ** queue = nullptr ;
        // The number of PCBs in the entire queue.
        unsigned size = 0 ;
        // The index of the highest priority non-empty list.
        unsigned top = 0 ;
   
    public:
        /**
         * @brief Default constructor.
         */
        PriorityQueue() ;
        
        /**
         * @brief Populates a queue with the PCBs contained in vec, with up to 
         * max_priority lists.
         * @param vec The process table from which the queue is initialized.
         * @param max_priority The highest possible priority of a PCB. The
         * number of lists in the queue is max_priority+1 for ease of indexing.
         */
        PriorityQueue( std::vector<PCB> & vec , const unsigned & max_priority = MAX_PRIORITY ) ;
        
        /**
         * @brief Destructor.
         */
        ~PriorityQueue() ;

        /**
         * @brief Indicates is the queue is empty.
         * @return True if the queue is empty, otherwise false.
         */
        bool isEmpty() const ;

        /**
         * @brief Gives the number of PCBs in the queue with the given priority.
         * @param p The priority to search.
         * @return The number of PCBs in the queue with priority p if any,
         * otherwise 0.
         */
        unsigned sizeAtPriority( unsigned p ) const ;
        
        /**
         * @brief Removes the earliest highest-priority element from the queue.
         * @return A pointer to the earliest, highest-priority element in the 
         * queue.
         */
        PCB * pop() ;

        /**
         * @brief Add a PCB to the queue.
         * @param pcb A pointer to the PCB to be added.
         */
        void push( PCB * pcb ) ;

    } ; // End PriorityQueue

    // The ready queue of processes.
    PriorityQueue * ready_queue = nullptr ;
    // The time slice allocated to running processes.
    unsigned slice = 0 ;

public:
    /**
     * @brief Construct a new SchedulerRR object.
     * @param time_quantum The maximum burst time per turn.
     */
    SchedulerPriorityRR( unsigned time_quantum = 10 ) ;

    /**
     * @brief Destroy the SchedulerRR object.
     */
    ~SchedulerPriorityRR() override ;

    /**
     * @brief This function is called once before the simulation starts. It is 
     * used to initialize the scheduler.
     * @param process_list The list of processes in the simulation.
     */
    void init( std::vector<PCB> & process_list ) override ;

    /**
     * @brief This function simulates the scheduling of processes in the ready 
     * queue. It stops when all processes are finished.
     */
    void simulate() override ;
} ;


#endif //ASSIGN3_SCHEDULER_PRIORITY_RR_H
