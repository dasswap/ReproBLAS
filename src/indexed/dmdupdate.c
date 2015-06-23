#include <math.h>

#include <indexed.h>

#include "../common/common.h"

/**
 * @internal
 * @brief Update manually specified indexed double precision with double precision (X -> Y)
 *
 * This method updates Y to an index suitable for adding numbers with absolute value less than X
 *
 * @param fold the fold of the indexed types
 * @param X scalar X
 * @param priY Y's primary vector
 * @param incpriY stride within Y's primary vector (use every incpriY'th element)
 * @param carY Y's carry vector
 * @param inccarY stride within Y's carry vector (use every inccarY'th element)
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   5 May 2015
 */
void dmdupdate(const int fold, const double X, double* priY, const int incpriY, double* carY, const int inccarY) {
  int i;
  int j;
  int X_index;
  int shift;
  const double *bins;

  if (ISNANINF(priY[0])){
    return;
  }

  X_index = dindex(X);
  if(priY[0] == 0.0){
    bins = dmbins(X_index);
    for(i = 0; i < fold; i++){
      priY[i * incpriY] = bins[i];
      carY[i * inccarY] = 0.0;
    }
  }else{
    shift = dmindex(priY) - X_index;
    if(shift > 0){
      for(i = fold - 1; i >= shift; i--){
        priY[i * incpriY] = priY[(i - shift) * incpriY];
        carY[i * inccarY] = carY[(i - shift) * inccarY];
      }
      bins = dmbins(X_index);
      for(j = 0; j < i + 1; j++){
        priY[j * incpriY] = bins[j];
        carY[j * inccarY] = 0.0;
      }
    }
  }
}
