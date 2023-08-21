/**
 * Assignment 4: Producer Consumer Problem
 * @file buffer.h
 * @author Corey Talbert
 * @brief header file for the buffer class
 * @version 0.1
 * @date 11/23/2022
 */
#ifndef ASSIGN4_BUFFER_H
#define ASSIGN4_BUFFER_H

#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
// Define the data type of the buffer items
typedef int buffer_item ;

/**
 * @brief The bounded buffer class. The number of items in the buffer cannot 
 * exceed the size of the buffer.
 */
class Buffer 
{
private:
    // The buffer storage.
    buffer_item * buffer ;
    // Signal indicating that the buffer is allowed to be manipulated. If false,
    // threads accessing the buffer must terminate.
    bool open_for_work = true ;
    // The capacity of the buffer.
    int size = 0 ;
    // The number of items in the buffer.
    int count = 0;
    // The index of the first item in the circular queue.
    int front = 0 ;
    // The index of the first empty space in the circular queue.
    int back = 0 ;
    // Buffer lock.
    pthread_mutex_t mutex ;
    // Wait condition for placing items in the buffer.
    pthread_cond_t has_space ;
    // Wait condition for taking items out of the buffer.
    pthread_cond_t has_items ;

    /**
     * @brief Check if the buffer is empty
     * @return true if the buffer is empty, else false
     */
    bool is_empty();

    /**
     * @brief Check if the buffer is full
     * @return true if the buffer is full, else false
     */
    bool is_full();

    /**
     * @brief Print the buffer. This function is not thread safe and should only
     * be called from mutex protected critical sections of other member functions.
     */
    void print_buffer();

public:
    /**
     * @brief Construct a new Buffer object
     * @param size the size of the buffer
     */
    Buffer( unsigned size = BUFFER_SIZE );

    /**
     * @brief Copy constructor. Cppcheck insisted.
     * @param b The buffer to be copied.
     */
    Buffer( const Buffer & b ) ;

    /**
     * @brief Assignment operator. Copies the RHS buffer to the LHS. Cppcheck insisted.
     * @param b The buffer to be copied
     */
    void operator=( const Buffer & b ) ;

    /**
     * @brief Destroy the Buffer object
     */
    ~Buffer();

    /**
     * @brief Signals that the buffer may no longer be manipulated. Threads
     * waiting to manipulate the buffer are unblocked and must quit. Threads in 
     * the critical sections of remove_item() and insert_item() will still 
     * finish their work before quitting.
     */
    void close() ;

    /**
     * @brief Insert an item into the buffer
     * @param item the item to insert
     * @return true if successful
     * @return false if not successful
     */
    bool insert_item( buffer_item item );

    /**
     * @brief Remove an item from the buffer
     * @param item the item to remove
     * @return true if successful
     * @return false if not successful
     */
    bool remove_item( buffer_item * item );
};
#endif //ASSIGN4_BUFFER_H
