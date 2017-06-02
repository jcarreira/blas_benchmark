#include <armadillo>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>

#define MIC_IN_SEC (1000000)
#define LIMIT 28

double dot(double* a, double* b, uint64_t size) {
    double res = 0;
    for (uint64_t i = 0; i < size; ++i)
        res += a[i] * b[i];
    return res;
}

int main() {

    double* a, *b;
    a = new double[1 << LIMIT];
    b = new double[1 << LIMIT];

    for (uint64_t i = 1; i < LIMIT; ++i) {
        uint64_t count = (1 << i);

        // generate code
        double res = 0;
        for (uint64_t j = 1; j < count;++j) {
            a[j] = 1.0 / j;
            b[j] = 1.0 / j;
            res += (1.0 / j) * (1.0 / j);
        }
   
        /**
          * Benchmark armadillo code
          */   
        struct timeval tv, tv_end;
        gettimeofday(&tv, NULL);

        arma::vec v1(a, count, false, true);
        arma::vec v2(b, count, false, true);

        double r = arma::dot(v1, v2);
        gettimeofday(&tv_end, NULL);

        uint64_t us_eigen = (tv_end.tv_sec * MIC_IN_SEC + tv_end.tv_usec) -
                             (tv.tv_sec * MIC_IN_SEC + tv.tv_usec);
     
        /**
          * Benchmark manual code
          */   
        struct timeval tv2, tv_end2;
        gettimeofday(&tv2, NULL);
        double r2 = dot(a, b, count);
        gettimeofday(&tv_end2, NULL);
        uint64_t us_hand = (tv_end2.tv_sec * MIC_IN_SEC + tv_end2.tv_usec) -
                             (tv2.tv_sec * MIC_IN_SEC + tv2.tv_usec);

        printf("ARMA\tsize: %lu\tspent %lu\n", count, us_eigen);
        printf("MANUAL\tsize: %lu\tspent %lu\n", count, us_hand);

        printf("Correct resultl: %lf\tEigen result: %lf\tManual result: %lf\n", res, r, r2);
    }

    delete[] a;
    delete[] b;

    return 0;
}
