/*
    Code adapted from Figure 3.21-3.22 Ordinary pipe in UNIX.
*/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]) {
    printf("The size of an integer is %ld bytes \n", sizeof(int));
    int fd[2];
    pid_t pid;

    /* create the pipe */
    if ( pipe (fd) == -1) {
        fprintf(stderr,"Pipe failed\n");
        return 1;
    }

    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }

    if (pid > 0) { /* parent process */
     printf("\n");
        int arr[10] = {0,1,2,110,116,42,789,2,10,11};
        for(int i=0;i<10;i++)
	    {
            printf("Parent reads %d\n",arr[i]);
        }
        /* write to the pipe */
        //printf("Parent sends %s\n", write_msg);
        write(fd[WRITE_END], arr, 40);

        /* close the WRITE_END of the pipe */
        close(fd[WRITE_END]);

        /* wait for the child */
        wait(NULL);
        printf("Waited for the child\n");
    }
    else { /* child process */
        //char read_msg[BUFFER_SIZE];
        int read_msg[10];
        /* read from the pipe */
        //lectures //slides //2.3
        //■ int fd — the file descriptor to be read
        //■ void *buf — a buffer into which the data will be read
        //■ size t count — the maximum number of bytes to be read into the buffer
        //■ On a successful read, the number of bytes read is returned. A return value of 0 indicates end of file. If an error occurs, read() returns −1
        read(fd[READ_END], read_msg, 40);
        for(int i=0;i<10;i++)
	    {
            printf("Child reads %d\n",read_msg[i]);
        }
        
        /* close the READ_END of the pipe */
        close(fd[READ_END]);
    }
    return 0;
}