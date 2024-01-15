// assignmnet 4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function used to get the shorted job first
int compare(const void *a, const void *b) {
    int lastElementA = ((int *)a)[2];
    int lastElementB = ((int *)b)[2];
    return lastElementA - lastElementB;
}

int main(int argc, char *argv[]) {
    printf("\n--> main function stating <--\n\n");
    // check if file given in arguments
    if (argc != 2) {
        printf("Need file name, given: %s\n", argv[0]);
        return 1;
    }

    // opening file and check for error
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[1024]; // assume max chars in line will be less then 1024
    int countCases = 1; // counter for number of test cases

    // looping through each line in input file given
    while (fgets(line, sizeof(line), file) != NULL) {
        int numProcess = (strlen(line) / 6); // number of processes in test 
        int cpuTime = 0; // counter to hold cpu time
        int process[numProcess][4]; //[[1, 3, 4], [2, 7, 9], ...] ommit the p for process number
        double totalWaitTime = 0; // holds total wait time for final calc
        double totalTurnAround = 0; // holds total turnaround time for final calc

        printf("Test case #%d: %s", countCases, line);

        // getting each item from line and adding to array
        char * token = strtok(line, " ");
        int count = 0;
        int row = 0;
        while(token != NULL ) {
            // ommiting 'p' from the processes
            if(token[0] == 'p') {
                process[row][count] = atoi(&token[1]);
            } else {
                process[row][count] = atoi(token);
            }
            token = strtok(NULL, " ");
            count++;
            // go to next row
            if(count == 3) {
                row++;
                count = 0;
            }
        }
        // sort the array based on the last element of each inner array
        qsort(process, numProcess, sizeof(process[0]), compare);

        printf("Number of Processes: %d\n", numProcess);
        printf("Process Scheduling Started:\n");

        // total time needed to complete all processes
        int done = 0; 

        // array to hold queue of running process or to be ran
        // initialize array with -1
        int running[numProcess][6];
        for(int i = 0; i < numProcess; i++) {
            running[i][0] = -1; // to be ran or not
            // adding to the running array
            running[i][1] = 0; // current burst total
            running[i][2] = process[i][2]; // burst max
            running[i][3] = 0; // wait time
            running[i][4] = process[i][0]; // p
            running[i][5] = 0; // turnaroundtime
        }

        // cpu runtime 
        for(int i = 0; i < 999; i++) {
            int changed = 0; // 0 if unchaged, 1 if changed output
            printf("CPU Time %d: ", i);

            // checking for arrivals
            for(int j=0; j < numProcess; j++) {
                if(process[j][1] == i) {
                    printf("[P%d Arrived] ", process[j][0]);
                    changed = 1;
                    for(int i = 0; i < numProcess; i++) {
                        if(running[i][4] == process[j][0]) {
                            running[i][0] = 0; // start process run
                            // checking if process is in prosition 0
                            if(i != 0 && running[0][0] != 0) {
                                int tmp[6];
                                for(int y=0; y<6;y++) {
                                    tmp[y] = running[i][y];
                                }
                                // rearanging array
                                for(int x = i; x > 0; x--) {
                                    for(int y=0;y<6;y++) {
                                        running[x][y] = running[x-1][y];
                                    }
                                }
                                // adding tmp to from of array
                                for(int y=0; y<6;y++) {
                                    running[0][y] = tmp[y];
                                }
                            }
                        }
                    } 
                }
            }

            // check if there is a process in running
            if(running[0][0] != -1) {
                changed = 1;
                // process to be ran
                printf("P%d [%d/%d]", running[0][4], running[0][1], running[0][2]);

                // checking if cpu burst is complete
                if(running[0][1] == running[0][2]) {
                    printf("\nProcess P%d completed with Turnaround Time: %d, Waiting Time: %d", running[0][4], running[0][5], running[0][3]);
                    totalWaitTime += running[0][3];
                    totalTurnAround += running[0][5];
                    done += 1;
                    for(int i = 0; i < numProcess-1; i++) {
                        running[i][0] = running[i+1][0];
                        running[i][1] = running[i+1][1];
                        running[i][2] = running[i+1][2];
                        running[i][3] = running[i+1][3];
                        running[i][4] = running[i+1][4];
                        running[i][5] = running[i+1][5];
                    }
                    // print cpu time again and check if cpu time is complete
                    if(done != numProcess) {
                        printf("\nCPU Time %d: P%d [%d/%d]", i, running[0][4], running[0][1], running[0][2]);
                    }
                }
                running[0][1] += 1;
            }

        // adding wait times
        for(int i = 1; i < numProcess; i++) {
            if(running[i][0] != -1) {
                running[i][3] += 1;
            }
        }
        // adding turnaround times
        for (int i = 0; i < numProcess; i++) {
            if(running[i][0] != -1) {
                running[i][5] += 1;
            }
        }     

        // check for no changes
        if(changed == 0) {
            printf("None");
        }
        // end of cpu burst, indent
        printf("\n");
        // done execution
        if(done == numProcess) {
            break; 
        }
    }

    // calculating avg wait time and turnaround
    totalWaitTime = totalWaitTime / numProcess;
    totalTurnAround = totalTurnAround / numProcess;
    printf("Process scheduling completed with Avg Turnaround Time: %.2f, Avg Waiting Time: %.2f\n\n", totalTurnAround, totalWaitTime);

    countCases++; // increment counter
    }

    // close file
    fclose(file);

    // default return
    return 0;
}