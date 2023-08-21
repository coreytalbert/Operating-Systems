/**
 * Assignment 3: CPU Scheduler
 * @file scheduler_sjf.h
 * @author Corey Talbert
 * @brief This Scheduler class implements the SJF scheduling algorithm.
 * @version 0.1
 * @date 11/4/2022
 */

#ifndef ASSIGN3_SCHEDULER_SJF_H
#define ASSIGN3_SCHEDULER_SJF_H
#include "scheduler.h"

/**
 * @brief The class implements a CPU Scheduler using a shortest job first
 * algorithm.
 */
class SchedulerSJF : public Scheduler
{
public:
    /**
     * @brief Construct a new SchedulerSJF object
     */
    SchedulerSJF();

    /**
     * @brief Destroy the SchedulerSJF object
     */
    ~SchedulerSJF() override;

    /**
     * @brief Sort the processes by ascending burst time.
     * @param process_list The ready queue of waiting processes.
     */
    void sort( std::vector<PCB> & process_list ) ;

    /**
     * @brief This function is called once before the simulation starts.
     *        It is used to initialize the scheduler.
     * @param process_list The list of processes in the simulation.
     */
    void init( std::vector<PCB> & process_list ) override;

    /**
     * @brief This function simulates the scheduling of processes in the ready queue.
     *        It stops when all processes are finished.
     */
    void simulate() override;
};

#endif //ASSIGN3_SCHEDULER_SJF_H