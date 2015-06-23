#include <indexed.h>

/**
 * @internal
 * @brief Add manually specified indexed double precision sacled sums of squares (Y += X)
 *
 * Performs the operation Y += X, where X and Y represent scaled sums of squares.
 *
 * @param fold the fold of the indexed types
 * @param scaleX scale of X (scaleX == dscale(Z) for some @c double Z)
 * @param priX X's primary vector
 * @param incpriX stride within X's primary vector (use every incpriX'th element)
 * @param carX X's carry vector
 * @param inccarX stride within X's carry vector (use every inccarX'th element)
 * @param scaleY scale of Y (scaleY == dscale(Z) for some @c double Z)
 * @param priY Y's primary vector
 * @param incpriY stride within Y's primary vector (use every incpriY'th element)
 * @param carY Y's carry vector
 * @param inccarY stride within Y's carry vector (use every inccarY'th element)
 *
 * @return updated scale of Y
 *
 * @author Peter Ahrens
 * @date   1 Jun 2015
 */
double dmdmaddsq(const int fold, const double scaleX, const double *priX, const int incpriX, const double *carX, const int inccarX, const double scaleY, double* priY, const int incpriY, double* carY, const int inccarY) {
  if (scaleX > scaleY){
    dmdrescale(fold, scaleX, scaleY, priY, incpriY, carY, inccarY);
    dmdmadd(fold, priX, incpriX, carX, inccarX, priY, incpriY, carY, inccarY);
    return scaleX;
  }else if(scaleX == scaleY){
    dmdmadd(fold, priX, incpriX, carX, inccarX, priY, incpriY, carY, inccarY);
    return scaleX;
  }else{
    double_indexed *tmp_X = dialloc(fold);
    dmdmset(fold, priX, incpriX, carX, inccarX, tmp_X, 1, tmp_X + fold, 1);
    dmdrescale(fold, scaleY, scaleX, tmp_X, 1, tmp_X + fold, 1);
    dmdmadd(fold, tmp_X, 1, tmp_X + fold, 1, priY, incpriY, carY, inccarY);
    return scaleY;
  }
}
