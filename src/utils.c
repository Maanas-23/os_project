#include "utils.h"

#include <stdlib.h>
#include <time.h>

int *generate_page_string(int length, int page_range) {
    int *page_string = malloc(length * sizeof(int));

    static int cnt = 0;
    srand(time(NULL) + cnt++);

    for (int i = 0; i < length; ++i) {
        page_string[i] = rand() % (page_range + 1);
    }

    return page_string;
}
