/* Assignment 2: Question 2
 * October 11, 2017 
 * Cameron Novina, 991400962
 * Program copies information from one file into another. Program takes two
 * arguments, the first is the source file to copy, the second id the
 * destination file to write to. The program uses pipes to accomplish this
 * as specified in the assignment 2 specifications.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int filecopy(char *inputfile, char *outputfile);

int main (int argc, char *argv[]){
        
    //Check to make sure the user has provided enough arguments.
    if(argc != 3){
        fprintf(stderr,"%s %s %d\n","Incorrect number of arguments.", "Expected 2 arguments; Received ", (argc-1));
        printf("%s","usage: filecopy 'inputfile' 'outputfile'\n");
        return -1;
    }
    
    //If enough arguments have been provided call the function and pass it the arguments.
    else{
        filecopy(argv[1], argv[2]);
    }
    return(0);
}

//Primary function of the program
int filecopy(char *inputfile, char *outputfile){    
    char buffer[100];           //Buffer between the read location and write location.
    int fd[2], src, dst;        //Pipe type(read/ write), file descriptors of the source and destination files.
    pid_t pid;                  //Process Id after the fork command. Will differ between the child and the parent.

    //Creation of the pipe.
    if(pipe(fd) == -1){
        perror("Pipe creation failed.");   
    }
    
    //Opening the source and destination files with required permissions.
    src = open(inputfile, 0);
    dst = open(outputfile, O_RDWR|O_CREAT|O_APPEND, 0666);

    //Making sure that the file opened correctly.
    if(src == -1 || dst == -1){
        perror("One or more files failed to open.");
        exit(1);
    }
    
    //Creation of the child process through fork. Child is assigned a pid of 0.
    //Child receives copy of entire address space of parent.
    pid = fork();
    
    if(pid == -1){
        perror("Process creation failed");
        exit(1);
    }

    //Now both processes will run independent of one another we need to separate the code
    //each process is allowed to run. If the process is the child (pid=0) run this code. 
    if(pid == 0){
        
        //The child will be writing the copied data to a new file (or appending to an existing file).
        //We are receiving from the parent so we close the writing end of the pipe.
        close(fd[1]);

        
        //While we can still read from the pipe (fd[0] is treated like a file) with the buffer that
        //we provided and data is still within the buffer, write that data buffer to the destination
        //file.
        while(read(fd[0], buffer, sizeof(buffer)) > 0){
            if(write(dst, buffer, strlen(buffer)+1) == -1){
                perror("Write failed");
            }
        }
        
        //Close the reading end of the pipe on the child and closing the file once we are finished.
        close(fd[0]);
        close(dst);
    }

    //We don't know the pid of the parent (we could find it if we wanted to) so else will suffice.
    else{

        //The parent will be reading the code from the source file.
        //We are transmitting to the child so we close the reading end of the pipe.
        close(fd[0]);    
        
        //While we can still read from the source file with the buffer that we provided and data
        //is still within the buffer, write that data to the pipe (fd[1] is treated like a file).
        //Memset clears the buffer once data is finished transmitting.
        while(read(src, buffer, sizeof(buffer)) > 0){
            if(write(fd[1], buffer, sizeof(buffer)) == -1){      
                perror("Write failed");
            }
            memset(buffer, 0, 100);
        }

        //Close the writing end of the pipe on the parent and closing the file once we are finished.
        //Wait for the child process to complete before resuming.
        close(fd[1]);
        close(src);
        wait(NULL);
    }

    return(0);
}
