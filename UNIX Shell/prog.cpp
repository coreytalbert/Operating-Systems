/**
 * Assignment 2: Simple UNIX Shell
 * @file prog.cpp
 * @author Corey Talbert
 * @brief This is the main function of a simple UNIX Shell.
 * @version 1.0
 * @date 10-10-2022
 */
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;



/**
 * @brief This class represents the text of a command, the methods by which it
 * is parsed, and the characteristics of its functionality.
 */
class Command ;

/**
 * @brief Handles the history command "!!", restoring the previous command,
 * if any.
 * @param The invoking command object.
 * @return True if there is a command store in history, otherwise false.
 */
bool historySetup( Command & ) ;

/**
 * @brief Handles commands that construct a pipe, spawning two child process
 * that communicate via a UNIX pipe.
 * @param  The invoking command object.
 */
void nonPipedCommand( Command & ) ;

/**
 * @brief Handles commands without inter-process communication, including those
 * with input/output redirection.
 * @param The invoking command object.
 */
void pipedCommand( Command & ) ;



// The maximum length of a command.
#define MAX_LINE 80 
// The maximum number of tokens allowed.
#define MAX_ARGS MAX_LINE / 2 + 1
// The array index for the read file descriptor returned by pipe().
#define READ_END 0
// The array index for the write file descriptor returned by pipe().
#define WRITE_END 1
// The token delimiters, used for splitting a command line.
const char * DELIMITERS = " \n" ;
// Stores the last command entered.
Command * _last_command = nullptr ;



class Command {
private:
    // The text of the command entered by the user.
    char raw[ MAX_LINE ] ;
    // The length of the text of the command.
    unsigned line_length = 0 ;
    // The vector of command tokens generated by the parsing routine.
    char * args[ MAX_ARGS ] ;
    // The number of tokens in the token array.
    unsigned arg_count = 0 ;
    // Defines the command as terminating the shell.
    bool is_exit_command = false ;
    // Defines the command as recalling the command stored in history..
    bool is_repeat_command = false ;
    // Defines the command as causing the shell to wait for execution to complete.
    bool invokes_blocking_process = true ;
    // Defines the command as directing its output to a file.
    bool is_redirect_out = false ;
    // Defines the command as receiving input from a file.
    bool is_redirect_in = false ;
    // Indicates the index of the redirection pathname in the token vector args[].
    unsigned redirect_path_index = -1 ;
    // Defines the command as constructing a pipe.
    bool piped = false ;
    // Used to store the text of the producer portion when the command has a pipe.
    char producer_line[ MAX_LINE ] ;
    // Used to store the text of the consumer portion when the command has a pipe .
    char consumer_line[ MAX_LINE ] ;
    // Used when the command has a pipe to point the producing process to the consuming process.
    Command * pipe_to = nullptr ;

    /**
     * @brief Splits a command containing the pipe symbol into distinct producer
     * and consumer Commands.
     * @param input The command text.
     */
    void buildPipe( char *& input ) ;

    /**
     * @brief The starting point of the command text parsing routine. It
     * handles the history feature, the built-in exit command, and calls
     * the buildPipe(), sanitize(), and splitTokens() subroutines.
     * @param input The command text.
     */
    void parse( char input[] ) ;

    /**
     * @brief Removes the newline character at the end of the command text,
     * and handles the concurrency feature, stripping the ampersand from the
     * line.
     * @param input The command text.
     */
    void sanitize( char *& input ) ;

    /**
     * @brief Splits the command text into tokens, separated by the characters
     * defined by the constant string DELIMITERS, and sets the members involved
     * in input/output redirection, if needed.
     * @param input The command text.
     */
    void splitTokens( char *& input ) ;

public:
    /**
     * @brief Copy constructor.
     * @param c The command to be copied.
     */
    Command( Command * c ) ;

    /**
     * @brief A constructor that generates a command from a command line string.
     * @param input The command text.
     */
    Command( const char input[] ) ;

    /**
     * @brief Returns a token in the token vector at index n.
     * @param n An index in the token vector.
     * @return The string in the token vector at index n.
     */
    char * argn( const unsigned & n ) const { return this->args[ n ] ; }

    /**
     * @brief Get the vector containing the parsed tokens of the command text.
     * @return The token vector.
     */
    char ** getArgs() { return this->args ; }

