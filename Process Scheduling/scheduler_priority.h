/**
 * Assignment 3: CPU Scheduler
 * @file scheduler_priority.h
 * @author Corey Talbert
 * @brief This Scheduler class implements the priority scheduling algorithm.
 * @version 0.1
 * @date 11/4/2022
 */

#ifndef ASSIGN3_SCHEDULER_PRIORITY_H
#define ASSIGN3_SCHEDULER_PRIORITY_H
#include "scheduler.h"

/**
 * @brief Implements a CPU Scheduler using a highest-priority first algorithm.
 */
class SchedulerPriority : public Scheduler
{
public:
    /**
     * @brief Construct a new SchedulerPriority object
     */
    SchedulerPriority() ;

    /**
     * @brief Destroy the SchedulerPriority object
     */
    ~SchedulerPriority() override ;

    /**
     * @brief This function is called once before the simulation starts. It is 
     * used to initialize the scheduler.
     * @param process_list The list of processes in the simulation.
     */
    void init( std::vector<PCB> & process_list ) override ;

    /**
     * @brief Sort the ready queue by descending priority (higher value is
     * higher priority).
     * @param process_list The ready queue of waiting processes.
     */
    void sort( std::vector<PCB> & process_list ) ;

    /**
     * @brief This function simulates the scheduling of processes in the ready
     * queue. It stops when all processes are finished.
     */
    void simulate() override ;

};

#endif //ASSIGN3_SCHEDULER_PRIORITY_H
