
#include <stdio.h>
#include <sys/time.h>

#include <stdlib.h>

#include "lcg.h"


void test_lcg_speed() {
    float x;
    struct timeval stop, start;

    int n = 1000000;

    // testing speed vs stdlib
    gettimeofday(&start, NULL);

    for (int i=0; i<n; ++i) {
        x = (float)rand() / (float)RAND_MAX;
        (void)x;
    }
    gettimeofday(&stop, NULL);
    printf("rand(): %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

    struct lcg* lcg = lcg_new();
    gettimeofday(&start, NULL);

    for (int i=0; i<n; ++i) {
        x = lcg_get_float(lcg);
        (void)x;
    }

    gettimeofday(&stop, NULL);
    printf("lcg->get_float(): %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
}

void test_lcg_values() {
    // TODO
}
void test_pink_values() {
    // TODO
}

int main() {
    test_lcg_speed();
    test_lcg_values();

    test_pink_values();

}
