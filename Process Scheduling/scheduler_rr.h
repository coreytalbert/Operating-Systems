/**
 * Assignment 3: CPU Scheduler
 * @file scheduler_rr.h
 * @author Corey Talbert
 * @brief This Scheduler class implements the round robin scheduling algorithm.
 * @version 0.1
 * @date 11/4/2022
 */

#ifndef ASSIGN3_SCHEDULER_RR_H
#define ASSIGN3_SCHEDULER_RR_H

#include "scheduler.h"

class SchedulerRR : public Scheduler
{
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
                : prev( nullptr ) , next( nullptr ) , data( new_data )
            {}
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

    // The ready queue of processes.
    List * ready_queue = nullptr ;
    // The time slice allocated to running processes.
    unsigned slice = 0 ;

public:
    /**
     * @brief Construct a new SchedulerRR object.
     */
    SchedulerRR( int = 10 ) ;

    /**
     * @brief Destroy the SchedulerRR object.
     */
    ~SchedulerRR() override ;

    /**
     * @brief This function is called once before the simulation starts. It is
     * used to initialize the scheduler.
     * @param process_list The list of processes in the simulation.
     */
    void init( std::vector<PCB> & process_list ) override ;

    /**
     * @brief This function simulates the scheduling of processes in the ready queue.
     *        It stops when all processes are finished.
     */
    void simulate() override ;
} ;

#endif //ASSIGN3_SCHEDULER_RR_H