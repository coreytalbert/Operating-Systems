/**
 * Assignment 5: Page replacement algorithms
 * @file fifo_replacement.cpp
 * @author Corey Talbert
 * @brief A class implementing the FIFO page replacement algorithms
 * @version 0.1
 * @date 12/9/2022
 */

#include "fifo_replacement.h"

/**
 * @brief Constructor.
 * @param num_pages Total number of available free frames.
 * @param num_frames Total number of free frames.
 */
FIFOReplacement::FIFOReplacement( const int & num_pages , const int & num_frames )
    : Replacement( num_pages , num_frames )
{}

/**
 * @brief Access an invalid page, but free frames are available.
 * Assign the page to an available frame, no replacement needed.
 * @param page_num The logical page number.
 */
void FIFOReplacement::load_page( int page_num )
{
    // The first free frame is assigned to the page.
    this->page_table[ page_num ].frame_num = this->free_frames.front() ;
    this->free_frames.pop_front() ;
    // The page is validated and pushed to the end of the load order queue.
    this->page_table[ page_num ].valid = true ;
    this->page_load_order.push( page_num ) ;
}

/**
 * @brief Access an invalid page, and there is no free frame.
 * Replace the page with the page that has been in memory the longest.
 * @param page_num The logical page number.
 * @return Selected victim page #
 */
int FIFOReplacement::replace_page( int page_num )
{
    // The oldest page in the queue is selected for replacement.
    int victim_page = page_load_order.front() ;
    page_load_order.pop() ;
    // The old page is invalidated. The new page takes the frame from 
    // the replaced page.
    page_table[ victim_page ].valid = false ;
    page_table[ page_num ].frame_num = page_table[ victim_page ].frame_num ;
    page_table[ page_num ].valid = true ;
    page_load_order.push( page_num ) ;
    return victim_page ;
}