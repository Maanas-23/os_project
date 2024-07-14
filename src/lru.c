#include "lru.h"

typedef struct {
    int page_number;
    int time;
} frame;

typedef enum {false, true} bool;

bool find_lru(frame *arr, int frames, int page_number, int time) {
    for (int i = 0; i < frames; ++i) {
        if (arr[i].page_number == page_number) {
            arr[i].time = time;
            return true;
        }
    }
    return false;
}

int simulate_lru(int frames, int *pages, int n) {
    int page_faults = 0;

    frame arr[frames];
    for (int i = 0; i < frames; ++i) {
        arr[i].page_number = -1;
        arr[i].time = -1;
    }

    for (int i = 0; i < n; ++i) {
        if (!find_lru(arr, frames, pages[i], i)) {
            ++page_faults;

            int lru = 0;
            for (int i = 1; i < frames; ++i) {
                if (arr[i].time < arr[lru].time) {
                    lru = i;
                }
            }

            arr[lru].page_number = pages[i];
            arr[lru].time = i;
        }
    }

    return page_faults;
}

