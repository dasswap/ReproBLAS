#include <idxd.h>

/**
 * @internal
 * @brief Set manually specified indexed complex double precision to manually specified indexed double precision (Y = X)
 *
 * Performs the operation Y = X
 *
 * @param fold the fold of the indexed types
 * @param priX X's primary vector
 * @param incpriX stride within X's primary vector (use every incpriX'th element)
 * @param carX X's carry vector
 * @param inccarX stride within X's carry vector (use every inccarX'th element)
 * @param priY Y's primary vector
 * @param incpriY stride within Y's primary vector (use every incpriY'th element)
 * @param carY Y's carry vector
 * @param inccarY stride within Y's carry vector (use every inccarY'th element)
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
void idxd_zmdmset(const int fold, const double *priX, const int incpriX, const double *carX, const int inccarX, double *priY, const int incpriY, double *carY, const int inccarY){
  idxd_dmdmset(fold, priX, incpriX, carX, inccarX, priY, 2 * incpriY, carY, 2 * inccarY);
  idxd_dmsetzero(fold, priY + 1, 2 * incpriY, carY + 1, 2 * inccarY);
}
