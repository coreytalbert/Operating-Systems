/**
 * Assignment 5: Page replacement algorithms
 * @file lifo_replacement.cpp
 * @author Corey Talbert
 * @brief A class implementing the LIFO page replacement algorithms
 * @version 0.1
 * @date 12/9/2022
 */

#include "lifo_replacement.h"

/**
 * @brief Constructor
 * @param num_pages Total number of available free frames.
 * @param num_frames Total number of free frames.
 */
LIFOReplacement::LIFOReplacement( const int & num_pages , const int & num_frames )
    : Replacement( num_pages , num_frames )
{}

/**
 * @brief Access an invalid page, but free frames are available.
 * Assign the page to an available frame, no replacement needed.
 * @param page_num The logical page number.
 */
void LIFOReplacement::load_page( int page_num )
{
    // The first free frame is assigned to the page.
    page_table[ page_num ].frame_num = free_frames.front() ;
    free_frames.pop_front() ;
    // The page is validated and pushed to the end of the load order queue.
    page_table[ page_num ].valid = true ;
    page_load_order.push( page_num ) ;
}

/**
 * @brief Access an invalid page, and there is no free frame.
 * Replace the page with the page that has been in memory the longest.
 * @param page_num The logical page number.
 * @return Selected victim page #
 */
int LIFOReplacement::replace_page( int page_num )
{
    // The most recent page in the load order list is selected for replacement.
    int victim_page = page_load_order.top() ;
    page_load_order.pop() ;
    // The old page is invalidated. The new page takes the frame from 
    // the replaced page.
    page_table[ victim_page ].valid = false ;
    int target_frame = page_table[ victim_page ].frame_num ;
    page_table[ page_num ].frame_num = target_frame ;
    page_table[ page_num ].valid = true ;
    page_load_order.push( page_num ) ;
    return victim_page ;
}