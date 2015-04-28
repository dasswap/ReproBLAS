/*
 *  Created   13/10/25   H.D. Nguyen
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "reproBLAS.h"
#include "indexedBLAS.h"

float scnrm2I(
	int N,
	float complex* x, int incx,
	I_float* sum
) {
	I_float_Complex tmp;
	return scnrm2I1(N, x, incx, DEFAULT_FOLD, sum->m, sum->c, (float complex*)&tmp);
}

float rscnrm2(int N, float complex* v, int inc) {
	I_float sum;
	I_float_Complex tmp;
	float sqrt_sum;
	float scale = 0.0;
	
	sisetzero(DEFAULT_FOLD, &sum);
	scale = scnrm2I1(N, v, inc, DEFAULT_FOLD, sum.m, sum.c, (float complex*)&tmp);

	if (isnan(scale))
		return scale;

	if (isinf(sum.m[0]))
		return sum.m[0];

	sqrt_sum = ssiconv(&sum, DEFAULT_FOLD);
//	printf("scale: %g sum: %g\n", scale, sqrt_sum);
	sqrt_sum = sqrt(sqrt_sum);
	return scale * sqrt_sum;
}
