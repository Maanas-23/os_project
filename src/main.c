#include "fifo.h"
#include "lru.h"
#include "utils.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_TESTS 30
#define FRAMES 4
#define RANGES 3 
#define LENGTHS 4

typedef struct {
    int length;
    int range;
    int *frames;
    FILE *f;
} ThreadParams;

void run_tests(FILE *f, int *frames, int *ranges, int *lengths) {
    fprintf(f, "Frames, Page Range, Page String Length, FIFO Page Faults, LRU Page Faults\n");

    for (int j = 0; j < RANGES; ++j) {
        for (int k = 0; k < LENGTHS; ++k) {
            int fifo_page_faults[FRAMES] = {};
            int lru_page_faults[FRAMES] = {};

            for (int _ = 0; _ < NUM_TESTS; ++_) {
                int *page_string = generate_page_string(lengths[k], ranges[j]);

                for (int i = 0; i < FRAMES; ++i) {
                    int page_faults = simulate_fifo(frames[i], page_string, lengths[k]);
                    fifo_page_faults[i] += page_faults;

                    page_faults = simulate_lru(frames[i], page_string, lengths[k]);
                    lru_page_faults[i] += page_faults;
                }

                free(page_string);
            }


            for (int i = 0; i < FRAMES; ++i) {
                double avg_fifo_page_faults = (double) fifo_page_faults[i] / NUM_TESTS;
                double avg_lru_page_faults = (double) lru_page_faults[i] / NUM_TESTS;

                fprintf(
                    f,
                    "%d, %d, %d, %lf, %lf\n",
                    frames[i], ranges[j], lengths[k],
                    avg_fifo_page_faults, avg_lru_page_faults
                );
            }
        }

    }
}

void *run_thread(void *x) {
    ThreadParams *params = (ThreadParams *) x;
    FILE *f = params->f;

    int fifo_page_faults[FRAMES] = {};
    int lru_page_faults[FRAMES] = {};

    for (int _ = 0; _ < NUM_TESTS; ++_) {
        int *page_string = generate_page_string(params->length, params->range);

        for (int i = 0; i < FRAMES; ++i) {
            int page_faults = simulate_fifo(params->frames[i], page_string, params->length);
            fifo_page_faults[i] += page_faults;

            page_faults = simulate_lru(params->frames[i], page_string, params->length);
            lru_page_faults[i] += page_faults;
        }

        free(page_string);
    }

    for (int i = 0; i < FRAMES; ++i) {
        double avg_fifo_page_faults = (double)fifo_page_faults[i] / NUM_TESTS;
        double avg_lru_page_faults = (double)lru_page_faults[i] / NUM_TESTS;

        fprintf(
            f,
            "%d, %d, %d, %lf, %lf\n",
            params->frames[i], params->range, params->length,
            avg_fifo_page_faults, avg_lru_page_faults
        );
    }

    return NULL;
}

void run_tests_multi(FILE *f, int *frames, int *ranges, int *lengths) {
    fprintf(f, "Frames, Page Range, Page String Length, FIFO Page Faults, LRU Page Faults\n");

    pthread_t threads[RANGES * LENGTHS]; // Multithreading for different page ranges and string lengths
    ThreadParams params[RANGES * LENGTHS];

    int thread_idx = 0;

    for (int j = 0; j < RANGES; ++j) {
        for (int k = 0; k < LENGTHS; ++k) {
            params[thread_idx].f = f;
            params[thread_idx].length = lengths[k];
            params[thread_idx].range = ranges[j];
            params[thread_idx].frames = frames;

            pthread_create(&threads[thread_idx], NULL, run_thread, (void *)&params[thread_idx]);
            ++thread_idx;
        }
    }

    for (int i = 0; i < RANGES * LENGTHS; ++i) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    int frames[] = {3, 4, 5, 6};
    int ranges[] = {8, 10, 12};
    int lengths[] = {20, 30, 40, 50};

    FILE *f = fopen("data.csv", "w");
    if (f == NULL) {
        printf("Unable to create/edit csv\n");
        return -1;
    }

    clock_t start = clock();
    run_tests(f, frames, ranges, lengths);
    clock_t end = clock();

    printf("Single Threaded Time taken: %lf ms\n", (double) (end - start) * 1000 / CLOCKS_PER_SEC);

    fclose(f);

    f = fopen("data_multithreaded.csv", "w");
    if (f == NULL) {
        printf("Unable to create/edit csv\n");
        return -1;
    }

    start = clock();
    run_tests_multi(f, frames, ranges, lengths);
    end = clock();

    printf("Multi Threaded Time taken: %lf ms\n", (double) (end - start) * 1000 / CLOCKS_PER_SEC);

    fclose(f);

    return 0;
}
