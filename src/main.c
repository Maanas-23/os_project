#include "fifo.h"
#include "lru.h"
#include "utils.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_TESTS 300

void run_tests(FILE *f, int *frames, int *ranges, int *lengths) {
    fprintf(f, "Frames, Page Range, Page String Length, FIFO Page Faults, LRU Page Faults\n");

    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 4; ++k) {
            int fifo_page_faults[4] = {};
            int lru_page_faults[4] = {};

            for (int _ = 0; _ < NUM_TESTS; ++_) {
                int *page_string = generate_page_string(lengths[k], ranges[j]);

                for (int i = 0; i < 4; ++i) {
                    int page_faults;

                    page_faults = simulate_fifo(frames[i], page_string, lengths[k]);
                    fifo_page_faults[i] += page_faults;

                    page_faults = simulate_lru(frames[i], page_string, lengths[k]);
                    lru_page_faults[i] += page_faults;
                }

                free(page_string);
            }


            for (int i = 0; i < 4; ++i) {
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

    printf("Successfully run tests and created data.csv\n");
    printf("Time taken: %lf ms\n", (double) (end - start) * 1000 / CLOCKS_PER_SEC);

    fclose(f);

    return 0;
}
