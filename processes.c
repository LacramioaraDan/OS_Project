#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include<dirent.h>
 
 void process(){
 pid_t child_pid;

    // Create a child process
    child_pid = fork();

    if (child_pid < 0) {

        perror("Error!");
        return 1;

    } 
    else if (child_pid == 0) {

        // Child process
        printf("Child process\n");
        // Add child process code here


    } 
    else {
        
        // Parent process
        printf("Parent process\n");
        // Add parent process code here

    }
 }