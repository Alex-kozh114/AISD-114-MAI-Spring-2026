#include <stdio.h>
#include "udt.h"

void quicksort_queue(queue *q) {
    if (queue_size(q) <= 1) {
        return;
    }

    queue less, equal, greater;
    queue_create(&less);
    queue_create(&equal);
    queue_create(&greater);

    data_type pivot_item = queue_pop_front(q);
    key_type pivot = pivot_item.key;
    queue_push_back(&equal, pivot_item);

    while (!queue_is_empty(q)) {
        data_type item = queue_pop_front(q);
        if (item.key < pivot) {
            queue_push_back(&less, item);
        } else if (item.key > pivot) {
            queue_push_back(&greater, item);
        } else {
            queue_push_back(&equal, item);
        }
    }

    quicksort_queue(&less);
    quicksort_queue(&greater);

    queue_concat(q, &less);
    queue_concat(q, &equal);
    queue_concat(q, &greater);
}

int main(void) {
    queue my_queue;
    queue_create(&my_queue);

    data_type items[] = {
        {42, {"MAI"}},
        {17, {"Aero"}},
        {99, {"Jet"}},
        {17, {"Flanker"}},
        {5,  {"Space"}},
        {31, {"Rocket"}}
    };

    for (int i = 0; i < 6; i++) {
        queue_push_back(&my_queue, items[i]);
    }

    printf("Source queue:\n");
    queue_print(&my_queue);

    quicksort_queue(&my_queue);

    printf("\nSorted queue:\n");
    queue_print(&my_queue);

    return 0;
}
