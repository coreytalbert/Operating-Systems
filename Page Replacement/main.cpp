/**
 * Assignment 5: Page replacement algorithms
 * @file main.cpp
 * @author Corey Talbert
 * @brief The main function
 * @version 0.1
 * @date 12/9/2022
 */

#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
using namespace std::chrono ;

#include "fifo_replacement.h"
#include "lru_replacement.h"
#include "lifo_replacement.h"

// Check if an integer is power of 2
bool isPowerOfTwo( unsigned int x )
{
    // First x in the below expression is for the case when x is 0
    return x && ( !( x & ( x - 1 ) ) );
}


int main( int argc , char * argv[] )
{
    //Print basic information about the program
    std::cout << "=================================================================" << std::endl;
    std::cout << "CS 433 Programming assignment 5" << std::endl;
    std::cout << "Author: Corey Talbert" << std::endl;
    std::cout << "Date: 12/9/2022" << std::endl;
    std::cout << "Course: CS433 (Operating Systems)" << std::endl;
    std::cout << "Description : Program to simulate different page replacement algorithms" << std::endl;
    std::cout << "=================================================================\n" << std::endl;

    // user does not enter enough parameters
    if ( argc < 3 )
    {
        std::cout << "You have entered too few parameters to run the program.  You must enter" << std::endl
            << "two command-line arguments:" << std::endl
            << " - page size (in bytes): between 256 and 8192, inclusive" << std::endl
            << " - physical memory size (in megabytes): between 4 and 64, inclusive" << std::endl;
        abort() ;
    }

    // Page size and Physical memory size
    // Their values should be read from command-line arguments, and always a power of 2
    unsigned int page_size = atoi( argv[ 1 ] );
    if ( !isPowerOfTwo( page_size ) )
    {
        std::cout << "You have entered an invalid parameter for page size (bytes)" << std::endl
            << "  (must be an power of 2 between 256 and 8192, inclusive)." << std::endl;
        abort() ;
    }

    unsigned int phys_mem_size = atoi( argv[ 2 ] ) << 20; // convert from MB to bytes
    if ( !isPowerOfTwo( phys_mem_size ) )
    {
        std::cout << "You have entered an invalid parameter for physical memory size (MB)" << std::endl
            << "  (must be an even integer between 4 and 64, inclusive)." << std::endl;
        abort() ;
    }

    // CALCULATE NUMBER OF PAGES AND FRAMES
    // 27-bit logical memory (128 MB logical memory assumed by the assignment)
    int logic_mem_bits = 27 ;

    // Num of bits for physical memory addresses, calculated from physical
    // memory size, e.g. 24 bits for 16 MB memory
    int phys_mem_bits = std::log2( phys_mem_size ) ;

    // Num of bits for page offset, calculated from page size,
    // e.g. 12 bits for 4096 byte page
    int page_offset_bits = std::log2( page_size ) ;

    // Number of pages in logical memory = 2^(logic_mem_bits - page_bit)
    int num_pages = 1 << ( logic_mem_bits - page_offset_bits ) ;

    // Number of free frames in physical memory = 2^(phys_mem_bits - page_offset_bits)
    int num_frames = 1 << ( phys_mem_bits - page_offset_bits ) ;

    std::cout << "Page size = " << page_size << " bytes" << std::endl ;
    std::cout << "Physical Memory size = " << phys_mem_size << " bytes" << std::endl ;
    std::cout << "Number of pages = " << num_pages << std::endl ;
    std::cout << "Number of physical frames = " << num_frames << std::endl ;

    // Test 1: Read and simulate the small list of logical addresses from the input file "small_refs.txt"
    std::cout << "\n================================Test 1==================================================\n";

    // Open the small reference file
    std::ifstream in;
    in.open( "small_refs.txt" ) ;
    if ( !in.is_open() )
    {
        std::cerr << "Cannot open small_refs.txt to read. Please check your path." << std::endl ;
        abort() ;
    }

    // Create a vector to store the logical addresses
    int val ;
    std::vector<int> small_refs ;
    while ( in >> val )
        small_refs.push_back( val ) ;

    // Create a virtual memory simulation using FIFO replacement algorithm
    FIFOReplacement vm( num_pages , num_frames );
    for ( std::vector<int>::const_iterator it = small_refs.begin(); it != small_refs.end(); ++it )
    {
        int page_num = ( *it ) >> page_offset_bits;
        bool isPageFault = vm.access_page( page_num , 0 );
        PageEntry pg = vm.getPageEntry( page_num );
        std::cout << "Logical address: " << *it << ", \tpage number: " << page_num;
        std::cout << ", \tframe number = " << pg.frame_num << ", \tis page fault? " << isPageFault << std::endl;
    }

    in.close();
    vm.print_statistics();

    //
    // Test 2: Read and simulate the large list of logical addresses from the input file "large_refs.txt"
    //
    puts( "\n================================Test 2==================================================\n" ) ;

    std::ifstream large_refs_file( "large_refs.txt" );
    if ( not large_refs_file.is_open() )
    {
        std::cerr << "Cannot open large_refs.txt to read. Please check your path." << std::endl ;
        abort() ;
    }

    /* READ THE TEST REFERENCES INTO THE PROGRAM */
    std::vector<int> large_refs ;
    for ( int logical_address = 0 ;
        large_refs_file >> logical_address ;
        large_refs.push_back( logical_address >> page_offset_bits ) ) ;

    
    puts( "****************Simulate FIFO replacement****************************\n" ) ;

    FIFOReplacement fifo( num_pages , num_frames ) ;
    high_resolution_clock::time_point fifo_start_time = high_resolution_clock::now() ;
    for ( std::vector<int>::const_iterator it = large_refs.begin() ; it != large_refs.end() ; ++it )
    {
        int page_num = ( *it ) ;
        fifo.access_page( page_num , 0 ) ;
    }
    high_resolution_clock::time_point fifo_end_time = high_resolution_clock::now() ;
    duration<double> fifo_simulation_run_time = duration_cast< duration<double> >( fifo_end_time - fifo_start_time ) ;
    fifo.print_statistics() ;
    printf( "Elapsed time = %.6f seconds\n" , fifo_simulation_run_time.count() ) ;

    
    puts( "****************Simulate LIFO replacement****************************\n" ) ;

    LIFOReplacement lifo( num_pages , num_frames ) ;
    high_resolution_clock::time_point lifo_start_time = high_resolution_clock::now() ;
    for ( std::vector<int>::const_iterator it = large_refs.begin() ; it != large_refs.end() ; ++it )
    {
        int page_num = ( *it ) ;
        lifo.access_page( page_num , 0 ) ;
    }
    high_resolution_clock::time_point lifo_end_time = high_resolution_clock::now() ;
    duration<double> lifo_simulation_run_time = duration_cast< duration<double> >( lifo_end_time - lifo_start_time );
    lifo.print_statistics() ;
    printf( "Elapsed time = %.6f seconds\n" , lifo_simulation_run_time.count() );

    
    puts( "****************Simulate LRU replacement****************************\n" ) ;
    
    LRUReplacement lru( num_pages , num_frames ) ;
    high_resolution_clock::time_point lru_start_time = high_resolution_clock::now() ;
    for ( std::vector<int>::const_iterator it = large_refs.begin() ; it != large_refs.end() ; ++it )
    {
        lru.access_page( *it , 0 ) ;
    }
    high_resolution_clock::time_point lru_end_time = high_resolution_clock::now() ;
    duration<double> lru_simulation_run_time = duration_cast< duration<double> >( lru_end_time - lru_start_time );
    lru.print_statistics() ;
    printf( "Elapsed time = %.6f seconds\n" , lru_simulation_run_time.count() );
    

}