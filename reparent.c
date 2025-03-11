#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

/*
 * This program demonstrates process reparenting in Unix systems.
 * 
 * In Unix, when a parent process terminates before its children,
 * the children are typically reparented to the init process (PID 1).
 * 
 * This code creates a three-generation process hierarchy:
 * 1. Main process (grandparent)
 * 2. Intermediate process (parent)
 * 3. Child process
 * 
 * The parent process will exit, causing the child to be reparented.
 */

int main() {
    pid_t pid_parent, pid_child;
    
    printf("Grandparent process. PID: %d, PPID: %d\n", getpid(), getppid());
    
    // Create parent process
    pid_parent = fork();
    
    if (pid_parent < 0) {
        perror("Fork failed");
        exit(1);
    }
    
    if (pid_parent == 0) {
        // This is the parent process
        printf("Parent process. PID: %d, PPID: %d\n", getpid(), getppid());
        
        // Create child process
        pid_child = fork();
        
        if (pid_child < 0) {
            perror("Fork failed");
            exit(1);
        }
        
        if (pid_child == 0) {
            // This is the child process
            printf("Child process. PID: %d, PPID: %d\n", getpid(), getppid());
            
            // Sleep to observe reparenting
            printf("Child sleeping for 5 seconds...\n");
            sleep(5);
            
            // After parent exits, check new parent PID
            printf("Child after parent exit. PID: %d, PPID: %d\n", getpid(), getppid());
            
            // Sleep more to let grandparent wait for us
            sleep(3);
            exit(0);
        } else {
            // Parent process will exit, causing child to be reparented
            printf("Parent exiting. Child PID: %d will be reparented.\n", pid_child);
            exit(0);
        }
    } else {
        // Grandparent process
        printf("Grandparent waiting for 10 seconds to observe reparenting...\n");
        sleep(10);
        
        printf("Grandparent exiting.\n");
        return 0;
    }
}