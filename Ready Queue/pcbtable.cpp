/**
 * Assignment 1: priority queue of processes
 * @file pcbtable.h
 * @author Corey Talbert
 * @brief This is the implementation file for the PCBTable class.
 * @date 09-19-2022
 */

#include "pcbtable.h"

/**
 * @brief Construct a new PCBTable object of the given size (number of PCBs)
 * @param size: the capacity of the PCBTable
 */
PCBTable::PCBTable( const int& size ) 
    : table( vector<PCB*>( size ) ) 
{}

/**
 * @brief Destroy the PCBTable object. Make sure to delete all the PCBs in the table.
 */
PCBTable::~PCBTable()
{   
    // Destroy each block in the table
    if ( not this -> table . empty() ) // Maybe don't bother
    {    
        for ( PCB* block : this -> table ) 
        {    
            if ( block ) 
            {   
                //printf( "Deleting PCB id=%u\N", block->getID() ) ;
                delete block ; 
            }
        }           
    }
}

/**
 * @brief Get the PCB at index "idx" of the PCBTable.
 * @param idx: the index of the PCB to get
 * @return PCB*: pointer to the PCB at index "idx"
 */
PCB* PCBTable::getPCB( const unsigned& idx ) const 
{
    //printf( "pcbtable.cpp::getPCB(%u)\n", idx ) ;
    return idx < table . size() // Verify parameter
        ? table[ idx ] // Either get pointer, or...
        : ( printf( "\n%u is an invalid index.", idx ), nullptr ) ; // bad index
}

/**
 * @brief Add a PCB to the PCBTable.
 * @param pcb: the PCB to add
 */
void PCBTable::addPCB( PCB* pcb, const unsigned& idx ) 
{
    //printf( "pcbtable.cpp::addPCB(0x%p, %u)\n", pcb, idx ) ;
    this -> table[ idx ] = pcb ;
}