    /**
     * @brief Get the number of tokens in the token vector args[].
     * @return The number of tokens in args[].
     */
    unsigned getArgCount() const { return this->arg_count ; }

    /**
     * @brief Get the consumer Command to which a piped producer command sends
     * its input.
     * @return The pointer to a consumer Command that uses this Command's
     * output.
     */
    Command * getPipedTo() { return this->pipe_to ; }

    /**
     * @brief Get the command text.
     * @return The command text.
     */
    char * getRawLine() { return this->raw ; }

    /**
     * @brief Get the string containing the pathname of the file used in
     * input/output redirection.
     * @return The pathname of the redirection file.
     */
    char * getRedirectFileToken() const
    {
        return ( this->is_redirect_out or this->is_redirect_in )
            ? this->args[ redirect_path_index ]
            : nullptr ;
    }

    /**
     * @brief Get the concurrency blocking state.
     * @return True if the process blocks its parent, this shell, otherwise
     * false.
     */
    bool isBlocking() const { return this->invokes_blocking_process ; }

    /**
     * @brief Get the empty state of the token vector args[].
     * @return True if there are no tokens in args[], otherwise false.
     */
    bool isEmpty() const { return this->args[ 0 ] == nullptr ; }

    /**
     * @brief Get the exit command state.
     * @return True if the command terminates the shell, false otherwise.
     */
    bool isExit() const { return this->is_exit_command ; }

    /**
     * @brief Get the piping state.
     * @return True if the command creates a pipe between processes, false
     * otherwise.
     */
    bool isPiped() const { return this->piped ; }

    /**
     * @brief Get the input redirection state.
     * @return True if the command careates a process that takes input from a
     * file, otherwise false.
     */
    bool isRedirectIn() const { return this->is_redirect_in ; }

    /**
     * @brief Get the output redirection state.
     * @return True if the command creates a process that sends its output to a
     * file, otherwise false.
     */
    bool isRedirectOut() const { return this->is_redirect_out ; }

    /**
     * @brief Get the history recall state.
     * @return True if the command invokes the command stored in history,
     * otherwise false.
     */
    bool isRepeat() const { return this->is_repeat_command ; }
} ;



void Command::buildPipe( char *& input )
{
    if ( input and strstr( input , " | " ) )
    {
        // ZERO THE STRINGS
        memset( this->producer_line , 0 , MAX_LINE ) ;
        memset( this->consumer_line , 0 , MAX_LINE ) ;

        // COMPUTE LINE LENGTHS
        size_t producer_line_length = strcspn( input , "|" ) - 1 ;
        size_t consumer_line_length = strcspn( input + ( producer_line_length + 3 ) , "\n" ) ;

        // COPY COMMAND STRINGS TO SPECIAL STORAGE
        memmove( this->producer_line , input , producer_line_length ) ;
        memmove( this->consumer_line , input + ( producer_line_length + 3 ) , consumer_line_length ) ;
        printf( "pipe split: producer = %s, consumer = %s\n" , this->producer_line , this->consumer_line ) ;

        // SEPARATE PRODUCCER AND CONSUMER COMMANDS
        /* The original command text is preserved in the class member raw,
         * but from now on parsing will only be done on the producer portion.
         * A second Command objecting representing the consumer proccess is
         * generated.
         */
        input = this->producer_line ;
        this->pipe_to = new Command( this->consumer_line ) ;
        this->piped = true ;
        this->pipe_to->piped = true ;
    }
}



void Command::sanitize( char *& input )
{
    if ( input )
    {
        // Position of "&\0" in command, if any.
        char * ampersand_pos = strpbrk( input , "&" ) ;
        // Update wait signal if '&' is found at the end, 
        // and replace ampersand character with null terminator, if any.
        this->invokes_blocking_process = ampersand_pos
            ? strncpy( ampersand_pos , ( char * ) "" , 1 ) , 0
            : 1 ;

        // The length of the command string up to "\\n\0".
        size_t newline_pos = strcspn( input , "\n" ) ;
        memmove( input , input , newline_pos ) ;
    }
}



