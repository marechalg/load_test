#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>

static inline void view() {
    FILE file = fopen("outputs/unsaved", "r");

    if (!file) {
        printf("No load test recorded yet\n");
        return;
    }


}

#endif