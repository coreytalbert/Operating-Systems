/**
 * Assignment 5: Page replacement algorithms
 * @file lifo_replacement.h
 * @author Corey Talbert
 * @brief A class implementing the LIFO page replacement algorithms
 * @version 0.1
 * @date 12/9/2022
 */
#pragma once
#include "replacement.h"
#include <stack>

/**
 * @brief A class to simulate LIFO (last in first out) page replacement algorithm.
 */
class LIFOReplacement : public Replacement
{
private:
    // The last-in-first-out ordered list of pages for determining replacement.
    std::stack<int> page_load_order ;

public:
    /**
     * @brief Constructor
     * @param num_pages Total number of available free frames.
     * @param num_frames Total number of free frames.
     */
    LIFOReplacement( const int & num_pages , const int & num_frames ) ;

    /**
     * @brief Access an invalid page, but free frames are available.
     * Assign the page to an available  frame, not replacement needed
     * @param page_num The logical page number.
     */
    virtual void load_page( int page_num ) ;

    /**
     * @brief Access an invalid page, and there is no free frame.
     * Replace the page with the page that has been in memory the longest.
     * @param page_num The logical page number.
     * @return Selected victim page #
     */
    virtual int replace_page( int page_num ) ;
} ;
