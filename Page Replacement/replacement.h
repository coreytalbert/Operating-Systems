/**
* Assignment 5: Page replacement algorithms
 * @file replacement.h
 * @author Corey Talbert
 * @brief A base class for different page replacement algorithms.
 * @version 0.1
 * @date 12/9/2022
 */

#pragma once
#include <list>
#include "pagetable.h"

/**
 * @brief A base class to simulate page replacement algorithms.
 * A specific page replacement algorithm, e.g. FIFO or LRU, should be subclass 
 * of this.
 */
class Replacement
{
protected:
    // The pool of free frames as a FIFO queue.
    std::list<int> free_frames ;
    // The list of pages in virtual memory mapped to frames in physical memory.
    PageTable page_table ;
    // The total page references made fromm the page table.
    int page_references_quantity = 0 ;
    // The total references to pages not yet loaded to physical memory.
    int page_faults_quantity = 0 ;
    // The total number of page replacements made in the page table.
    int page_replacements_quantity = 0 ;

public:
    /**
     * @brief Constructor.
     * @param num_pages Total number of logical pages for the simulation.
     * @param num_frames Total number of available free frames.
     */
    Replacement( const int & num_pages , const int & num_frames ) ;

    /**
     * @brief Destructor
     */
    virtual ~Replacement() {}

    /**
     * @brief Simulate a single page access.
     * @details If the page is valid, it calls the touch_page function.
     *          If the page is not valid but free frames are available, it calls the load_page function.
     *          If the page is not valid and there is no free frame, it calls the replace_page function.
     * @param page_num The logical page number.
     * @param is_write whether this access a memory write
     * @return whether it's a page fault: true if it's a page fault
     */
    virtual bool access_page( const int & page_num , const bool & is_write = false ) ;

    /**
     * @brief Access a page already in physical memory
     * It may be overridden in a subclass
     * @param page_num The logical page number.
     */
    virtual void touch_page( const int & page_num ) {}

    /**
     * @brief Access an invalid page, but free frames are available.
     * Assign the page to an available frame, not replacement needed
     * It may be overridden in a subclass
     * @param page_num The logical page number.
     */
    virtual void load_page( int page_num ) {}

    /**
     * @brief Access an invalid page and no free frame is available.
     * Select a victim page to be replaced.
     * It is a pure virtual function to be implemented in specific replacement subclasses.
     * @param page_num  The logical page number of the desired page.
     * @return Selected victim page #
     */
    virtual int replace_page( int page_num ) = 0 ;

    /**
     * @brief Get the ith entry of the page table
     */
    PageEntry getPageEntry( const int & page_num )
    {
        return this->page_table[ page_num ] ;
    }

    /**
     * @brief Print the statistics of simulation
     */
    void print_statistics() const;
};