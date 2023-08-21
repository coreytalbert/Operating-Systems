/**
 * Assignment 1: priority queue of processes
 * @file pcbtable.h
 * @author Corey Talbert
 * @brief This is the header file for the PCBTable class, an array(list) of all PCB elements in the system..
 * @version 0.1
 * @date 09-19-2022
 */

#pragma once
#include "pcb.h"
#include <vector>

/**
 * @brief PCTable is an array of all PCB's in the system.
 */
class PCBTable {
private:
    vector<PCB*> table ;

public:
    /**
     * @brief Construct a new PCBTable object of the given size (number of PCBs)
     * @param size: the capacity of the PCBTable
     */
    PCBTable( const int& size = 100 ) ;

    /**
     * @brief Destroy the PCBTable object. Make sure to delete all the PCBs in the table.
     */
    ~PCBTable() ;

    /**
     * @brief Get the PCB at index "idx" of the PCBTable.
     * @param idx: the index of the PCB to get
     * @return PCB*: pointer to the PCB at index "idx"
     */
    PCB* getPCB( const unsigned& idx ) const ;

    /**
     * @brief Overload of the operator [] that returns the PCB at idx
     * @param idx: The ID number of the wanted process
     * @return PCB*
     */
    PCB* operator[]( const unsigned& idx ) const 
    {
        return getPCB( idx );
    }

    /**
     * @brief Add a PCB to the PCBTable at index idx.
     * @param pcb: the PCB to add
     * @param idx: the index to add the PCB at
     */
    void addPCB( PCB *pcb, const unsigned& idx ) ;

    /**
     * @brief Add a new PCB to the PCBTable.
     * @param pid Id of the new PCB
     * @param priority Priority of the new PCB
     * @param idx The index of the new PCB in the PCBTable
     */
    void addNewPCB( const unsigned& pid, 
                    const unsigned& priority, 
                    const unsigned& idx )
    {
        PCB *pcb = new PCB( pid, priority ) ;
        addPCB( pcb, idx ) ;
    }
} ;