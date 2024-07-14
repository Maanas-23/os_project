#include "fifo.h"

#include <stdbool.h>
#include <stdio.h>

bool find_fifo(int *queue, int frames, int page_number) {
    for (int i = 0; i < frames; ++i) {
        if (queue[i] == page_number) {
            return true;
        }
    }
    return false;
}

int simulate_fifo(int frames, int *pages, int n) {
    int page_faults = 0;

    int queue[frames];
    int rear = 0;

    for (int i = 0; i < frames; ++i) {
        queue[i] = -1;
    }

    for (int i = 0; i < n; ++i) {
        if (!find_fifo(queue, frames, pages[i])) {
            ++page_faults;
            queue[rear] = pages[i];
            rear = (rear + 1) % frames;
        }
    }

    return page_faults;
}
