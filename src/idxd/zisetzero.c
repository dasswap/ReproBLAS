#include <string.h>

#include <idxd.h>

/**
 * @brief Set indexed double precision to 0 (X = 0)
 *
 * Performs the operation X = 0
 *
 * @param fold the fold of the indexed types
 * @param X indexed scalar X
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
void idxd_zisetzero(const int fold, double_complex_indexed *X){
  memset(X, 0, idxd_zisize(fold));
}
