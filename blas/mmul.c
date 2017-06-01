#include <cblas.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#define MIC_IN_SEC (1000000)
#define LIMIT 2000

void mmul(float* a, float* b, float* c, uint64_t size) {
    for (uint64_t i = 0; i < size; ++i) { // row
        for (uint64_t j = 0; j < size; ++j) { // col
            for (uint64_t k = 0; k < size; ++k) {
               c[i * size + j] += a[i * size + k] * b[k * size + j]; 
            }
        }
    }
}

int main() {

    float* a, *b, *c, *c2;

    a = malloc(sizeof(float) * (LIMIT * LIMIT));
    b = malloc(sizeof(float) * (LIMIT * LIMIT));
    c = malloc(sizeof(float) * (LIMIT * LIMIT));
    c2 = malloc(sizeof(float) * (LIMIT * LIMIT));

    memset(a, 0, sizeof(float) * (LIMIT * LIMIT));
    memset(b, 0, sizeof(float) * (LIMIT * LIMIT));
    memset(c, 0, sizeof(float) * (LIMIT * LIMIT));
    memset(c2, 0, sizeof(float) * (LIMIT * LIMIT));

    for (uint64_t i = 1; i < LIMIT; i+=10) {
        memset(c, 0, sizeof(float) * (LIMIT * LIMIT));
        memset(c2, 0, sizeof(float) * (LIMIT * LIMIT));

        uint64_t count = i;

        a[0] = b[0] = 1;

        for (uint64_t j = 1; j < count; ++j) {
            a[j] = 1.0 / j;
            b[j] = 1.0 / j;
        }

        struct timeval tv, tv_end;
        gettimeofday(&tv, NULL);
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, count,
                count, count, 1, a, count, b, count, 0, c, count);
        gettimeofday(&tv_end, NULL);

        uint64_t mics_blas = (tv_end.tv_sec * MIC_IN_SEC + tv_end.tv_usec) -
                             (tv.tv_sec * MIC_IN_SEC + tv.tv_usec);
        
        struct timeval tv2, tv_end2;
        gettimeofday(&tv2, NULL);
        mmul(a, b, c2, count);
        gettimeofday(&tv_end2, NULL);
        uint64_t mics_hand = (tv_end2.tv_sec * MIC_IN_SEC + tv_end2.tv_usec) -
                             (tv2.tv_sec * MIC_IN_SEC + tv2.tv_usec);

        printf("BLAS\tsize: %lu\tspent %lu\n", count, mics_blas);
        printf("MANUAL\tsize: %lu\tspent %lu\n", count, mics_hand);

        printf("%lf %lf\n", c[0], c2[0]);
    }

    free(a);
    free(b);
    free(c);

    return 0;
}
