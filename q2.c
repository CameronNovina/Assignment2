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
#include <unistd.h>
#include <assert.h>

int filecopy(char *inputfile, char *outputfile);


int main (int argc, char *argv[]){
    //assert(argc == 2);
    if(argc != 2){
        fprintf(stderr,"%s %s %d\n","Incorrect number of arguments.", "Expected 2 arguments; Received ", (argc-1));
        printf("%s","usage: filecopy 'inputfile' 'outputfile'\n");
        return -1;
    }
    
    else{
        filecopy(argv[1], argv[2]);
    }
    return(0);
}


int filecopy(char *inputfile, char *outputfile){    
    FILE *fp;
    char buffer[100];
    int fd[2], src, bytes, dst;

    //pipe(fd);

    fp = fopen(inputfile, "r");
    if(fp = NULL){
        perror("file");
        return(-1);
    }
    fclose(fp);

    return(0);


}
