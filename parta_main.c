#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    if (strcmp(argv[1], "rr") == 0) {
        // Need at least 1 burst after the algorithm name
        if (argc <= 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        //set quantum to the int of second arg
        int quantum = atoi(argv[2]);
        printf("Using RR(%d)\n\n", quantum);

        int plen = argc - 3;
        //create array of bursts
        int* bursts = malloc(sizeof(int) * plen);


        //read args into array
        for (int i = 0; i < plen; i++) {
            bursts[i] = atoi(argv[i + 3]); //convert to int from string args
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        struct pcb* procs = init_procs(bursts, plen);

        rr_run(procs, plen, quantum);

        //calculate average wait time
        double avg = 0;
        for (int i = 0; i < plen; i++) {
            avg += procs[i].wait;
        }
        avg /= plen;

        //print wait time
        printf("Average wait time: %.2f\n", avg);

        //free arrays
        free(bursts);
        free(procs);
        return 0;
    }
    else if (strcmp(argv[1], "fcfs") == 0) {
        // Need at least 1 burst after the algorithm name
        if (argc <= 2) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        printf("Using FCFS\n\n");

        int plen = argc - 2;
        //create array of bursts
        int* bursts = malloc(sizeof(int) * plen);


        //read args into array
        for (int i = 0; i < plen; i++) {
            bursts[i] = atoi(argv[i + 2]); //convert to int from string args
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        //create array of procs from bursts
        struct pcb* procs = init_procs(bursts, plen);

        //run fcfs
        fcfs_run(procs, plen);

        //calculate average wait time
        double avg = 0;
        for (int i = 0; i < plen; i++) {
            avg += procs[i].wait;
        }
        avg /= plen;


        //print wait time
        printf("Average wait time: %.2f\n", avg);

        //free arrays
        free(bursts);
        free(procs);
        return 0;
    } else {
        printf("ERROR: Missing arguments\n");
    }
    return 0;
}
