/*
 *  Created   13/10/25   H.D. Nguyen
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "reproBLAS.h"
#include "indexedBLAS.h"

I_double dasumI(int N, double* v, int inc) {
	I_double sum;
	cisetzero(DEFAULT_FOLD, &sum);
	dasumI1(N, v, inc, DEFAULT_FOLD, sum.m, sum.c);
	return sum;
}

double rdasum(int N, double* v, int inc) {
	I_double sum;
	cisetzero(DEFAULT_FOLD, &sum);
	dasumI1(N, v, inc, DEFAULT_FOLD, sum.m, sum.c);
	return ddiconv(&sum, DEFAULT_FOLD);
}

