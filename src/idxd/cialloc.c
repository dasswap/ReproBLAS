#include <idxd.h>

/**
 * @brief indexed complex single precision allocation
 *
 * @param fold the fold of the indexed type
 * @return a freshly allocated indexed type. (free with @c free())
 *
 * @author Peter Ahrens
 * @date   27 Apr 2015
 */
float_complex_indexed *idxd_cialloc(const int fold){
  return (float_complex_indexed*)malloc(idxd_cisize(fold));
}
