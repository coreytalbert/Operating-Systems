/**
 * Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Corey Talbert
 * @brief Implementation file for the buffer class
 * @version 0.1
 * @date 11/23/2022
 */
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <stdlib.h>
#include "buffer.h"

/**
 * @brief Construct a new Buffer object
 * @param size the size of the buffer
 */
Buffer::Buffer( unsigned size )
    : buffer( new buffer_item[ size ] ) , size( size )
{
    memset( this->buffer , 0 , sizeof( buffer_item ) * this->size ) ;
    pthread_mutex_init( &this->mutex , NULL );
    pthread_cond_init( &has_space , NULL );
    pthread_cond_init( &has_items , NULL );
}

/**
 * @brief Copy constructor. Cppcheck insisted.
 * @param b The buffer to be copied.
 */
Buffer::Buffer( const Buffer & b )
{
    memcpy( this , &b , sizeof( b ) ) ;
}

/**
 * @brief Assignment operator. Copies the RHS buffer to the LHS. Cppcheck insisted.
 * @param b The buffer to be copied
 */
void Buffer::operator=( const Buffer & b )
{
    memcpy( this , &b , sizeof( b ) ) ;
}

/**
 * @brief Destroy the Buffer object
 */
Buffer::~Buffer()
{
    pthread_mutex_destroy( &mutex ) ;
    pthread_cond_destroy( &has_space ) ;
    pthread_cond_destroy( &has_items ) ;
    delete this->buffer ;
}

/**
 * @brief Signals that the buffer may no longer be manipulated. Threads waiting 
 * to manipulate the buffer are unblocked and must quit. Threads in the critical
 * sections of remove_item() and insert_item() will still finish their work 
 * before quitting.
 */
void Buffer::close()
{
    // Signal that the buffer is no longer accepting modifications.
    this->open_for_work = false ;
    // Unblock all threads waiting for space to insert an item. Threads waiting
    // on has_space and has_items conditions will return without 
    // modifying the buffer.
    pthread_cond_broadcast( &has_space ) ;
    // Unblock all threads waiting to remove an item. 
    pthread_cond_broadcast( &has_items ) ;
}

/**
 * @brief Insert an item into the buffer
 * @param item the item to insert
 * @return true if successful
 * @return false if not successful
 */
bool Buffer::insert_item( buffer_item item )
{
    bool item_inserted ;
    pthread_mutex_lock( &mutex ) ;
    while ( this->open_for_work and this->is_full() )
    {
        pthread_cond_wait( &has_space , &mutex ) ;
    }
    // CRITICAL SECTION
    // Thread-safe insertion of parameter buffer_item item into the buffer at
    // index this->back.
    if ( this->open_for_work )
    {
        buffer[ back ] = item ;
        ++ count ;
        back = ( back + 1 ) % size ;
        printf( "Producer %i: Inserted item %i\n" , ( int ) item , ( int ) item ) ;
        print_buffer() ;
        pthread_mutex_unlock( &mutex ) ;
        pthread_cond_signal( &has_items ) ;
        item_inserted = true ;
    }
    // If the buffer is closed to modifications then the thread will release the
    // mutex and return without changing the buffer.
    else
    {
        pthread_mutex_unlock( &mutex ) ;
        item_inserted = false ;
    }
    return item_inserted ;
}

/**
 * @brief Remove an item from the buffer
 * @param item the item to remove
 * @return true if successful
 * @return false if not successful
 */
bool Buffer::remove_item( buffer_item * item )
{
    bool item_removed ;
    pthread_mutex_lock( &mutex ) ;
    while ( this->open_for_work and this->is_empty() )
    {
        pthread_cond_wait( &has_items , &mutex ) ;
    }
    // CRITICAL SECTION
    // Thread-safe removal of a single item from the front of the buffer, 
    // to be stored at the location pointed to by parameter buffer_item *item.
    if ( this->open_for_work )
    {
        * item = buffer[ front ] ;
        buffer[ front ] = -1 ;
        -- this->count ;
        front = ( front + 1 ) % this->size ;
        printf( "Consumer Removed item %i\n" , ( int ) *item ) ;
        this->print_buffer() ;
        pthread_mutex_unlock( &mutex ) ;
        pthread_cond_signal( &has_space ) ;
        item_removed = true ;
    }
    // If the buffer is closed to modifications then the thread will release the
    // mutex and return without changing the buffer.
    else
    {
        pthread_mutex_unlock( &mutex ) ;
        item_removed = false ;
    }
    return item_removed ;
}

/**
 * @brief Check if the buffer is empty. Not thread-safe.
 * @return true if the buffer is empty, else false
 */
bool Buffer::is_empty() { return not this->count ; }

/**
 * @brief Check if the buffer is full. Not thread-safe.
 * @return true if the buffer is full, else false
 */
bool Buffer::is_full() { return count == size ; }

/**
 * @brief Print the buffer. This function is not thread safe and should only be
 * called from mutex protected critical sections of other functions.
 */
void Buffer::print_buffer()
{
    /*printf( "\nBuffer @ %p | size = %u | count = %u | front = %i | back = %i\n" ,
        this , this->size , this->count , front , back ) ;*/
    printf( "[" ) ;
    for ( int index = front , iteration = 0 ; iteration < count ; index = ( index + 1 ) % size , ++iteration )
    {
        printf( "%i" , this->buffer[ index ] ) ;
        if ( ( index + 1 ) % this->size != this->back )
            printf( ", " ) ;
    }
    printf( "]\n" ) ;
}