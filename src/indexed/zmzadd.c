#include <indexed.h>

/**
 * @internal
 * @brief  Add complex double precision to manually specified indexed complex double precision (Y += X)
 *
 * Performs the operation Y += X on an indexed type Y
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
 * @date   27 Apr 2015
 */
void zmzadd(const int fold, const void *X, double *priY, const int incpriY, double *carY, const int inccarY){
  zmzupdate(fold, X, priY, incpriY, carY, inccarY);
  zmzdeposit(fold, X, priY, incpriY);
  zmrenorm(fold, priY, incpriY, carY, inccarY);
}
