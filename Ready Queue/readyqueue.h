/**
 * Assignment 1: priority queue of processes
 * @file readyqueue.h
 * @author Corey Talbert
 * @brief ReadyQueue is a queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 * @version 0.1
 * @date 09-19-2022
 */

#pragma once
#include "pcb.h"


/**
 * @brief A queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 */
class ReadyQueue 
{
private:

    /** 
     * @brief A queue-like, FIFO, singly-linked list of PCB pointers
     * representing an ordered list of processes at the same priority level.
     * The ReadyQueue class is implemented as an array of these lists.
     */
    class List
    {
    private:
        
        /** 
         * @brief A simple liked list node with a forward link 
         * and a PCB pointer payload. 
         */
        struct Node
        {
            PCB* data = nullptr ;
            Node* next = nullptr ;
        } ;
        
        /** 
         * @brief The front and oldest element of the list. 
         */
        Node* head = nullptr ;
        
        /** 
         * @brief The end and newest element of the list. 
         */
        Node* tail = nullptr ;
        
        /** 
         * @brief The number of elements in the list. 
         */
        unsigned size = 0 ;
    
    public:
        /**
         * @brief Construct an empty list. 
         */
        List() ;
        
        /** 
         * @brief Destroy a list. 
         */
        ~List() ;
        
        /** 
         * @brief Check if the list is empty.
         * @return bool: True if the list is empty, otherwise false.
         */
        bool isEmpty() const ;    
        
        /** 
         * @brief Get the rq_size member.
         * @return unsigned: the number of PCB pointers in the list.
         */
        unsigned getSize() const ;
        
        /**
         * @brief Call the display method on each PCB pointed to in the list.
         * @return void
         */
        void display() const ;
        
        /**
         * @brief Reset the list to an empty state.
         */
        void clear() ;
        
        /**
         * @brief Remove the front (head) node from the list.
         * @return PCB*: The payload member in the head node.
         */
        PCB* popFront() ;
    
        /**
         * @brief Add a node to the end of the list.
         * @param new_data The PCB pointer to be held by the node.
         */
        void append( PCB* new_data ) ;

    } ; // End of class List
    
    /**
     * @brief The table of ordered, FIFO priority lists.
     * The table is one bigger than MAX_PRIORITY for 
     * ease of indexing / self-care / harm reduction. 
     */
    List rq[ MAX_PRIORITY + 1 ] ;
    
    /** 
     * @brief The index of the highest priority non-empty list.
     */
    unsigned top = 0 ;

    /**
     * @brief The number of PCB pointers in the queue.
     */
    unsigned rq_size = 0 ;

public:
    /**
     * @brief Construct a new ReadyQueue object
     */
    ReadyQueue() ;

    /**
     * @brief Destructor
     */
    ~ReadyQueue() ;

    /**
     * @brief Add a PCB representing a process into the ready queue.
     * @param pcbPtr: the pointer to the PCB to be added
     */
	void addPCB( PCB* pcbPtr ) ;

    /**
     * @brief Remove and return the PCB with the highest priority from the queue
     * @return PCB*: the pointer to the PCB with the highest priority
     */
	PCB* removePCB() ;

    /**
     * @brief Returns the number of elements in the queue.
     * @return int: the number of PCBs in the queue
     */
	int size() ;

     /**
      * @brief Display the PCBs in the queue.
      */
	void displayAll() ;

}; // End of class ReadyQueue