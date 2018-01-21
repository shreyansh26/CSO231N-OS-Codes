#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {    
    int pid;
    pid=getpid();
    
    printf("Current Process ID is : %d\n",pid);

    printf("[ Forking Child Process ... ] \n");    
    pid=fork(); /* This will Create Child Process and
               Returns Child's PID */
    if(pid < 0) {
        /* Process Creation Failed ... */

        exit(-1);
    }
    else if(pid==0) {
        /* Child Process */

        printf("Child Process Started ...\n");
        printf("Current Process ID is : %d\n",getpid());
        sleep(10);
        printf("\nOrphan Child's Parent ID : %d\n",getppid());
        printf("Child Process Completed ...\n");
        
    }
    else {
        /* Parent Process */

        sleep(10);
        printf("Parent Process Running ... \n");
        printf("I am In Zombie State ...\n");
        printf("Current Process ID is : %d\n",pid);
        while(1)
        {
            /* 
                Infinite Loop that Keeps the
                   Process Running
            */

        }
    }    
    return 0;
}