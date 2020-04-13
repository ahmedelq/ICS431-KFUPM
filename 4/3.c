
#include<stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    char* name;
    double arrivalTime;
    double executionTime;
    double inturruptTime;
    double IOTime;
    double totalTime;
    unsigned int priority;
} Process;

Process* init_process(char* name, double arrivalTime, double executionTime, double inturruptTime, double IOTime, double totalTime, unsigned int priority ){
    Process *proc = (Process *) malloc(sizeof(Process));
    proc->name = name;
    proc->arrivalTime=arrivalTime;
    proc->executionTime=executionTime;
    proc->inturruptTime=inturruptTime;
    proc->IOTime = IOTime;
    proc->totalTime = totalTime;
    proc->priority = priority;
    return proc;
}

Process prc[10];

int main(int argc, char *argv[]){
    char* test = "P1";
    Process* p1 =  init_process(test, 0, 20.0, 1.5, 5.0, 2);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        {
            perror("Error, couldn't read the file!");
            return -1;
        }

    unsigned int i;
    for (i =0; (read = getline(&line, &len, fp)) != -1; i++)  {
        //printf("Retrieved line of length %zu:\n", read);
        //printf("%s", line);
        //printf("%s\n", line);

        char* pname = strtok(line, " ");
        char* arrivalTimeS = strtok(NULL, " ");
        char* executionTimeS = strtok(NULL, " ");
        char* inturruptTimeS = strtok(NULL, " ");
        char* IOTimeS = strtok(NULL, " ");
        char* priorityS = strtok(NULL, " ");
        
        double arrivalTime = atof(arrivalTimeS);
        double executionTime = atof(executionTimeS);
        double inturruptTime = atof(inturruptTimeS);
        double IOTime = atof(IOTimeS);
        unsigned int priority = atoi(priorityS);
        double totalTime = executionTime + inturruptTime + IOTime;
        prc[i] = *init_process(pname, arrivalTime, executionTime, inturruptTime, IOTime, totalTime, priority);        
    }
    fclose(fp);
    if (line)
        free(line);
   // printf("\nTotal lines %u\n", i);





    exit(EXIT_SUCCESS);
}