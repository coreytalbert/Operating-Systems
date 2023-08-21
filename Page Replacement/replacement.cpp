/**
 * Assignment 5: Page replacement algorithms
 * @file replacement.cpp
 * @author Corey Talbert
 * @brief A base class for different page replacement algorithms.
 * @version 0.1
 * @date 12/9/2022
 */

#include <iostream>
#include "replacement.h"

/**
 * @brief Constructor.
 * @param num_pages Total number of logical pages for the simulation.
 * @param num_frames Total number of available free frames.
 */
Replacement::Replacement( const int & num_pages , const int & num_frames ) : page_table( num_pages )
{
    for ( int index = 0 ; index < num_frames ; ++index )
        free_frames.push_back( index ) ;
}

/**
 * @brief Simulate a single page access.
 * @details If the page is valid, it calls the touch_page function.
 *          If the page is not valid but free frames are available, it calls the load_page function.
 *          If the page is not valid and there is no free frame, it calls the replace_page function.
 * @param page_num The logical page number.
 * @param is_write whether this access a memory write
 * @return whether it's a page fault: true if it's a page fault
 */
bool Replacement::access_page( const int & page_num , const bool & is_write )
{
    bool access_is_page_fault = false ;
    ++ page_references_quantity ;
    switch ( this->page_table[ page_num ].valid )
    {
        case true:
            // If the page is valid, it calls the touch_page function. 
            touch_page( page_num ) ;
            break ;

        case false:
            access_is_page_fault = true ;
            ++ page_faults_quantity ;
            // If the page is not valid but free frames are available, 
            // it calls the load_page function.
            if ( this->free_frames.size() > 0 )
                load_page( page_num ) ;
            // If the page is not valid and there is no free frame, 
            // it calls the replace_page function.
            else
            {
                ++ page_replacements_quantity ;
                replace_page( page_num ) ;
            }
            
    }
    page_table[page_num].reference_number = this->page_references_quantity ;
    return access_is_page_fault ;
}

/**
 * @brief Print the statistics of simulation.
 */
void Replacement::print_statistics() const
{
    std::cout << "Number of references: \t\t" << this->page_references_quantity
        << "\nNumber of page faults: \t\t" << this->page_faults_quantity
        << "\nNumber of page replacements: \t" << this->page_replacements_quantity
        << "\n" ;
}