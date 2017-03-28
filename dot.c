#include <cblas.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>

#define MIC_IN_SEC (1000000)
#define LIMIT 28

float dot(float* a, float* b, uint64_t size) {
    float res = 0;
    for (uint64_t i = 0; i < size; ++i)
        res += a[i] * b[i];
    return res;
}

int main() {

    float* a, *b;
    a = malloc(sizeof(float) * (1<<LIMIT));
    b = malloc(sizeof(float) * (1<<LIMIT));

    for (uint64_t i = 1; i < LIMIT; ++i) {
        uint64_t count = (1 << i);


        float res = 0;
        for (uint64_t j = 1; j < count;++j) {
            a[j] = 1.0 / j;
            b[j] = 1.0 / j;
            res += (1.0 / j) * (1.0 / j);
        }
   
        struct timeval tv, tv_end;
        gettimeofday(&tv, NULL);
        double r = cblas_sdot(count, a, 1, b, 1);
        gettimeofday(&tv_end, NULL);

        uint64_t mics_blas = (tv_end.tv_sec * MIC_IN_SEC + tv_end.tv_usec) - (tv.tv_sec * MIC_IN_SEC + tv.tv_usec);
        
        struct timeval tv2, tv_end2;
        gettimeofday(&tv2, NULL);
        double r2 = dot(a, b, count);
        gettimeofday(&tv_end2, NULL);
        uint64_t mics_hand = (tv_end2.tv_sec * MIC_IN_SEC + tv_end2.tv_usec) - (tv2.tv_sec * MIC_IN_SEC + tv2.tv_usec);

        printf("size: %lu spent %lu\n", count, mics_blas);
        printf("size: %lu spent %lu\n", count, mics_hand);

        printf("%lf %lf %lf\n", res, r, r2);
    }

    free(a);
    free(b);

    return 0;
}
