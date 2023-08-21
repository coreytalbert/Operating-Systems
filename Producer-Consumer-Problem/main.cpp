/**
 * Assignment 4: Producer Consumer Problem
 * @file main.cpp
 * @author Corey Talbert
 * @brief The main program for the producer consumer problem.
 * @version 0.1
 * @date 11/23/2022
 */
#include "buffer.h"
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

// Signal shared among all threads indicating whether threads should continue or exit.
bool should_run = true ;
// The shared buffer.
Buffer main_buffer ;

/**
 * @brief Producer threads running this function will continuously try to insert
 * an item into the buffer at a random interval between 0 and 1 second, until
 * signaled by the main thread to exit.
 * @param param The item to attempt to insert into the buffer.
 * @return void
 */
void * producer( void * param )
{
    buffer_item item = *( ( int * ) param );
    while ( should_run )
    {
        usleep( rand() % 1000000 ) ;
        if ( not main_buffer.insert_item( item ) )
            printf( "Producer %i found the buffer closed.\n", item );
    }
    pthread_exit( NULL ) ;
}

/**
 * @brief Consumer threads running this function will continuously try to remove
 * an item from the buffer at a random interval between 0 and 1 second, until
 * signaled by the main thread to exit.
 * @param param The location at which to store the item removed from the buffer.
 * @return void
 */
void * consumer( void * param )
{
    buffer_item item;
    while ( should_run )
    {
        usleep( rand() % 1000000 );
        if ( not main_buffer.remove_item( &item ) )
            printf( "Consumer %i found the buffer closed.\n", *((int*) param) ) ;
    }
    pthread_exit( NULL ) ;
}

int main( int argc , char * argv[] )
{
    /* GET COMMAND LINE ARGUMENTS */
    // The total program time.
    int simulation_time = 0 ;
    // The number of producer threads.
    int quantity_of_producers = 0 ;
    // The number of consumer threads.
    int quantity_of_consumers = 0 ;
    if ( argc != 4
        or ( simulation_time = atoi( argv[ 1 ] ) ) < 0
        or ( quantity_of_producers = atoi( argv[ 2 ] ) ) < 0
        or ( quantity_of_consumers = atoi( argv[ 3 ] ) ) < 0 )
    {
        puts( "Usage: [simulation time] [# of producer threads] [# of consumer threads]" ) ;
        abort() ;
    }

    /* CREATE PRODUCER THREADS */
    // The collection of producer threads.
    pthread_t producers[ quantity_of_producers ] ;
    // The collection of parameters passed to the producer function via 
    // pthread_create. The parameter is the item to be inserted into the buffer.
    int producer_parameters[ quantity_of_producers ] ;
    for ( int index = 0 ; index < quantity_of_producers ; ++ index )
    {
        producer_parameters[ index ] = index + 1;
        pthread_create( &producers[ index ] , NULL , producer , &producer_parameters[ index ] ) ;
    }

    /* CREATE CONSUMER THREADS */
    // The collection of consumer threads.
    pthread_t consumers[ quantity_of_consumers ] ;
    // The collection of parameters passed to the consumer function via 
    // pthread_create. The parameter is the pointer to the item removed from 
    // the buffer.
    int consumer_parameters[ quantity_of_consumers ] ;
    for ( int index = 0 ; index < quantity_of_consumers ; ++ index )
    {
        consumer_parameters[ index ] = index + 1 ;
        pthread_create( &consumers[ index ] , NULL , consumer , &consumer_parameters[ index ] ) ;
    }

    /* MAIN THREAD SLEEP */
    usleep( simulation_time * 1000000 ) ;

    /* CONCLUSION */
    // Tell child threads to stop working.
    should_run = false ;
    // Close the buffer to new modifications. 
    // All threads will be unblocked if necessary and eventually exit.
    main_buffer.close();
    // Wait for all threads to finish.
    for ( int index = 0 ; index < quantity_of_producers ; ++ index )
        pthread_join( producers[ index ] , NULL ) ;
    for ( int index = 0 ; index < quantity_of_consumers ; ++ index )
        pthread_join( consumers[ index ] , NULL ) ;

    return 0 ;
}
