#include <indexed.h>

/**
 * @brief  Add complex single precision to suitably indexed manually specified indexed complex single precision (Y += X)
 *
 * Performs the operation Y += X on an indexed type Y where the index of Y is larger than the index of X
 *
 * @note This routine was provided as a means of allowing the you to optimize your code. After you have called #cicupdate() on Y with the maximum absolute value of all future elements you wish to deposit in Y, you can call #cicdeposit() to deposit a maximum of #SIENDURANCE elements into Y before renormalizing Y with #cirenorm(). After any number of successive calls of #cicdeposit() on Y, you must renormalize Y with #cirenorm() before using any other function on Y.
 *
 * @param fold the fold of the indexed types
 * @param X scalar X
 * @param priY Y's primary vector
 * @param incpriY stride within Y's primary vector (use every incpriY'th element)
 *
 * @author Hong Diep Nguyen
 * @author Peter Ahrens
 * @date   10 Jun 2015
 */
void cicdeposit(const int fold, const void *X, float_complex_indexed *Y){
  cmcdeposit(fold, X, Y, 1);
}
