/**
 * Assignment 5: Page replacement algorithms
 * @file lru_replacement.h
 * @author Corey Talbert
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 * @date 12/9/2022
 */
#pragma once
#include <deque>
#include "replacement.h"

/**
 * @brief A class to simulate the least recently used (LRU) page replacement algorithm.
 */
class LRUReplacement : public Replacement
{
    std::deque<int> page_access_order ;
public:

    /**
     * @brief Constructor
     * @param num_pages
     * @param num_frames
     */
    LRUReplacement( const int & num_pages , const int & num_frames ) ;

    /**
    * @brief Destructor
    */
    virtual ~LRUReplacement() {}

    /**
     * @brief Access a page already in physical memory.
     * @param page_num The logical page number.
     */
    virtual void touch_page( const int & page_num ) ;

    /**
     * @brief Access an invalid page, but free frames are available.
     * Assign the page to an available frame, no replacement needed
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