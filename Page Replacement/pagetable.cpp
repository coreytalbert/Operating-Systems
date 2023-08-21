/**
 * Assignment 5: Page replacement algorithms
 * @file pagetable.cpp
 * @author Corey Talbert
 * @brief This class represents a traditional pagetable data structure.
 * @version 0.1
 * @date 12/9/2022
 */
#include "pagetable.h"

/**
 * @brief Constructor.
 * @param num_pages The number of page entries in the table.
 */
PageTable::PageTable( const int & num_pages )
{
    pages.insert( pages.begin() , num_pages , PageEntry() ) ;
}