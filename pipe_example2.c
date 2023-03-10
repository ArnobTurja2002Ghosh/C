/*
    Code adapted from Figure 3.21-3.22 Ordinary pipe in UNIX.
*/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

//#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int ARRAY_SIZE;

//The variables representing the average, minimum, and maximum values will be stored globally. 
float average;
int min;
int max;

void *avg_func(void *thread_param){
    int * my_array = (int *) thread_param;
    float sum=0;
	

	for(int i=0;i<ARRAY_SIZE;i++)
		{
			sum=sum+my_array[i];
		}
	average=sum/ARRAY_SIZE;
}

void *min_func(void *thread_param){
    int * my_array = (int *) thread_param;
    min=my_array[0];

	for(int i=0;i<ARRAY_SIZE;i++)
		{
			if(my_array[i]<min){
                min=my_array[i];
            }
		}
	//average=sum/ARRAY_SIZE;
	//printf("The minimum value is:%d \n",min);
}

void *max_func(void *thread_param){
    int * my_array = (int *) thread_param;
    max=my_array[0];
    

	for(int i=0;i<ARRAY_SIZE;i++)
		{
			if(my_array[i]>max){
                max=my_array[i];
            }
		}
	//average=sum/ARRAY_SIZE;
	//printf("The maximum value is:%d \n",max);
}
int main(int argc, char *argv[]) {
    int fd[2];
    pid_t pid;
    int BUFFER_SIZE = argc*sizeof(int);

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
        printf("I'm the parent sending");
        ARRAY_SIZE = argc-1;
        int my_array[ARRAY_SIZE];
        for(int i=0;i<ARRAY_SIZE;i++)
	    {
            printf("\t %d", atoi(argv[i+1]));
		    my_array[i] = atoi(argv[i+1]);
	    }
     printf("\n");
        //char write_msg[BUFFER_SIZE] = "Greetings";

        /* write to the pipe */
        //printf("Parent sends %s\n", write_msg);
        write(fd[WRITE_END], my_array, BUFFER_SIZE);

        /* close the WRITE_END of the pipe */
        //close(fd[WRITE_END]);

        /* wait for the child */
        wait(NULL);
        printf("Waited for the child\n");

        printf("I am the parent receiving and printing the result: \n");
        float read_msg[ARRAY_SIZE];
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("The average value is %f \n", read_msg[0]);
        printf("The minimum value is %d \n",(int)read_msg[1]);
        printf("The maximum value is %d \n",(int)read_msg[2]);
    }
    else { /* child process */
        ARRAY_SIZE = argc-1;
        printf("I'm the child receiving and calculating\n");
        //char read_msg[BUFFER_SIZE];
        int read_msg[ARRAY_SIZE];
        /* read from the pipe */
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        max_func((void *) read_msg);
        avg_func((void *) read_msg);
        min_func((void *) read_msg);
        /* close the READ_END of the pipe */
        //close(fd[READ_END]);
        
        printf("I'm the child sending the results \n");
        float my_array[3] = {average, min, max};
        write(fd[WRITE_END], my_array, BUFFER_SIZE);
    }
    return 0;
}