#include <stdlib.h>

#include <indexedBLAS.h>
#include <indexed.h>


void didgemv(const int fold, const char Order,
             const char TransA, const int M, const int N,
             const double alpha, const double *A, const int lda,
             const double *X, const int incX,
             double_indexed *Y, const int incY){
  dmdgemv(fold, Order,
          TransA, M, N,
          alpha, A, lda,
          X, incX,
          Y, 1, 2 * incY,
          Y + fold, 1, 2 * incY);
}
