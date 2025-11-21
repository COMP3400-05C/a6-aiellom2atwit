#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * This function takes an array of CPU bursts, and returns an array of PCBs
 */
struct pcb* init_procs(int* bursts, int blen) {
    //if blen is 0 or less, invalid
    if (blen <= 0) {
        return NULL;
    }
    //create procs array
    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    //if null, return null
    if (procs == NULL) {
        return NULL;
    }
    //initialize for each burst
    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }
    return procs;
}

/**
 * Helper function that prints the current values of each PCB.
 */
void printall(struct pcb* procs, int plen) {
    if (plen <= 0 || procs == NULL) {
        return;
    }
    for (int i = 0; i < plen; i++) {
        printf("P%d: Burst %d", procs[i].pid, procs[i].burst_left);
    }
    return;
}

/**
 * "runs" the current process by reducing its burst_left by amount,
 and also increases the other processes wait by the same amount.
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    //if length is 0 or procs is NULL return
    if (plen <= 0 || procs == NULL) {
        return;
    }
    
    //set t to lesser amount
    int t = amount;
    if (procs[current].burst_left < t) {
        t = procs[current].burst_left;
    }

    //subtract burst_left by calculated t
    procs[current].burst_left -= t;

    //add t to other procs
    for (int i = 0; i < plen; i++) {
        if (i != current && procs[i].burst_left > 0) {
            procs[i].wait += t;
        }
    }
    return;
}

/**
 * fcfs Scheduler
 */
int fcfs_run(struct pcb* procs, int plen) {
    if (plen == 0 || procs == NULL) {
        return -1;
    }

    int time = 0;

    //cycle through every pcb in procs array
    for (int i = 0; i < plen; i++) {
        //run this process until it finishes
        while (procs[i].burst_left > 0) {
            //use run_proc for 1 time unit
            run_proc(procs, plen, i, 1);

            //increase total time elapsed
            time += 1;
        }
    }

    return time;
}

/**
 * return the next process to run
 */
int rr_next(int current, struct pcb* procs, int plen) {
    if (plen == 0 || procs == NULL) {
        return -1;
    }

    //test if every burst_left is complete
    bool allDone = true;
    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left > 0) {
            allDone = false;
        }
    }
    //return -1 if alldone
    if (allDone) {
        return -1;
    }

    //cycle to next pcb until all are complete
    int next = (current + 1) % plen;
    while (procs[next].burst_left == 0) {
        next = (next + 1) % plen;
    }
    return next;
}
/**
 * rr Scheduler
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    if (plen == 0 || procs == NULL || quantum <= 0) {
        return -1;
    }

    //set time and current to 0 to initialize
    int time = 0;
    int current = 0;

    //keep looping until completed
    while (rr_next(current, procs, plen) != -1 || procs[current].burst_left != 0) {
        int t = quantum;
        //set t to burst_left if less than quantum
        if (procs[current].burst_left < t) {
            t = procs[current].burst_left;
        }

        //use run_proc with current and t
        run_proc(procs, plen, current, t);

        //add t to time
        time += t;

        //set next to rr_next value
        int next = rr_next(current, procs, plen);
        //if rr_next returns -1, no more values to complete
        if (next == -1) {
            break;
        }

        //iterate to next
        current = next;
    }

    return time;
}

