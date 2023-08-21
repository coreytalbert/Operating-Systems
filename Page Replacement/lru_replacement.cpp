/**
 * Assignment 5: Page replacement algorithms
 * @file lru_replacement.cpp
 * @author Corey Talbert
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 * @date 12/9/2022
 */
#include "lru_replacement.h"

LRUReplacement::LRUReplacement( const int & num_pages , const int & num_frames )
    : Replacement( num_pages , num_frames )
{}

/**
 * @brief Access a page already in physical memory.
 * @param page_num The logical page number.
 */
void LRUReplacement::touch_page( const int & page_num )
{
    // The last time the page was referenced.
    int last_reference_number = page_table[ page_num ].reference_number ;
    // The index of the page in the access order list.
    int last_page_order_index = 0;
    int list_size = page_access_order.size() ;
    // Binary search of the access order list for the page.
    // The access order list is implicitly monotonically decreasing by reference
    // number--that is, pages are ordered by highest to lowest reference number,
    // which is stored with each entry in the page table. 
    /* Derived from: https://en.wikipedia.org/wiki/Binary_search_algorithm */    
    int reference_distance = 0 ; // The signed difference between the last 
        // reference of the page at index m of the order list and the page being 
        //searched for.  
    int l = 0 ; // The leftmost index of the search range.
    int r = list_size - 1; // The rightmost index of the search range.
    int m ; // The index of between l and r.
    while ( l <= r )
    {
        m = ( l + r ) / 2 ;
        reference_distance = page_table[ page_access_order[ m ] ].reference_number - last_reference_number ;
        if ( reference_distance < 0 )
        {
            r = m - 1 ;
            continue ;
        }
        else if ( reference_distance > 0 )
        {
            l = m + 1;
            continue;
        }
        last_page_order_index = m ;
        break ;
    }
    page_access_order.erase( page_access_order.begin() + last_page_order_index ) ;
    page_access_order.push_front( page_num );
}

/**
 * @brief Access an invalid page, but free frames are available.
 * Assign the page to an available frame, no replacement needed
 * @param page_num The logical page number.
 */
void LRUReplacement::load_page( int page_num )
{
    page_table[ page_num ].frame_num = this->free_frames.front() ;
    this->free_frames.pop_front() ;
    page_table[ page_num ].valid = true ;
    page_access_order.push_front( page_num );
}

/**
 * @brief Access an invalid page, and there is no free frame.
 * Replace the page with the page that has been in memory the longest.
 * @param page_num The logical page number.
 * @return Selected victim page #
 */
int LRUReplacement::replace_page( int page_num )
{
    /* INVALIDATE VICTIM PAGE, RELEASING ITS FRAME. */
    // The last element is the least recently used page.
    int victim_page = page_access_order.back() ;
    page_access_order.pop_back() ;
    page_table[ victim_page ].valid = false ;
    /* TAKE VICTIM PAGE'S FRAME FOR LOADING THE NEW PAGE. */
    page_table[ page_num ].frame_num = page_table[ victim_page ].frame_num ;
    page_table[ page_num ].valid = true ;
    page_access_order.push_front( page_num );
    return victim_page ;
}