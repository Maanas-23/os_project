#include "optimal.h"

#include <stdbool.h>
#include <limits.h>

typedef struct {
    int page_number;
    int next;
} pair;

bool find_optimal(pair *arr, int frames, int page_number, int next) {
    for (int i = 0; i < frames; ++i) {
        if (arr[i].page_number == page_number) {
            arr[i].next = next;
            return true;
        }
    }
    return false;
}

int simulate_optimal(int frames, int *pages, int n, int page_range) {
    int page_faults = 0;

    pair arr[frames];
    for (int i = 0; i < frames; ++i) {
        arr[i].page_number = -1;
        arr[i].next = n;
    }

    int next[n], last_vis[page_range+1];
    for (int i = 0; i <= page_range; ++i) {
        last_vis[i] = n+1;
    }

    for (int i = n-1; i >= 0; --i) {
        next[i] = last_vis[pages[i]];
        last_vis[pages[i]] = i;
    }

    for (int i = 0; i < n; ++i) {
        if (!find_optimal(arr, frames, pages[i], next[i])) {
            ++page_faults;

            int farthest = 0;
            for (int j = 1; j < frames; ++j) {
                if (arr[j].next > arr[farthest].next) {
                    farthest = j;
                }
            }

            arr[farthest].page_number = pages[i];
            arr[farthest].next = next[i];
        }
    }

    return page_faults;
}
