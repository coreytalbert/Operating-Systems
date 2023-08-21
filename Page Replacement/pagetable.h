/**
 * Assignment 5: Page replacement algorithms
 * @file pagetable.h
 * @author Corey Talbert
 * @brief This class represents a traditional pagetable data structure.
 * @version 0.1
 * @date 12/9/2022
 */

#pragma once

#include <vector>
#include <cstdio>


// A page table entry.
/**
 * @brief A page table entry. This class is used to represent a page table entry.
 * @details Each page has one entry in the page table. It contains the following fields:
 * - frame number
 * - valid bit
 */
class PageEntry
{
public:
    // Physical frame number for a given page
    int frame_num = -1 ;
    // Valid bit represents whether a page is in the physical memory.
    bool valid = false ;
    int reference_number = 0 ;
} ;


/**
 * @brief A page table is like an array of page entries.
 * The size of the page table should equal to the number of pages in logical memory.
 */
class PageTable
{
private:
    // A page table is like an array of page entries.
    std::vector<PageEntry> pages;

public:
    /**
     * @brief Constructor.
     * @param num_pages The number of page entries in the table.
     */
    PageTable( const int & num_pages = 0 ) ;
    
    /**
     * @brief Destructor.
     */
    ~PageTable() {}

    /**
     * @brief Access a page in the page table.
     * @param i The page number.
     * @return The PageEntry in pages at index i.
     */
    PageEntry & operator [] ( const int &i )
    {
        return pages[ i ];
    }
};