void Command::splitTokens( char *& input )
{
    char * token = input;
    // END AT FIRST NULL TOKEN
    while ( token != nullptr and strcmp( token , "" ) != 0 )
    {
        // GET TOKEN AND STORE IT
        token = strtok( ( this->arg_count == 0 ? input : nullptr ) , DELIMITERS ) ;
        args[ this->arg_count ] = token ;
        //printf( "args[ %u ]: %s\n", this -> arg_count, args[ this -> arg_count ] ) ; 

        if ( token != nullptr )
        {
            // CHECK FOR REDIRECT
            if ( strcmp( token , ">" ) == 0 ) // output
            {
                // The null character prevents execv..() from processing 
                // any following tokens.
                args[ this->arg_count ] = ( char * ) '\0' ;
                this->is_redirect_out = true ;
                this->redirect_path_index = this->arg_count + 1 ;
            }
            else if ( strcmp( token , "<" ) == 0 ) // input
            {
                args[ this->arg_count ] = ( char * ) '\0' ;
                this->is_redirect_in = true ;
                this->redirect_path_index = this->arg_count + 1 ;
            }
            // UPDATE COUNT
            ++ this->arg_count ;
        }

    } // End while
    //printf( "token_count: %u\n", this -> arg_count ) ;
}



void Command::parse( char input[] )
{
    // Repeat the last command
    if ( input != nullptr and strcmp( input , "!!\n" ) == 0 )
    {
        this->is_repeat_command = true ;
    }
    // Parse the new command
    else
    {
        buildPipe( input ) ;
        sanitize( input ) ;
        splitTokens( input ) ;

        // BUILT-IN EXIT COMMAND
        if ( this->arg_count == 1 and strcmp( this->args[ 0 ] , "exit" ) == 0 )
            this->is_exit_command = true ;

        // Save the new command to history, if any
        if ( not is_repeat_command and this->arg_count > 0 )
        {
            if ( _last_command != nullptr )
                delete _last_command ;
            _last_command = new Command( this ) ;
        }
    } // End else    
} // End of parse()



Command::Command( Command * c )
{
    memcpy( ( void * ) this , c , sizeof( Command ) ) ;
}



Command::Command( const char input[] )
{
    strncpy( this->raw , input , MAX_LINE ) ;
    line_length = strlen( this->raw ) ;
    parse( this->raw ) ;
}



bool historySetup( Command & com )
{
    // There is a command in the history, echo and restore it
    if ( _last_command != nullptr )
    {
        com = *_last_command ;
        for ( unsigned i = 0 ; i < com.getArgCount() ; ++ i )
            printf( "%s " , com.argn( i ) ) ;
        putchar( '\n' ) ;
        return true ;
    }
    // There is no prior command, start over
    else
    {
        puts( "No command history found." ) ;
        return false ;
    }
}



void nonPipedCommand( Command & com )
{
    pid_t pid = fork();
    /* error occurred */
    if ( pid < 0 )
    {
        fprintf( stderr , "Fork Failed" ) ;
        abort() ;
    }
    /* parent process */
    else if ( pid > 0 )
    {
        // This command causes the parent process to wait for the command to complete.
        if ( com.isBlocking() )
        {
            wait( NULL ) ;
            puts( "Child Complete" ) ;
        }
        else
            puts( "Non-blocking command..." ) ;
    }
    /* child process */
    else
    {
        // This command reads input from a file.
        if ( com.isRedirectIn() )
        {
            puts( "Redirect in..." ) ;
            int input_file = open( com.getRedirectFileToken() , O_RDONLY ) ;
            if ( input_file == -1 )
            {
                puts( "File could not be opened..." ) ;
                printf( "errno = %i\n" , errno ) ;
                abort() ;
            }
            dup2( input_file , STDIN_FILENO ) ;
            execvp( com.argn( 0 ) , com.getArgs() ) ;
            close( input_file ) ;
        }
        // This command writes its output to a file.
        else if ( com.isRedirectOut() )
        {
            puts( "Redirect out..." ) ;
            int output_file = open( com.getRedirectFileToken() , O_CREAT | O_WRONLY | O_TRUNC , 00777 ) ;
            dup2( output_file , STDOUT_FILENO ) ;
            execvp( com.argn( 0 ) , com.getArgs() ) ;
            close( output_file ) ;
        }
        // Plain old command without redirection.
        else
        {
            if ( execvp( com.argn( 0 ) , com.getArgs() ) == -1 )
            {
                puts( "Command not found" ) ;
            }
        }
    }
}



