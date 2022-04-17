/**
 * @file allocate.c
 * @brief continuously allocate blocks of memory as specified by arguments
 *
 * This program takes in command line arguments of allocation size, 
 * allocation count, and delay time. Then it makes allocations accordingly, 
 * with an option of printing allocation status in the process. 
 * After allocation is done, the program attempts to free all allocated 
 * memory, provided that the system does not crash at somepoint during 
 * allocations.
 *
 * The main purpose of this program is to test the memory behavior of any 
 * computer. Specifically, I wrote this program to test out the swapfile 
 * behaviors of my personal laptop.
 *
 * @author Owen Li <tianwei2@andrew.cmu.edu>
 */

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<getopt.h>

static bool verbose = false;

static const size_t MB = 1024l * 1024l;

/**
 * @brief prints a help message.
 */
void print_help() {
    printf("Usage: allocate SIZE [COUNT] [TIME]\n");
    printf("creates COUNT (default: 1) memory allocations of SIZE, with "
            "[TIME] (default: 1) sec of sleep between each allocation.\n\n");

    printf("  -m \t specifies the size (mb) of allocations\n");
    printf("  -c \t specifies the number of allocations\n");
    printf("  -t \t specifies the delay in secs betwen allocations\n");
    printf("  -v \t verbose mode\n");
    printf("  -h \t display this message and exit\n");
}

/**
 * @brief prints out information about total allocation size.
 *
 * @param[in] total The total amount of allocated memory in bytes.
 */
void print_alloc(size_t total) {
    if (verbose) {
        double mbs = total / (double) MB;
        printf("Total allocated size: %.2f M\n", mbs);
    }
}

/**
 * @brief main function. parses commandline arguments and makes allocations.
 */
int main(int argc, char **argv) {

    char opt;
    size_t m = 0;
    size_t c = 1;
    size_t t= 1;
    size_t argcount = 0;

    // parses command line arguments.
    while ((opt = getopt(argc, argv, "m:c:t:vh")) != -1) {
        switch(opt) {
            case 'm':
                argcount++;
                m = atoi(optarg);
                break;

            case 'c':
                argcount++;
                c = atoi(optarg);
                break;

            case 't':
                argcount++;
                t = atoi(optarg);
                break;

            case 'v':
                verbose = true;
                break;

            case 'h':
                print_help();
                return 0;

            default:
                printf("Invalid argument: %c\n", opt);
                return 1;
        }
    }

    // checks allocation size
    if(m <= 0l) {
        printf("Invalid argument: allocation size must be positive! \n");
        exit(1);
    }

    // checks allocation count
    if(c <= 0l) {
        printf("Invalid argument: allocation count must be positive! \n");
        exit(1);
    }

    // checks delay time
    if(t <= 0l) {
        printf("Invalid argument: delay time must be positive! \n");
        exit(1);
    }

    if (verbose)
        printf("Making %lu allocations, each with size %luM, with %lu secs"
                " delay in between\n\n", c, m, t);

    // initialize array of memory-block pointers.
    if (verbose) 
        printf("Creating array of memory block pointers\n");
    void **ptrarr = (void *) calloc(sizeof(void *), c);
    sleep(t);
    size_t total = sizeof(ptrarr) * c;
    size_t allocsize = sizeof(char) * m * MB;
    print_alloc(total);

    if (verbose) 
        printf("Allocation starts:\n----------\n");

    for (int i = 0; i < c; i++) {
        ptrarr[i] = malloc(allocsize);
        // write bytes to make sure system mem-usage takes place.
        memset(ptrarr[i], 233, allocsize);
        sleep(t);
        total += allocsize;
        print_alloc(total);
    }

    if (verbose) 
        printf("\nDeallocation starts:\n----------\n");

    while (c) {
        c--; 
        total -= allocsize;
        free(ptrarr[c]);
        sleep(t);
        print_alloc(total);
    }
    
    if(verbose) 
        printf("Freeing array of memory block pointers\n");
    free(ptrarr);
    sleep(t);
    print_alloc(total);

    return 0;
}
