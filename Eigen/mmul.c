#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <Eigen/Dense>
#include <iostream>

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

void test() {
    float a[] = {1,2,1,2};
    float b[] = {1,2,1,2};
    Eigen::Map<Eigen::MatrixXf> mfa(a, 2, 2);
    Eigen::Map<Eigen::MatrixXf> mfb(b, 2, 2);
    std::cout << mfa * mfb << std::endl;
}

int main() {
    //test();
    //return 0;

    float* a, *b, *c, *c2;

    a = new float[LIMIT * LIMIT];
    b = new float[LIMIT * LIMIT];
    c = new float[LIMIT * LIMIT];
    c2 = new float[LIMIT * LIMIT];

    memset(a, 0, sizeof(float) * (LIMIT * LIMIT));
    memset(b, 0, sizeof(float) * (LIMIT * LIMIT));
    memset(c, 0, sizeof(float) * (LIMIT * LIMIT));
    memset(c2, 0, sizeof(float) * (LIMIT * LIMIT));

    // size is the length of a side of the matrix
    for (uint64_t size = 1; size < LIMIT; size += 10) {
        memset(c, 0, sizeof(float) * (LIMIT * LIMIT));
        memset(c2, 0, sizeof(float) * (LIMIT * LIMIT));

        a[0] = b[0] = 1;

        for (uint64_t j = 1; j < size * size; ++j) {
            a[j] = 1.0 / j;
            b[j] = 1.0 / j;
        }

        struct timeval tv, tv_end;
        gettimeofday(&tv, NULL);

        Eigen::Map<Eigen::MatrixXf> mfa(a, size, size);
        Eigen::Map<Eigen::MatrixXf> mfb(b, size, size);
        auto res = mfa * mfb;
        res.eval();

        gettimeofday(&tv_end, NULL);

        uint64_t us_eigen = (tv_end.tv_sec * MIC_IN_SEC + tv_end.tv_usec) -
                             (tv.tv_sec * MIC_IN_SEC + tv.tv_usec);
        
        struct timeval tv2, tv_end2;
        gettimeofday(&tv2, NULL);
        mmul(a, b, c2, size);
        gettimeofday(&tv_end2, NULL);
        uint64_t us_hand = (tv_end2.tv_sec * MIC_IN_SEC + tv_end2.tv_usec) -
                             (tv2.tv_sec * MIC_IN_SEC + tv2.tv_usec);

        printf("BLAS\tsize: %lu\tspent %lu\n", size, us_eigen);
        printf("MANUAL\tsize: %lu\tspent %lu\n", size, us_hand);

        printf("%lf %lf\n", c[0], c2[0]);
    }

    delete[] a;
    delete[] b;
    delete[] c;

    return 0;
}