void pipedCommand( Command & producer )
{
    int file_descriptors[ 2 ] ;
    if ( pipe( file_descriptors ) == -1 )
    {
        fprintf( stderr , "Pipe failed" ) ;
        abort() ;
    }

    Command * consumer = producer.getPipedTo() ;
    pid_t producer_pid = -1;
    pid_t consumer_pid = -1;
    // int log = open("log.txt", O_CREAT | O_RDWR | O_APPEND ) ;
    // system("chmod +rw log.txt") ;
    // dup2( log , 1 ) ;

    /* FORK TO PRODUCER PROCESS */
    producer_pid = fork() ;
    // ERROR
    if ( producer_pid == -1 )
    {
        fprintf( stderr , "Fork Failed" ) ;
        abort() ;
    }
    // PRODUCER PROCESS
    else if ( producer_pid == 0 )
    {
        // SETUP WRITE TO PIPE
        //puts( "Starting producer child process..." ) ;
        close( file_descriptors[ READ_END ] ) ; /* PRODUCER  will not use the READ_END */
        dup2( file_descriptors[ WRITE_END ] , STDOUT_FILENO ) ; /* STDOUT copies to pipe WRITE_END */
        close( file_descriptors[ WRITE_END ] ) ;
        //puts( "Producer about to execute " ) ;

        if ( execvp( producer.argn( 0 ) , producer.getArgs() ) == -1 )
        {
            puts( "Producer command not found." ) ;
            printf( "errno = %i\n" , errno ) ;
            printf( "raw = %s\narg count=%u" , producer.getRawLine() , producer.getArgCount() ) ;
        }
    }
    // PARENT
    else if ( producer_pid > 0 )
    {
    }


    //puts( "About to fork consumer process" ) ;
    consumer_pid = fork() ;
    // ERROR
    if ( consumer_pid == -1 )
    {
        fprintf( stderr , "Fork Failed" ) ;
        abort() ;
    }
    // CONSUMER PROCESS
    else if ( consumer_pid == 0 )
    {
        // SETUP READ FROM PIPE
        //puts( "Starting consumer child process..." ) ;
        close( file_descriptors[ WRITE_END ] ); /* GRANDCHILD will not use the WRITE_END */
        dup2( file_descriptors[ READ_END ] , STDIN_FILENO ); /* STDOUT copies to pipe WRITE_END */
        close( file_descriptors[ READ_END ] ) ;
        //puts( "Consumer about to execute " ) ;

        if ( execvp( consumer->argn( 0 ) , consumer->getArgs() ) == -1 )
        {
            puts( "Consumer command not found." ) ;
            printf( "errno = %i\n" , errno ) ;
            printf( "%s\n" , consumer->getRawLine() ) ;
        }
    }
    // PARENT
    else if ( consumer_pid > 0 )
    {
    }

    close( file_descriptors[ READ_END ] ) ;
    close( file_descriptors[ WRITE_END ] ) ;
    //close( log ) ;
    wait( NULL ) ;
    wait( NULL ) ;
}



/**
 * @brief The main function of a simple UNIX Shell.
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main( int argc , char * argv[] )
{
    char input[ MAX_LINE ];       // the command that was entered

    while ( 1 )
    {
        usleep( 2000 ) ; // sometimes the following printf happens before the previous process gives its output
        printf( "\u03C4\u03B1sh>" ) ;
        fflush( stdout ) ;
        fgets( input , MAX_LINE , stdin ) ; // Read the input command
        Command new_command( input ) ;


        // Repeat the last command
        if ( new_command.isRepeat() and not historySetup( new_command ) )
        {
            continue;
        }


        if ( new_command.isEmpty() )
        {
            continue ;
        }
        else if ( new_command.isExit() )
        {
            exit( 0 ) ;
        }
        else if ( not new_command.isPiped() )
        {
            nonPipedCommand( new_command ) ;
        }
        else if ( new_command.isPiped() )
        {
            pipedCommand( new_command ) ;
        }
        else
        {
            puts( "You shouldn't be here..." ) ;
        }
    } // End of while


    return 0;
} // End of main()