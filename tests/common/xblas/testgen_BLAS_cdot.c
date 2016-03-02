#include <stdlib.h>
#include <math.h>
#include "testgen_aux.h"


void BLAS_cdot_x(enum blas_conj_type conj, int n, const void *alpha,
		 const void *x, int incx, const void *beta,
		 const void *y, int incy, void *r, enum blas_prec_type prec)

/*
 * Purpose
 * =======
 * 
 * This routine computes the inner product:
 * 
 *     r <- beta * r + alpha * SUM_{i=0, n-1} x[i] * y[i].
 * 
 * Arguments
 * =========
 *  
 * conj   (input) enum blas_conj_type
 *        When x and y are complex vectors, specifies whether vector
 *        components x[i] are used unconjugated or conjugated. 
 * 
 * n      (input) int
 *        The length of vectors x and y.
 * 
 * alpha  (input) const void*
 * 
 * x      (input) const void*
 *        Array of length n.
 * 
 * incx   (input) int
 *        The stride used to access components x[i].
 *
 * beta   (input) const void*
 *
 * y      (input) const void*
 *        Array of length n.
 *      
 * incy   (input) int
 *        The stride used to access components y[i].
 *
 * r      (input/output) void*
 * 
 * prec   (input) enum blas_prec_type
 *        Specifies the internal precision to be used.
 *        = blas_prec_single: single precision.
 *        = blas_prec_double: double precision.
 *        = blas_prec_extra : anything at least 1.5 times as accurate
 *                            than double, and wider than 80-bits.
 *                            We use double-double in our implementation.
 *
 */
{
  static const char routine_name[] = "BLAS_cdot_x";

  switch (prec) {
  case blas_prec_single:{

      int i, ix = 0, iy = 0;
      float *r_i = (float *) r;
      const float *x_i = (float *) x;
      const float *y_i = (float *) y;
      float *alpha_i = (float *) alpha;
      float *beta_i = (float *) beta;
      float x_ii[2];
      float y_ii[2];
      float r_v[2];
      float prod[2];
      float sum[2];
      float tmp1[2];
      float tmp2[2];


      /* Test the input parameters. */
      if (n < 0)
	BLAS_error(routine_name, -2, n, NULL);
      else if (incx == 0)
	BLAS_error(routine_name, -5, incx, NULL);
      else if (incy == 0)
	BLAS_error(routine_name, -8, incy, NULL);

      /* Immediate return. */
      if (((beta_i[0] == 1.0 && beta_i[1] == 0.0))
	  && (n == 0 || (alpha_i[0] == 0.0 && alpha_i[1] == 0.0)))
	return;



      r_v[0] = r_i[0];
      r_v[1] = r_i[0 + 1];
      sum[0] = sum[1] = 0.0;
      incx *= 2;
      incy *= 2;
      if (incx < 0)
	ix = (-n + 1) * incx;
      if (incy < 0)
	iy = (-n + 1) * incy;

      if (conj == blas_conj) {
	for (i = 0; i < n; ++i) {
	  x_ii[0] = x_i[ix];
	  x_ii[1] = x_i[ix + 1];
	  y_ii[0] = y_i[iy];
	  y_ii[1] = y_i[iy + 1];
	  x_ii[1] = -x_ii[1];
	  {
	    prod[0] = x_ii[0] * y_ii[0] - x_ii[1] * y_ii[1];
	    prod[1] = x_ii[0] * y_ii[1] + x_ii[1] * y_ii[0];
	  }
	  /* prod = x[i]*y[i] */
	  sum[0] = sum[0] + prod[0];
	  sum[1] = sum[1] + prod[1];	/* sum = sum+prod */
	  ix += incx;
	  iy += incy;
	}			/* endfor */
      } else {
	/* do not conjugate */

	for (i = 0; i < n; ++i) {
	  x_ii[0] = x_i[ix];
	  x_ii[1] = x_i[ix + 1];
	  y_ii[0] = y_i[iy];
	  y_ii[1] = y_i[iy + 1];

	  {
	    prod[0] = x_ii[0] * y_ii[0] - x_ii[1] * y_ii[1];
	    prod[1] = x_ii[0] * y_ii[1] + x_ii[1] * y_ii[0];
	  }
	  /* prod = x[i]*y[i] */
	  sum[0] = sum[0] + prod[0];
	  sum[1] = sum[1] + prod[1];	/* sum = sum+prod */
	  ix += incx;
	  iy += incy;
	}			/* endfor */
      }

      {
	tmp1[0] = sum[0] * alpha_i[0] - sum[1] * alpha_i[1];
	tmp1[1] = sum[0] * alpha_i[1] + sum[1] * alpha_i[0];
      }
      /* tmp1 = sum*alpha */
      {
	tmp2[0] = r_v[0] * beta_i[0] - r_v[1] * beta_i[1];
	tmp2[1] = r_v[0] * beta_i[1] + r_v[1] * beta_i[0];
      }
      /* tmp2 = r*beta */
      tmp1[0] = tmp1[0] + tmp2[0];
      tmp1[1] = tmp1[1] + tmp2[1];	/* tmp1 = tmp1+tmp2 */
      ((float *) r)[0] = tmp1[0];
      ((float *) r)[1] = tmp1[1];	/* r = tmp1 */



      break;
    }
  case blas_prec_double:
  case blas_prec_indigenous:
    {
      int i, ix = 0, iy = 0;
      float *r_i = (float *) r;
      const float *x_i = (float *) x;
      const float *y_i = (float *) y;
      float *alpha_i = (float *) alpha;
      float *beta_i = (float *) beta;
      float x_ii[2];
      float y_ii[2];
      float r_v[2];
      double prod[2];
      double sum[2];
      double tmp1[2];
      double tmp2[2];


      /* Test the input parameters. */
      if (n < 0)
	BLAS_error(routine_name, -2, n, NULL);
      else if (incx == 0)
	BLAS_error(routine_name, -5, incx, NULL);
      else if (incy == 0)
	BLAS_error(routine_name, -8, incy, NULL);

      /* Immediate return. */
      if (((beta_i[0] == 1.0 && beta_i[1] == 0.0))
	  && (n == 0 || (alpha_i[0] == 0.0 && alpha_i[1] == 0.0)))
	return;



      r_v[0] = r_i[0];
      r_v[1] = r_i[0 + 1];
      sum[0] = sum[1] = 0.0;
      incx *= 2;
      incy *= 2;
      if (incx < 0)
	ix = (-n + 1) * incx;
      if (incy < 0)
	iy = (-n + 1) * incy;

      if (conj == blas_conj) {
	for (i = 0; i < n; ++i) {
	  x_ii[0] = x_i[ix];
	  x_ii[1] = x_i[ix + 1];
	  y_ii[0] = y_i[iy];
	  y_ii[1] = y_i[iy + 1];
	  x_ii[1] = -x_ii[1];
	  {
	    prod[0] = (double) x_ii[0] * y_ii[0] - (double) x_ii[1] * y_ii[1];
	    prod[1] = (double) x_ii[0] * y_ii[1] + (double) x_ii[1] * y_ii[0];
	  }			/* prod = x[i]*y[i] */
	  sum[0] = sum[0] + prod[0];
	  sum[1] = sum[1] + prod[1];	/* sum = sum+prod */
	  ix += incx;
	  iy += incy;
	}			/* endfor */
      } else {
	/* do not conjugate */

	for (i = 0; i < n; ++i) {
	  x_ii[0] = x_i[ix];
	  x_ii[1] = x_i[ix + 1];
	  y_ii[0] = y_i[iy];
	  y_ii[1] = y_i[iy + 1];

	  {
	    prod[0] = (double) x_ii[0] * y_ii[0] - (double) x_ii[1] * y_ii[1];
	    prod[1] = (double) x_ii[0] * y_ii[1] + (double) x_ii[1] * y_ii[0];
	  }			/* prod = x[i]*y[i] */
	  sum[0] = sum[0] + prod[0];
	  sum[1] = sum[1] + prod[1];	/* sum = sum+prod */
	  ix += incx;
	  iy += incy;
	}			/* endfor */
      }

      {
	tmp1[0] = (double) sum[0] * alpha_i[0] - (double) sum[1] * alpha_i[1];
	tmp1[1] = (double) sum[0] * alpha_i[1] + (double) sum[1] * alpha_i[0];
      }				/* tmp1 = sum*alpha */
      {
	tmp2[0] = (double) r_v[0] * beta_i[0] - (double) r_v[1] * beta_i[1];
	tmp2[1] = (double) r_v[0] * beta_i[1] + (double) r_v[1] * beta_i[0];
      }				/* tmp2 = r*beta */
      tmp1[0] = tmp1[0] + tmp2[0];
      tmp1[1] = tmp1[1] + tmp2[1];	/* tmp1 = tmp1+tmp2 */
      ((float *) r)[0] = tmp1[0];
      ((float *) r)[1] = tmp1[1];	/* r = tmp1 */


    }
    break;
  case blas_prec_extra:
    {
      int i, ix = 0, iy = 0;
      float *r_i = (float *) r;
      const float *x_i = (float *) x;
      const float *y_i = (float *) y;
      float *alpha_i = (float *) alpha;
      float *beta_i = (float *) beta;
      float x_ii[2];
      float y_ii[2];
      float r_v[2];
      double head_prod[2], tail_prod[2];
      double head_sum[2], tail_sum[2];
      double head_tmp1[2], tail_tmp1[2];
      double head_tmp2[2], tail_tmp2[2];
      FPU_FIX_DECL;

      /* Test the input parameters. */
      if (n < 0)
	BLAS_error(routine_name, -2, n, NULL);
      else if (incx == 0)
	BLAS_error(routine_name, -5, incx, NULL);
      else if (incy == 0)
	BLAS_error(routine_name, -8, incy, NULL);

      /* Immediate return. */
      if (((beta_i[0] == 1.0 && beta_i[1] == 0.0))
	  && (n == 0 || (alpha_i[0] == 0.0 && alpha_i[1] == 0.0)))
	return;

      FPU_FIX_START;

      r_v[0] = r_i[0];
      r_v[1] = r_i[0 + 1];
      head_sum[0] = head_sum[1] = tail_sum[0] = tail_sum[1] = 0.0;
      incx *= 2;
      incy *= 2;
      if (incx < 0)
	ix = (-n + 1) * incx;
      if (incy < 0)
	iy = (-n + 1) * incy;

      if (conj == blas_conj) {
	for (i = 0; i < n; ++i) {
	  x_ii[0] = x_i[ix];
	  x_ii[1] = x_i[ix + 1];
	  y_ii[0] = y_i[iy];
	  y_ii[1] = y_i[iy + 1];
	  x_ii[1] = -x_ii[1];
	  {
	    double head_e1, tail_e1;
	    double d1;
	    double d2;
	    /* Real part */
	    d1 = (double) x_ii[0] * y_ii[0];
	    d2 = (double) -x_ii[1] * y_ii[1];
	    {
	      /* Compute double-double = double + double. */
	      double e, t1, t2;

	      /* Knuth trick. */
	      t1 = d1 + d2;
	      e = t1 - d1;
	      t2 = ((d2 - e) + (d1 - (t1 - e)));

	      /* The result is t1 + t2, after normalization. */
	      head_e1 = t1 + t2;
	      tail_e1 = t2 - (head_e1 - t1);
	    }
	    head_prod[0] = head_e1;
	    tail_prod[0] = tail_e1;
	    /* imaginary part */
	    d1 = (double) x_ii[0] * y_ii[1];
	    d2 = (double) x_ii[1] * y_ii[0];
	    {
	      /* Compute double-double = double + double. */
	      double e, t1, t2;

	      /* Knuth trick. */
	      t1 = d1 + d2;
	      e = t1 - d1;
	      t2 = ((d2 - e) + (d1 - (t1 - e)));

	      /* The result is t1 + t2, after normalization. */
	      head_e1 = t1 + t2;
	      tail_e1 = t2 - (head_e1 - t1);
	    }
	    head_prod[1] = head_e1;
	    tail_prod[1] = tail_e1;
	  }			/* prod = x[i]*y[i] */
	  {
	    double head_t, tail_t;
	    double head_a, tail_a;
	    double head_b, tail_b;
	    /* Real part */
	    head_a = head_sum[0];
	    tail_a = tail_sum[0];
	    head_b = head_prod[0];
	    tail_b = tail_prod[0];
	    {
	      /* Compute double-double = double-double + double-double. */
	      double bv;
	      double s1, s2, t1, t2;

	      /* Add two hi words. */
	      s1 = head_a + head_b;
	      bv = s1 - head_a;
	      s2 = ((head_b - bv) + (head_a - (s1 - bv)));

	      /* Add two lo words. */
	      t1 = tail_a + tail_b;
	      bv = t1 - tail_a;
	      t2 = ((tail_b - bv) + (tail_a - (t1 - bv)));

	      s2 += t1;

	      /* Renormalize (s1, s2)  to  (t1, s2) */
	      t1 = s1 + s2;
	      s2 = s2 - (t1 - s1);

	      t2 += s2;

	      /* Renormalize (t1, t2)  */
	      head_t = t1 + t2;
	      tail_t = t2 - (head_t - t1);
	    }
	    head_sum[0] = head_t;
	    tail_sum[0] = tail_t;
	    /* Imaginary part */
	    head_a = head_sum[1];
	    tail_a = tail_sum[1];
	    head_b = head_prod[1];
	    tail_b = tail_prod[1];
	    {
	      /* Compute double-double = double-double + double-double. */
	      double bv;
	      double s1, s2, t1, t2;

	      /* Add two hi words. */
	      s1 = head_a + head_b;
	      bv = s1 - head_a;
	      s2 = ((head_b - bv) + (head_a - (s1 - bv)));

	      /* Add two lo words. */
	      t1 = tail_a + tail_b;
	      bv = t1 - tail_a;
	      t2 = ((tail_b - bv) + (tail_a - (t1 - bv)));

	      s2 += t1;

	      /* Renormalize (s1, s2)  to  (t1, s2) */
	      t1 = s1 + s2;
	      s2 = s2 - (t1 - s1);

	      t2 += s2;

	      /* Renormalize (t1, t2)  */
	      head_t = t1 + t2;
	      tail_t = t2 - (head_t - t1);
	    }
	    head_sum[1] = head_t;
	    tail_sum[1] = tail_t;
	  }			/* sum = sum+prod */
	  ix += incx;
	  iy += incy;
	}			/* endfor */
      } else {
	/* do not conjugate */

	for (i = 0; i < n; ++i) {
	  x_ii[0] = x_i[ix];
	  x_ii[1] = x_i[ix + 1];
	  y_ii[0] = y_i[iy];
	  y_ii[1] = y_i[iy + 1];

	  {
	    double head_e1, tail_e1;
	    double d1;
	    double d2;
	    /* Real part */
	    d1 = (double) x_ii[0] * y_ii[0];
	    d2 = (double) -x_ii[1] * y_ii[1];
	    {
	      /* Compute double-double = double + double. */
	      double e, t1, t2;

	      /* Knuth trick. */
	      t1 = d1 + d2;
	      e = t1 - d1;
	      t2 = ((d2 - e) + (d1 - (t1 - e)));

	      /* The result is t1 + t2, after normalization. */
	      head_e1 = t1 + t2;
	      tail_e1 = t2 - (head_e1 - t1);
	    }
	    head_prod[0] = head_e1;
	    tail_prod[0] = tail_e1;
	    /* imaginary part */
	    d1 = (double) x_ii[0] * y_ii[1];
	    d2 = (double) x_ii[1] * y_ii[0];
	    {
	      /* Compute double-double = double + double. */
	      double e, t1, t2;

	      /* Knuth trick. */
	      t1 = d1 + d2;
	      e = t1 - d1;
	      t2 = ((d2 - e) + (d1 - (t1 - e)));

	      /* The result is t1 + t2, after normalization. */
	      head_e1 = t1 + t2;
	      tail_e1 = t2 - (head_e1 - t1);
	    }
	    head_prod[1] = head_e1;
	    tail_prod[1] = tail_e1;
	  }			/* prod = x[i]*y[i] */
	  {
	    double head_t, tail_t;
	    double head_a, tail_a;
	    double head_b, tail_b;
	    /* Real part */
	    head_a = head_sum[0];
	    tail_a = tail_sum[0];
	    head_b = head_prod[0];
	    tail_b = tail_prod[0];
	    {
	      /* Compute double-double = double-double + double-double. */
	      double bv;
	      double s1, s2, t1, t2;

	      /* Add two hi words. */
	      s1 = head_a + head_b;
	      bv = s1 - head_a;
	      s2 = ((head_b - bv) + (head_a - (s1 - bv)));

	      /* Add two lo words. */
	      t1 = tail_a + tail_b;
	      bv = t1 - tail_a;
	      t2 = ((tail_b - bv) + (tail_a - (t1 - bv)));

	      s2 += t1;

	      /* Renormalize (s1, s2)  to  (t1, s2) */
	      t1 = s1 + s2;
	      s2 = s2 - (t1 - s1);

	      t2 += s2;

	      /* Renormalize (t1, t2)  */
	      head_t = t1 + t2;
	      tail_t = t2 - (head_t - t1);
	    }
	    head_sum[0] = head_t;
	    tail_sum[0] = tail_t;
	    /* Imaginary part */
	    head_a = head_sum[1];
	    tail_a = tail_sum[1];
	    head_b = head_prod[1];
	    tail_b = tail_prod[1];
	    {
	      /* Compute double-double = double-double + double-double. */
	      double bv;
	      double s1, s2, t1, t2;

	      /* Add two hi words. */
	      s1 = head_a + head_b;
	      bv = s1 - head_a;
	      s2 = ((head_b - bv) + (head_a - (s1 - bv)));

	      /* Add two lo words. */
	      t1 = tail_a + tail_b;
	      bv = t1 - tail_a;
	      t2 = ((tail_b - bv) + (tail_a - (t1 - bv)));

	      s2 += t1;

	      /* Renormalize (s1, s2)  to  (t1, s2) */
	      t1 = s1 + s2;
	      s2 = s2 - (t1 - s1);

	      t2 += s2;

	      /* Renormalize (t1, t2)  */
	      head_t = t1 + t2;
	      tail_t = t2 - (head_t - t1);
	    }
	    head_sum[1] = head_t;
	    tail_sum[1] = tail_t;
	  }			/* sum = sum+prod */
	  ix += incx;
	  iy += incy;
	}			/* endfor */
      }

      {
	double cd[2];
	cd[0] = (double) alpha_i[0];
	cd[1] = (double) alpha_i[1];
	{
	  /* Compute complex-extra = complex-extra * complex-double. */
	  double head_a0, tail_a0;
	  double head_a1, tail_a1;
	  double head_t1, tail_t1;
	  double head_t2, tail_t2;
	  head_a0 = head_sum[0];
	  tail_a0 = tail_sum[0];
	  head_a1 = head_sum[1];
	  tail_a1 = tail_sum[1];
	  /* real part */
	  {
	    /* Compute double-double = double-double * double. */
	    double a11, a21, b1, b2, c11, c21, c2, con, t1, t2;

	    con = head_a0 * split;
	    a11 = con - head_a0;
	    a11 = con - a11;
	    a21 = head_a0 - a11;
	    con = cd[0] * split;
	    b1 = con - cd[0];
	    b1 = con - b1;
	    b2 = cd[0] - b1;

	    c11 = head_a0 * cd[0];
	    c21 = (((a11 * b1 - c11) + a11 * b2) + a21 * b1) + a21 * b2;

	    c2 = tail_a0 * cd[0];
	    t1 = c11 + c2;
	    t2 = (c2 - (t1 - c11)) + c21;

	    head_t1 = t1 + t2;
	    tail_t1 = t2 - (head_t1 - t1);
	  }
	  {
	    /* Compute double-double = double-double * double. */
	    double a11, a21, b1, b2, c11, c21, c2, con, t1, t2;

	    con = head_a1 * split;
	    a11 = con - head_a1;
	    a11 = con - a11;
	    a21 = head_a1 - a11;
	    con = cd[1] * split;
	    b1 = con - cd[1];
	    b1 = con - b1;
	    b2 = cd[1] - b1;

	    c11 = head_a1 * cd[1];
	    c21 = (((a11 * b1 - c11) + a11 * b2) + a21 * b1) + a21 * b2;

	    c2 = tail_a1 * cd[1];
	    t1 = c11 + c2;
	    t2 = (c2 - (t1 - c11)) + c21;

	    head_t2 = t1 + t2;
	    tail_t2 = t2 - (head_t2 - t1);
	  }
	  head_t2 = -head_t2;
	  tail_t2 = -tail_t2;
	  {
	    /* Compute double-double = double-double + double-double. */
	    double bv;
	    double s1, s2, t1, t2;

	    /* Add two hi words. */
	    s1 = head_t1 + head_t2;
	    bv = s1 - head_t1;
	    s2 = ((head_t2 - bv) + (head_t1 - (s1 - bv)));

	    /* Add two lo words. */
	    t1 = tail_t1 + tail_t2;
	    bv = t1 - tail_t1;
	    t2 = ((tail_t2 - bv) + (tail_t1 - (t1 - bv)));

	    s2 += t1;

	    /* Renormalize (s1, s2)  to  (t1, s2) */
	    t1 = s1 + s2;
	    s2 = s2 - (t1 - s1);

	    t2 += s2;

	    /* Renormalize (t1, t2)  */
	    head_t1 = t1 + t2;
	    tail_t1 = t2 - (head_t1 - t1);
	  }
	  head_tmp1[0] = head_t1;
	  tail_tmp1[0] = tail_t1;
	  /* imaginary part */
	  {
	    /* Compute double-double = double-double * double. */
	    double a11, a21, b1, b2, c11, c21, c2, con, t1, t2;

	    con = head_a1 * split;
	    a11 = con - head_a1;
	    a11 = con - a11;
	    a21 = head_a1 - a11;
	    con = cd[0] * split;
	    b1 = con - cd[0];
	    b1 = con - b1;
	    b2 = cd[0] - b1;

	    c11 = head_a1 * cd[0];
	    c21 = (((a11 * b1 - c11) + a11 * b2) + a21 * b1) + a21 * b2;

	    c2 = tail_a1 * cd[0];
	    t1 = c11 + c2;
	    t2 = (c2 - (t1 - c11)) + c21;

	    head_t1 = t1 + t2;
	    tail_t1 = t2 - (head_t1 - t1);
	  }
	  {
	    /* Compute double-double = double-double * double. */
	    double a11, a21, b1, b2, c11, c21, c2, con, t1, t2;

	    con = head_a0 * split;
	    a11 = con - head_a0;
	    a11 = con - a11;
	    a21 = head_a0 - a11;
	    con = cd[1] * split;
	    b1 = con - cd[1];
	    b1 = con - b1;
	    b2 = cd[1] - b1;

	    c11 = head_a0 * cd[1];
	    c21 = (((a11 * b1 - c11) + a11 * b2) + a21 * b1) + a21 * b2;

	    c2 = tail_a0 * cd[1];
	    t1 = c11 + c2;
	    t2 = (c2 - (t1 - c11)) + c21;

	    head_t2 = t1 + t2;
	    tail_t2 = t2 - (head_t2 - t1);
	  }
	  {
	    /* Compute double-double = double-double + double-double. */
	    double bv;
	    double s1, s2, t1, t2;

	    /* Add two hi words. */
	    s1 = head_t1 + head_t2;
	    bv = s1 - head_t1;
	    s2 = ((head_t2 - bv) + (head_t1 - (s1 - bv)));

	    /* Add two lo words. */
	    t1 = tail_t1 + tail_t2;
	    bv = t1 - tail_t1;
	    t2 = ((tail_t2 - bv) + (tail_t1 - (t1 - bv)));

	    s2 += t1;

	    /* Renormalize (s1, s2)  to  (t1, s2) */
	    t1 = s1 + s2;
	    s2 = s2 - (t1 - s1);

	    t2 += s2;

	    /* Renormalize (t1, t2)  */
	    head_t1 = t1 + t2;
	    tail_t1 = t2 - (head_t1 - t1);
	  }
	  head_tmp1[1] = head_t1;
	  tail_tmp1[1] = tail_t1;
	}

      }				/* tmp1 = sum*alpha */
      {
	double head_e1, tail_e1;
	double d1;
	double d2;
	/* Real part */
	d1 = (double) r_v[0] * beta_i[0];
	d2 = (double) -r_v[1] * beta_i[1];
	{
	  /* Compute double-double = double + double. */
	  double e, t1, t2;

	  /* Knuth trick. */
	  t1 = d1 + d2;
	  e = t1 - d1;
	  t2 = ((d2 - e) + (d1 - (t1 - e)));

	  /* The result is t1 + t2, after normalization. */
	  head_e1 = t1 + t2;
	  tail_e1 = t2 - (head_e1 - t1);
	}
	head_tmp2[0] = head_e1;
	tail_tmp2[0] = tail_e1;
	/* imaginary part */
	d1 = (double) r_v[0] * beta_i[1];
	d2 = (double) r_v[1] * beta_i[0];
	{
	  /* Compute double-double = double + double. */
	  double e, t1, t2;

	  /* Knuth trick. */
	  t1 = d1 + d2;
	  e = t1 - d1;
	  t2 = ((d2 - e) + (d1 - (t1 - e)));

	  /* The result is t1 + t2, after normalization. */
	  head_e1 = t1 + t2;
	  tail_e1 = t2 - (head_e1 - t1);
	}
	head_tmp2[1] = head_e1;
	tail_tmp2[1] = tail_e1;
      }				/* tmp2 = r*beta */
      {
	double head_t, tail_t;
	double head_a, tail_a;
	double head_b, tail_b;
	/* Real part */
	head_a = head_tmp1[0];
	tail_a = tail_tmp1[0];
	head_b = head_tmp2[0];
	tail_b = tail_tmp2[0];
	{
	  /* Compute double-double = double-double + double-double. */
	  double bv;
	  double s1, s2, t1, t2;

	  /* Add two hi words. */
	  s1 = head_a + head_b;
	  bv = s1 - head_a;
	  s2 = ((head_b - bv) + (head_a - (s1 - bv)));

	  /* Add two lo words. */
	  t1 = tail_a + tail_b;
	  bv = t1 - tail_a;
	  t2 = ((tail_b - bv) + (tail_a - (t1 - bv)));

	  s2 += t1;

	  /* Renormalize (s1, s2)  to  (t1, s2) */
	  t1 = s1 + s2;
	  s2 = s2 - (t1 - s1);

	  t2 += s2;

	  /* Renormalize (t1, t2)  */
	  head_t = t1 + t2;
	  tail_t = t2 - (head_t - t1);
	}
	head_tmp1[0] = head_t;
	tail_tmp1[0] = tail_t;
	/* Imaginary part */
	head_a = head_tmp1[1];
	tail_a = tail_tmp1[1];
	head_b = head_tmp2[1];
	tail_b = tail_tmp2[1];
	{
	  /* Compute double-double = double-double + double-double. */
	  double bv;
	  double s1, s2, t1, t2;

	  /* Add two hi words. */
	  s1 = head_a + head_b;
	  bv = s1 - head_a;
	  s2 = ((head_b - bv) + (head_a - (s1 - bv)));

	  /* Add two lo words. */
	  t1 = tail_a + tail_b;
	  bv = t1 - tail_a;
	  t2 = ((tail_b - bv) + (tail_a - (t1 - bv)));

	  s2 += t1;

	  /* Renormalize (s1, s2)  to  (t1, s2) */
	  t1 = s1 + s2;
	  s2 = s2 - (t1 - s1);

	  t2 += s2;

	  /* Renormalize (t1, t2)  */
	  head_t = t1 + t2;
	  tail_t = t2 - (head_t - t1);
	}
	head_tmp1[1] = head_t;
	tail_tmp1[1] = tail_t;
      }				/* tmp1 = tmp1+tmp2 */
      ((float *) r)[0] = head_tmp1[0];
      ((float *) r)[1] = head_tmp1[1];	/* r = tmp1 */

      FPU_FIX_STOP;
    }
    break;
  }
}

/* Complex-Complex Multiplication */
void c_mul(float a[], float b[], float c[])
{
  float cr, ci;
  cr = a[0] * b[0] - a[1] * b[1];
  ci = a[1] * b[0] + a[0] * b[1];
  c[0] = cr;
  c[1] = ci;
}

/* Complex Division c = a/b */
void c_div(float a[], float b[], float c[])
{
  float ratio, den;
  float abr, abi, cr, ci;

  if ((abr = b[0]) < 0.)
    abr = -abr;
  if ((abi = b[1]) < 0.)
    abi = -abi;
  if (abr <= abi) {
    if (abi == 0) {
      BLAS_error("c_div: division by zero", 0, 0, NULL);
    }
    ratio = b[0] / b[1];
    den = b[1] * (1 + ratio * ratio);
    cr = (a[0] * ratio + a[1]) / den;
    ci = (a[1] * ratio - a[0]) / den;
  } else {
    ratio = b[1] / b[0];
    den = b[0] * (1 + ratio * ratio);
    cr = (a[0] + a[1] * ratio) / den;
    ci = (a[1] - a[0] * ratio) / den;
  }
  c[0] = cr;
  c[1] = ci;
}

static double ulp(float a)
/*
 * Purpose 
 * =======
 * 
 * Compute the unit last place of a double precision number.
 */
{
  double f;
  int e;
  f = frexp(a, &e);
  return power(2, e - BITS_S);
}


static float rand_half_1(int l_bits, int *seed)
/*
 * Purpose
 * =======
 * 
 * Generate random number in the interval [0.5, 1). 
 * l_bits specifies that only the leading l_bits are nonzero.
 * 
 */
{
  float a = xrand(seed);        /* [0,1] */
  a /= 2.;
  a += 0.5;
  if (l_bits < BITS_S) {
    float s = power(2, l_bits);
    float t = a / s;            /* shift right l_bits */
    t = (t + a) - a;            /* cancel trailing bits */
    a = t * s;                  /* shift back */
  }
  return a;
}


static void r_truth(enum blas_conj_type conj, int n, void *alpha, const void *x, int incx, void *beta, const void *y, int incy, void *r,        /* input */
                    double *r_true_l, double *r_true_t)
{
  int i, ix = 0, iy = 0;
  float *r_i = (float *) r;
  const float *x_i = (float *) x;
  const float *y_i = (float *) y;
  float *alpha_i = (float *) alpha;
  float *beta_i = (float *) beta;
  float x_ii[2];
  float y_ii[2];
  float r_v[2];
  double prod_l[2], prod_t[2];
  double sum_l[2], sum_t[2];
  double tmp1_l[2], tmp1_t[2];
  double tmp2_l[2], tmp2_t[2];

  /* Immediate return */
  if (n < 0) {
    r_true_l[0] = r_true_l[1] = r_true_t[0] = r_true_t[1] = 0.0;
    return;
  }

  r_v[0] = r_i[0];
  r_v[1] = r_i[0 + 1];
  sum_l[0] = sum_l[1] = sum_t[0] = sum_t[1] = 0.0;      /* sum = 0 */

  for (i = 0; i < n; ++i) {
    x_ii[0] = x_i[ix];
    x_ii[1] = x_i[ix + 1];
    if (conj == blas_conj)
      x_ii[1] = -x_ii[1];
    y_ii[0] = y_i[iy];
    y_ii[1] = y_i[iy + 1];
    {
      double e1_l, e1_t;
      double d1;
      double d2;
      /* Real part */
      d1 = (double) x_ii[0] * y_ii[0];
      d2 = (double) -x_ii[1] * y_ii[1];
      {
        /* Compute double-double = double + double. */
        double e, t1, t2;

        /* Knuth trick. */
        t1 = d1 + d2;
        e = t1 - d1;
        t2 = ((d2 - e) + (d1 - (t1 - e)));

        /* The result is t1 + t2, after normalization. */
        e1_l = t1 + t2;
        e1_t = t2 - (e1_l - t1);
      }
      prod_l[0] = e1_l;
      prod_t[0] = e1_t;
      /* imaginary part */
      d1 = (double) x_ii[0] * y_ii[1];
      d2 = (double) x_ii[1] * y_ii[0];
      {
        /* Compute double-double = double + double. */
        double e, t1, t2;

        /* Knuth trick. */
        t1 = d1 + d2;
        e = t1 - d1;
        t2 = ((d2 - e) + (d1 - (t1 - e)));

        /* The result is t1 + t2, after normalization. */
        e1_l = t1 + t2;
        e1_t = t2 - (e1_l - t1);
      }
      prod_l[1] = e1_l;
      prod_t[1] = e1_t;
    }                           /* prod = x[i]*y[i] */
    {
      double t_l, t_t;
      double a_l, a_t;
      double b_l, b_t;
      /* Real part */
      a_l = sum_l[0];
      a_t = sum_t[0];
      b_l = prod_l[0];
      b_t = prod_t[0];
      {
        /*
         * Compute double-double = double-double + double-double.
         */
        double e, t1, t2;

        /* Knuth trick. */
        t1 = a_l + b_l;
        e = t1 - a_l;
        t2 = ((b_l - e) + (a_l - (t1 - e))) + a_t + b_t;

        /* The result is t1 + t2, after normalization. */
        t_l = t1 + t2;
        t_t = t2 - (t_l - t1);
      }
      sum_l[0] = t_l;
      sum_t[0] = t_t;
      /* Imaginary part */
      a_l = sum_l[1];
      a_t = sum_t[1];
      b_l = prod_l[1];
      b_t = prod_t[1];
      {
        /*
         * Compute double-double = double-double + double-double.
         */
        double e, t1, t2;

        /* Knuth trick. */
        t1 = a_l + b_l;
        e = t1 - a_l;
        t2 = ((b_l - e) + (a_l - (t1 - e))) + a_t + b_t;

        /* The result is t1 + t2, after normalization. */
        t_l = t1 + t2;
        t_t = t2 - (t_l - t1);
      }
      sum_l[1] = t_l;
      sum_t[1] = t_t;
    }                           /* sum = sum+prod */
    ix += 2;
    iy += 2;
  }                             /* endfor */
  {
    double cd[2];
    cd[0] = (double) alpha_i[0];
    cd[1] = (double) alpha_i[1];
    {
      /*
       * Compute complex-extra = complex-extra * complex-double.
       */
      double a0_l, a0_t;
      double a1_l, a1_t;
      double t1_l, t1_t;
      double t2_l, t2_t;
      a0_l = sum_l[0];
      a0_t = sum_t[0];
      a1_l = sum_l[1];
      a1_t = sum_t[1];
      /* real part */
      {
        /* Compute double-double = double-double * double. */
        double a11, a21, b1, b2, c11, c21, c2, con, e, t1, t2;

        con = a0_l * split;
        a11 = con - a0_l;
        a11 = con - a11;
        a21 = a0_l - a11;
        con = cd[0] * split;
        b1 = con - cd[0];
        b1 = con - b1;
        b2 = cd[0] - b1;

        c11 = a0_l * cd[0];
        c21 = (((a11 * b1 - c11) + a11 * b2) + a21 * b1) + a21 * b2;

        c2 = a0_t * cd[0];
        t1 = c11 + c2;
        e = t1 - c11;
        t2 = ((c2 - e) + (c11 - (t1 - e))) + c21;

        t1_l = t1 + t2;
        t1_t = t2 - (t1_l - t1);
      }
      {
        /* Compute double-double = double-double * double. */
        double a11, a21, b1, b2, c11, c21, c2, con, e, t1, t2;

        con = a1_l * split;
        a11 = con - a1_l;
        a11 = con - a11;
        a21 = a1_l - a11;
        con = cd[1] * split;
        b1 = con - cd[1];
        b1 = con - b1;
        b2 = cd[1] - b1;

        c11 = a1_l * cd[1];
        c21 = (((a11 * b1 - c11) + a11 * b2) + a21 * b1) + a21 * b2;

        c2 = a1_t * cd[1];
        t1 = c11 + c2;
        e = t1 - c11;
        t2 = ((c2 - e) + (c11 - (t1 - e))) + c21;

        t2_l = t1 + t2;
        t2_t = t2 - (t2_l - t1);
      }
      t2_l = -t2_l;
      t2_t = -t2_t;
      {
        /*
         * Compute double-double = double-double + double-double.
         */
        double e, t1, t2;

        /* Knuth trick. */
        t1 = t1_l + t2_l;
        e = t1 - t1_l;
        t2 = ((t2_l - e) + (t1_l - (t1 - e))) + t1_t + t2_t;

        /* The result is t1 + t2, after normalization. */
        t1_l = t1 + t2;
        t1_t = t2 - (t1_l - t1);
      }
      tmp1_l[0] = t1_l;
      tmp1_t[0] = t1_t;
      /* imaginary part */
      {
        /* Compute double-double = double-double * double. */
        double a11, a21, b1, b2, c11, c21, c2, con, e, t1, t2;

        con = a1_l * split;
        a11 = con - a1_l;
        a11 = con - a11;
        a21 = a1_l - a11;
        con = cd[0] * split;
        b1 = con - cd[0];
        b1 = con - b1;
        b2 = cd[0] - b1;

        c11 = a1_l * cd[0];
        c21 = (((a11 * b1 - c11) + a11 * b2) + a21 * b1) + a21 * b2;

        c2 = a1_t * cd[0];
        t1 = c11 + c2;
        e = t1 - c11;
        t2 = ((c2 - e) + (c11 - (t1 - e))) + c21;

        t1_l = t1 + t2;
        t1_t = t2 - (t1_l - t1);
      }
      {
        /* Compute double-double = double-double * double. */
        double a11, a21, b1, b2, c11, c21, c2, con, e, t1, t2;

        con = a0_l * split;
        a11 = con - a0_l;
        a11 = con - a11;
        a21 = a0_l - a11;
        con = cd[1] * split;
        b1 = con - cd[1];
        b1 = con - b1;
        b2 = cd[1] - b1;

        c11 = a0_l * cd[1];
        c21 = (((a11 * b1 - c11) + a11 * b2) + a21 * b1) + a21 * b2;

        c2 = a0_t * cd[1];
        t1 = c11 + c2;
        e = t1 - c11;
        t2 = ((c2 - e) + (c11 - (t1 - e))) + c21;

        t2_l = t1 + t2;
        t2_t = t2 - (t2_l - t1);
      }
      {
        /*
         * Compute double-double = double-double +
         * double-double.
         */
        double e, t1, t2;

        /* Knuth trick. */
        t1 = t1_l + t2_l;
        e = t1 - t1_l;
        t2 = ((t2_l - e) + (t1_l - (t1 - e))) + t1_t + t2_t;

        /* The result is t1 + t2, after normalization. */
        t1_l = t1 + t2;
        t1_t = t2 - (t1_l - t1);
      }
      tmp1_l[1] = t1_l;
      tmp1_t[1] = t1_t;
    }

  }                             /* tmp1 = sum*alpha */
  {
    double e1_l, e1_t;
    double d1;
    double d2;
    /* Real part */
    d1 = (double) r_v[0] * beta_i[0];
    d2 = (double) -r_v[1] * beta_i[1];
    {
      /* Compute double-double = double + double. */
      double e, t1, t2;

      /* Knuth trick. */
      t1 = d1 + d2;
      e = t1 - d1;
      t2 = ((d2 - e) + (d1 - (t1 - e)));

      /* The result is t1 + t2, after normalization. */
      e1_l = t1 + t2;
      e1_t = t2 - (e1_l - t1);
    }
    tmp2_l[0] = e1_l;
    tmp2_t[0] = e1_t;
    /* imaginary part */
    d1 = (double) r_v[0] * beta_i[1];
    d2 = (double) r_v[1] * beta_i[0];
    {
      /* Compute double-double = double + double. */
      double e, t1, t2;

      /* Knuth trick. */
      t1 = d1 + d2;
      e = t1 - d1;
      t2 = ((d2 - e) + (d1 - (t1 - e)));

      /* The result is t1 + t2, after normalization. */
      e1_l = t1 + t2;
      e1_t = t2 - (e1_l - t1);
    }
    tmp2_l[1] = e1_l;
    tmp2_t[1] = e1_t;
  }                             /* tmp2 = r*beta */
  {
    double t_l, t_t;
    double a_l, a_t;
    double b_l, b_t;
    /* Real part */
    a_l = tmp1_l[0];
    a_t = tmp1_t[0];
    b_l = tmp2_l[0];
    b_t = tmp2_t[0];
    {
      /* Compute double-double = double-double + double-double. */
      double e, t1, t2;

      /* Knuth trick. */
      t1 = a_l + b_l;
      e = t1 - a_l;
      t2 = ((b_l - e) + (a_l - (t1 - e))) + a_t + b_t;

      /* The result is t1 + t2, after normalization. */
      t_l = t1 + t2;
      t_t = t2 - (t_l - t1);
    }
    tmp1_l[0] = t_l;
    tmp1_t[0] = t_t;
    /* Imaginary part */
    a_l = tmp1_l[1];
    a_t = tmp1_t[1];
    b_l = tmp2_l[1];
    b_t = tmp2_t[1];
    {
      /* Compute double-double = double-double + double-double. */
      double e, t1, t2;

      /* Knuth trick. */
      t1 = a_l + b_l;
      e = t1 - a_l;
      t2 = ((b_l - e) + (a_l - (t1 - e))) + a_t + b_t;

      /* The result is t1 + t2, after normalization. */
      t_l = t1 + t2;
      t_t = t2 - (t_l - t1);
    }
    tmp1_l[1] = t_l;
    tmp1_t[1] = t_t;
  }                             /* tmp1 = tmp1+tmp2 */

  /* Return r_truth = tmp1 */
  r_true_l[0] = tmp1_l[0];
  r_true_l[1] = tmp1_l[1];
  r_true_t[0] = tmp1_t[0];
  r_true_t[1] = tmp1_t[1];
}                               /* end r_truth */


static void
gen_y_to_cancel(int k, int n, enum blas_conj_type conj,
                void *alpha, void *x, void *y)
/*
 * Purpose
 * =======
 * 
 * Generate Y(i)'s from k to n-1 to cancel as much as possible.
 * 
 */
{
  int i, ii;
  float zero[2] = { 0.0, 0.0 };
  float r[2] = { 0.0, 0.0 };
  float tmp[2];
  double tmpd[2], tmp_l[2], tmp_t[2];
  float *x_i = x, *y_i = y;
  double r_true_l[2], r_true_t[2];

  for (i = k; i < n; ++i) {
    /* y[i] = -rtmp / (alpha * x[i]); */
    r_truth(conj, i, alpha, x, 1, zero, y, 1, r, r_true_l, r_true_t);
    /*printf("i=%d, r_true_l=%10.8e + %10.8ei\n",
       i,r_true_l[0],r_true_l[1]); */
    ii = 2 * i;
    tmp[0] = x_i[ii];
    if (conj == blas_conj)
      tmp[1] = -x_i[ii + 1];
    else
      tmp[1] = x_i[ii + 1];
    c_mul((float *) alpha, tmp, tmp);
    tmpd[0] = tmp[0];
    tmpd[1] = tmp[1];
    if (tmpd[0] == 0. && tmpd[1] == 0.) {
      y_i[ii] = 0.;
      y_i[ii + 1] = 0.;
    } else {
      z_dddivd(r_true_l, r_true_t, tmpd, tmp_l, tmp_t);
      y_i[ii] = -tmp_l[0];
      y_i[ii + 1] = -tmp_l[1];
    }
  }
}


static void
gen_r_to_cancel(int n, enum blas_conj_type conj,
                void *alpha, void *beta, void *x, void *y, void *r, int *seed)
/*
 * Purpose
 * =======
 * 
 * Generate r to cancel as much as possible.
 * 
 */
{
  float zero[2] = { 0.0, 0.0 };
  float rtmp[2] = { 0.0, 0.0 };
  float *beta_i = (float *) beta;
  double beta_d[2];
  float *r_i = (float *) r;
  double r_true_l[2], r_true_t[2];

  if (beta_i[0] == 0.0 && beta_i[1] == 0.0) {
    r_i[0] = xrand(seed);
    r_i[1] = xrand(seed);
  } else {
    r_truth(conj, n, alpha, x, 1, zero, y, 1, rtmp, r_true_l, r_true_t);
    beta_d[0] = beta_i[0];
    beta_d[1] = beta_i[1];
    z_dddivd(r_true_l, r_true_t, beta_d, r_true_l, r_true_t);
    r_i[0] = -r_true_l[0];
    r_i[1] = -r_true_l[1];
  }
}



void
util_xblas_cdot_fill(int n, int n_fix2, int n_mix, int norm,
                  char charconj,
                  void *alpha, int alpha_flag, void *beta, int beta_flag,
                  void *x, void *y, int *seed,
                  void *r, double r_true_l[], double r_true_t[])
/*
 * Purpose
 * =======
 *
 * This routine generates the test vectors X and Y for C_ZDOT.
 *
 * Arguments
 * =========
 * 
 * n       (input) int
 *         The length of the vectors X and Y.
 *
 * n_fix2  (input) int
 *         Number of pairs in the vectors X and Y that are fixed in value,
 *
 * n_mix   (input) int
 *         Number of pairs in the vectors X and Y with X(i) fixed
 *         and Y(i) free in value.
 *
 * norm    (input) int
 *         = -1 : the vectors are scaled with norms near underflow.
 *         = 0  : the vectors have norms of order 1.
 *         = 1  : the vectors are scaled with norms near overflow.
 *
 * conj    (input) enum blas_conj_type
 *
 * alpha   (input/output) void*
 *         If alpha_flag = 1, alpha is input.
 *         If alpha_flag = 0, alpha is output.
 *
 * alpha_flag (input) int
 *         = 0 : alpha is free, and is output.
 *         = 1 : alpha is fixed on input.
 *
 * beta    (input) void*
 *         If beta_flag = 1, beta is input.
 *         If beta_flag = 0, beta is output.
 *
 * beta_flag (input) int
 *         = 0 : beta is free, and is output.
 *         = 1 : beta is fixed on input.
 *
 * x       (input/output) void*
 *
 * y       (input/output) void*
 *
 * seed    (input/output) int* 
 *         The seed for the random number generator.
 * 
 * r       (output) void*
 *         The generated scalar r that will be used as an input to DOT.
 *
 * r_true_l (output) double[]
 *         The leading (real,imaginary) parts of the truth in double-double.
 *
 * r_true_t (output) double[]
 *         The trailing (real,imaginary) parts of the truth in double-double.
 *
 */
{
  int B, frees, y_free, i, ii, k, s;
  float zero[2] = { 0.0, 0.0 };
  float a, b;
  double eps, eps_out;
  float f[2], rtmp[2];
  float *alpha_i = (float *) alpha;
  float *beta_i = (float *) beta;
  float *r_i = (float *) r;
  float *x_i = (float *) x, *y_i = (float *) y;
  enum blas_conj_type conj = blas_no_conj;
  if(charconj == 'c' || charconj == 'C'){
    conj = blas_conj;
  }

  if (alpha_flag == 0) {
    alpha_i[0] = xrand(seed);
    alpha_i[1] = xrand(seed);
  }
  if (beta_flag == 0) {
    beta_i[0] = xrand(seed);
    beta_i[1] = xrand(seed);
  }

  y_free = n - n_fix2;
  k = 2 * n_fix2;
  eps_out = power(2, -BITS_S);

  /*
   * Compute the number of bits in the prefix sum:
   *     alpha * SUM_{i=0,n_fix2-1}(x[i] * y[i])
   */
  r_i[0] = r_i[1] = 0.0;
  r_truth(conj, n_fix2, alpha, x, 1, zero, y, 1, r, r_true_l, r_true_t);
  B = FixedBits(r_true_l[0], r_true_t[0]);      /* real */
  B = MAX(B, FixedBits(r_true_l[1], r_true_t[1]));      /* imag */

  /* Pick r at random */
  r_i[0] = xrand(seed);
  r_i[1] = xrand(seed);

  /* Pick the free X(i)'s at random. */
  for (i = n_fix2 + n_mix; i < n; ++i) {
    ii = 2 * i;
    x_i[ii] = xrand(seed);
    x_i[ii + 1] = xrand(seed);
  }

  if (alpha_flag == 1 && alpha_i[0] == 0.0 && alpha_i[1] == 0.0) {
    /* Pick the free Y(i)'s at random. */
    for (i = n_fix2; i < n; ++i) {
      ii = 2 * i;
      y_i[ii] = xrand(seed);
      y_i[ii + 1] = xrand(seed);
    }
    /* Compute r_truth in double-double */
    r_truth(conj, n, alpha, x, 1, beta, y, 1, r, r_true_l, r_true_t);
    return;
  }

  if (beta_flag == 1 && beta_i[0] == 0.0 && beta_i[1] == 0.0) {
    if (B == 0) {               /* Assume alpha is not very big . */
      switch (y_free) {
      case 0:
        break;
      case 1:
        y_i[k] = xrand(seed);
        y_i[k + 1] = xrand(seed);
        break;
      case 2:
        /*
         * Make SUM_{i=0,1}(x[k+i] * y[k+i]) small ... alpha * eps^2
         */
        if (n_mix == 0) {       /* Both x[k] and x[k+1] free. */
          /* x[k]*y[k] + x[k+1]*y[k+1] = eps_out^2, small.
           * For complex, each real number is multiplied by (i+1),
           * the result is 2i * eps_out^2.
           */
          a = rand_half_1(BITS_S, seed);        /* strictly < 1 */
          x_i[k] = a;           /* real */
          x_i[k + 1] = a;       /* imag */
          y_i[k] = a;
          y_i[k + 1] = a;
          x_i[k + 2] = a + eps_out;     /* exact */
          x_i[k + 3] = a + eps_out;     /* exact */
          y_i[k + 2] = -a + eps_out;    /* exact */
          y_i[k + 3] = -a + eps_out;    /* exact */
        } else if (n_mix == 1) {        /* x[k] fixed, x[k+1] free. */
          /* x[k]*y[k] + x[k+1]*y[k+1] = (eps1 + eps2*i)^2 */
          a = x_i[k];           /* real */
          b = x_i[k + 1];       /* imag */
          if (conj == blas_conj)
            b = -b;
          y_i[k] = a;
          y_i[k + 1] = b;
          eps = ulp(a);
          x_i[k + 2] = a + eps; /* exact */
          y_i[k + 2] = -a + eps;        /* exact */
          eps = ulp(b);
          x_i[k + 3] = b + eps; /* exact */
          if (conj == blas_conj)
            x_i[k + 3] = -x_i[k + 3];
          y_i[k + 3] = -b + eps;        /* exact */
        } else {                /* Both x[k] and x[k+1] fixed; cancel 24 bits. */
          y_i[k] = xrand(seed);
          y_i[k + 1] = xrand(seed);
          gen_y_to_cancel(n_fix2 + 1, n, conj, alpha, x, y);
        }
        break;
      case 3:
        /*
         * Make SUM_{i=0,2}(x[k+i] * y[k+i]) small
         * ... x[k]*y[k] = -x[k+2]*y[k+2] exact, x[k+1]*y[k+1] small
         */
        y_i[k] = -x_i[k + 4];
        y_i[k + 1] = -x_i[k + 5];
        y_i[k + 4] = x_i[k];
        y_i[k + 5] = x_i[k + 1];
        rtmp[0] = x_i[k];
        if (conj == blas_conj) {
          rtmp[1] = -x_i[k + 1];
          y_i[k + 1] = -y_i[k + 1];
          y_i[k + 5] = -y_i[k + 5];
        } else {
          rtmp[1] = x_i[k + 1];
        }
        c_mul(rtmp, &y_i[k], f);
        s = 100;                /* Should be random between 40 and 100. */
        b = power(2, -s);
        f[0] *= b;
        f[1] *= b;
        rtmp[0] = x_i[k + 2];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 3];
        else
          rtmp[1] = x_i[k + 3];
        if (rtmp[0] == 0. && rtmp[1] == 0.)
          y_i[k + 2] = y_i[k + 3] = 0.;
        else
          c_div(f, rtmp, &y_i[k + 2]);
        break;
      case 4:
        /*
         * Make SUM_{i=0,3}(x[k+i] * y[k+i]) small
         * ... x[k]*y[k] = -x[k+3]*y[k+3] exact, x[k+1]*y[k+1] small,
         *     x[k+2]*y[k+2] = 0.
         */
        y_i[k] = -x_i[k + 6];
        y_i[k + 1] = -x_i[k + 7];
        y_i[k + 6] = x_i[k];
        y_i[k + 7] = x_i[k + 1];
        rtmp[0] = x_i[k];
        if (conj == blas_conj) {
          rtmp[1] = -x_i[k + 1];
          y_i[k + 1] = -y_i[k + 1];
          y_i[k + 7] = -y_i[k + 7];
        } else {
          rtmp[1] = x_i[k + 1];
        }
        c_mul(rtmp, &y_i[k], f);
        s = 100;                /* Should be random between 40 and 100. */
        b = power(2, -s);
        f[0] *= b;
        f[1] *= b;
        rtmp[0] = x_i[k + 2];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 3];
        else
          rtmp[1] = x_i[k + 3];
        if (rtmp[0] == 0. && rtmp[1] == 0.)
          y_i[k + 2] = y_i[k + 3] = 0.;
        else
          c_div(f, rtmp, &y_i[k + 2]);
        y_i[k + 4] = 0.0;
        y_i[k + 5] = 0.0;
        break;
      default:                 /* y_free >= 5 */
        /*
         * Make SUM_{i=0,n-1}(x[k+i] * y[k+i]) small.
         * Use 2 to add bits, rest to cancel bits
         * ... Cancel >= 72 bits. */
        y_i[k] = xrand(seed);
        y_i[k + 1] = xrand(seed);
        rtmp[0] = x_i[k];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 1];
        else
          rtmp[1] = x_i[k + 1];
        c_mul(rtmp, &y_i[k], f);
        c_mul(alpha_i, f, f);
        s = 100;                /* Should be random between 40 and 100. */
        b = power(2, -s);
        f[0] *= b;
        f[1] *= b;
        rtmp[0] = x_i[k + 2];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 3];
        else
          rtmp[1] = x_i[k + 3];
        if (rtmp[0] == 0. && rtmp[1] == 0.)
          y_i[k + 2] = y_i[k + 3] = 0.;
        else
          c_div(f, rtmp, &y_i[k + 2]);
        gen_y_to_cancel(n_fix2 + 2, n, conj, alpha, x, y);
      }                         /* end switch */
    } else {                    /* B > 0 */
      if (B >= BITS_E) {        /* Choose Y(i)'s to cancel. */
        gen_y_to_cancel(n_fix2, n, conj, alpha, x, y);
      } else {                  /* At least y[n-1] is free. */
        if (y_free == 1) {
          /* Cancel min(B,24) bits. */
          gen_y_to_cancel(n_fix2, n, conj, alpha, x, y);
        } else {                /* >= 2 frees. */
          /*
           * There are 2 possibilities:
           * (1) use 1 to add bits, and y_free-1 to cancel 24*(y_free-1) bits
           * (2) use all to cancel min(B, 24*y_free) bits
           * Goal is to maximize the # of bits cancelled. By equating (1)
           * and (2), we find the crossover point is y_free = B/24 + 1.
           */
          if (y_free > B / (float) BITS_S + 1) {        /* Use scheme (1) */
            f[0] = f[1] = 0.0;
            r_truth(conj, n_fix2, alpha, x, 1, zero, y, 1, f,
                    r_true_l, r_true_t);
            f[0] = r_true_l[0];
            f[1] = r_true_l[1];
            s = 100;            /* Should be random between 40 and 100. */
            b = power(2, -s);
            f[0] *= b;
            f[1] *= b;
            rtmp[0] = x_i[k];
            if (conj == blas_conj)
              rtmp[1] = -x_i[k + 1];
            else
              rtmp[1] = x_i[k + 1];
            c_mul(alpha_i, rtmp, rtmp);
            if (rtmp[0] == 0. && rtmp[1] == 0.)
              y_i[k] = y_i[k + 1] = 0.;
            else
              c_div(f, rtmp, &y_i[k]);
            gen_y_to_cancel(n_fix2 + 1, n, conj, alpha, x, y);
          } else {              /* Use scheme (2) */
            gen_y_to_cancel(n_fix2, n, conj, alpha, x, y);
          }
        }
      }                         /* end else B < 106 */
    }                           /* end else B > 0 */

    /* Compute r_truth in double-double */
    r_truth(conj, n, alpha, x, 1, zero, y, 1, r, r_true_l, r_true_t);
    return;
  }

  /* if beta == 0 */
  /* Now, beta is non-zero. */
  if (B == 0) {
    switch (y_free) {
    case 0:
      break;
    case 1:
      /* Make alpha*x[k]*y[k] + beta*r small. */
      /* Count number of frees in alpha, x[k], and beta. */
      frees = 0;
      if (alpha_flag == 0)
        ++frees;
      if (beta_flag == 0)
        ++frees;
      if (n_mix == 0)
        ++frees;
      if (frees >= 2) {
        /* alpha*x[k]*y[k] + beta*r = -alpha * eps_out^2
         * For complex, each real number is multiplied by (i+1) to
         * yield final result -2i * alpha * eps_out^2.
         */
        a = rand_half_1(12, seed);      /* strictly < 1, only leading 12 bits */
        r_i[0] = 0.0;           /* real */
        r_i[1] = -a * a * 2;    /* imag, exact */
        if (beta_flag == 1) {   /* beta fixed */
          alpha_i[0] = beta_i[0];
          alpha_i[1] = beta_i[1];
          x_i[k] = a + eps_out; /* real, exact */
          x_i[k + 1] = a + eps_out;     /* imag, exact */
          if (conj == blas_conj)
            x_i[k + 1] = -x_i[k + 1];
          y_i[k] = a - eps_out; /* exact */
          y_i[k + 1] = a - eps_out;     /* exact */
        } else if (n_mix == 1) {        /* x[k] fixed */
          beta_i[0] = x_i[k];
          beta_i[1] = x_i[k + 1];
          if (conj == blas_conj)
            beta_i[1] = -beta_i[1];
          alpha_i[0] = a + eps_out;     /* real, exact */
          alpha_i[1] = a + eps_out;     /* imag, exact */
          y_i[k] = a - eps_out; /* exact */
          y_i[k + 1] = a - eps_out;     /* exact */
        } else {                /* alpha fixed or free, x[k] and beta free */
          beta_i[0] = alpha_i[0];
          beta_i[1] = alpha_i[1];
          x_i[k] = a + eps_out; /* real, exact */
          x_i[k + 1] = a + eps_out;     /* imag, exact */
          if (conj == blas_conj)
            x_i[k + 1] = -x_i[k + 1];
          y_i[k] = a - eps_out; /* exact */
          y_i[k + 1] = a - eps_out;     /* exact */
        }
      } else {                  /* Cancel 24 bits. */
        y_i[k] = xrand(seed);
        y_i[k + 1] = xrand(seed);
        gen_r_to_cancel(n, conj, alpha, beta, x, y, r, seed);
      }
      break;
    case 2:                    /* Actual frees = 3 */
      /* Make SUM_{i=0,1}(alpha * x[k+i] * y[k+i]) + beta*r small. */
      /* Count number of frees in alpha, x[k], and beta. */
      frees = 0;
      if (alpha_flag == 0)
        ++frees;
      if (beta_flag == 0)
        ++frees;
      if (n_mix == 0)
        ++frees;
      if (frees > 0) {
        /*
         * Make alpha*x[k]*y[k] = -beta*r exact, alpha*x[k+1]*y[k+1] small.
         */
        y_i[k] = -1.0;
        y_i[k + 1] = 0.0;
        if (alpha_flag == 0) {  /* alpha free */
          alpha_i[0] = beta_i[0];
          alpha_i[1] = beta_i[1];
          r_i[0] = x_i[k];
          r_i[1] = x_i[k + 1];
          if (conj == blas_conj)
            r_i[1] = -r_i[1];
        } else if (beta_flag == 0) {    /* beta free */
          beta_i[0] = alpha_i[0];
          beta_i[1] = alpha_i[1];
          r_i[0] = x_i[k];
          r_i[1] = x_i[k + 1];
          if (conj == blas_conj)
            r_i[1] = -r_i[1];
        } else {                /* x[k] free */
          x_i[k] = beta_i[0];
          x_i[k + 1] = beta_i[1];
          if (conj == blas_conj)
            r_i[1] = -r_i[1];
          r_i[0] = alpha_i[0];
          r_i[1] = alpha_i[1];
        }
        rtmp[0] = x_i[k];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 1];
        else
          rtmp[1] = x_i[k + 1];
        c_mul(rtmp, &y_i[k], f);
        c_mul(alpha_i, f, f);
        s = 100;                /* Should be random between 40 and 100. */
        b = power(2, -s);
        f[0] *= b;
        f[1] *= b;
        rtmp[0] = x_i[k + 2];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 3];
        else
          rtmp[1] = x_i[k + 3];
        c_mul(alpha_i, rtmp, rtmp);
        if (rtmp[0] == 0. && rtmp[1] == 0.)
          y_i[k + 2] = y_i[k + 3] = 0.;
        else
          c_div(f, rtmp, &y_i[k + 2]);
      } else {                  /* Cancel >= 24 bits. */
        y_i[k] = xrand(seed);
        y_i[k + 1] = xrand(seed);
        gen_y_to_cancel(n_fix2 + 1, n, conj, alpha, x, y);
        gen_r_to_cancel(n, conj, alpha, beta, x, y, r, seed);
      }
      break;
    case 3:
      /* Make SUM_{i=0,2}(alpha * x[k+i] * y[k+i]) + beta*r small
         ... x[k]*y[k] = -x[k+2]*y[k+2] exact, x[k+1]*y[k+1] small,
         ... r = 0        */
      y_i[k] = -x_i[k + 4];
      y_i[k + 1] = -x_i[k + 5];
      y_i[k + 4] = x_i[k];
      y_i[k + 5] = x_i[k + 1];
      rtmp[0] = x_i[k];
      if (conj == blas_conj) {
        rtmp[1] = -x_i[k + 1];
        y_i[k + 1] = -y_i[k + 1];
        y_i[k + 5] = -y_i[k + 5];
      } else {
        rtmp[1] = x_i[k + 1];
      }
      c_mul(rtmp, &y_i[k], f);
      s = 100;                  /* Should be random between 40 and 100. */
      b = power(2, -s);
      f[0] *= b;
      f[1] *= b;
      rtmp[0] = x_i[k + 2];
      if (conj == blas_conj)
        rtmp[1] = -x_i[k + 3];
      else
        rtmp[1] = x_i[k + 3];
      if (rtmp[0] == 0. && rtmp[1] == 0.)
        y_i[k + 2] = y_i[k + 3] = 0.;
      else
        c_div(f, rtmp, &y_i[k + 2]);
      r_i[0] = r_i[1] = 0.0;
      break;
    default:                   /* Actual frees >= 5 */
      if (y_free <= 6) {
        /* Use 2 to add bits, rest to cancel bits ... Cancel >= 72 bits. */
        y_i[k] = xrand(seed);
        y_i[k + 1] = xrand(seed);
        rtmp[0] = x_i[k];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 1];
        else
          rtmp[1] = x_i[k + 1];
        c_mul(rtmp, &y_i[k], f);
        c_mul(alpha_i, f, f);
        s = 100;                /* Should be random between 40 and 100. */
        b = power(2, -s);
        f[0] *= b;
        f[1] *= b;
        rtmp[0] = x_i[k + 2];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 3];
        else
          rtmp[1] = x_i[k + 3];
        if (rtmp[0] == 0. && rtmp[1] == 0.)
          y_i[k + 2] = y_i[k + 3] = 0.;
        else
          c_div(f, rtmp, &y_i[k + 2]);
        gen_y_to_cancel(n_fix2 + 2, n, conj, alpha, x, y);
        gen_r_to_cancel(n, conj, alpha, beta, x, y, r, seed);
      } else {
        /* Use last 5 (4 Y(i)'s and r) to cancel bits, and leading ones
           to add bits. */
        y_i[k] = xrand(seed);
        y_i[k + 1] = xrand(seed);
        rtmp[0] = x_i[k];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 1];
        else
          rtmp[1] = x_i[k + 1];
        c_mul(rtmp, &y_i[k], f);
        c_mul(alpha_i, f, f);
        s = 30;
        b = power(2, -s);
        for (i = n_fix2 + 1; i < n - 4; ++i) {
          f[0] *= b;
          f[1] *= b;
          ii = 2 * i;
          rtmp[0] = x_i[ii];
          if (conj == blas_conj)
            rtmp[1] = -x_i[ii + 1];
          else
            rtmp[1] = x_i[ii + 1];
          c_mul(alpha_i, rtmp, rtmp);
          if (rtmp[0] == 0. && rtmp[1] == 0.)
            y_i[ii] = y_i[ii + 1] = 0.;
          else
            c_div(f, rtmp, &y_i[ii]);
        }
        gen_y_to_cancel(n - 4, n, conj, alpha, x, y);
        gen_r_to_cancel(n, conj, alpha, beta, x, y, r, seed);
        break;
      }
    }                           /* end switch */
  } else {                      /* B > 0 */
    if (B >= BITS_E) {          /* Choose Y(i)'s and r to cancel. */
      gen_y_to_cancel(n_fix2, n, conj, alpha, x, y);
      gen_r_to_cancel(n, conj, alpha, beta, x, y, r, seed);
    } else {                    /* >= 2 frees. Use y[k] to add bits. */
      frees = y_free + 1;
      /*
       * There are 2 possibilities:
       * (1) use 1 to add bits, and y_free-1 to cancel 24*(y_free-1) bits
       * (2) use all to cancel min(B, 24*y_free) bits
       * Goal is to maximize the # of bits cancelled. By equating (1)
       * and (2), we find the crossover point is y_free = B/24 + 1.
       */
      if (frees > B / (float) BITS_S + 1) {     /* Use scheme (1) */
        f[0] = f[1] = 0.0;
        r_truth(conj, n_fix2, alpha, x, 1, zero, y, 1, f, r_true_l, r_true_t);
        f[0] = r_true_l[0];
        f[1] = r_true_l[1];
        s = 100;                /* Should be random between 40 and 100. */
        b = power(2, -s);
        f[0] *= b;
        f[1] *= b;
        rtmp[0] = x_i[k];
        if (conj == blas_conj)
          rtmp[1] = -x_i[k + 1];
        else
          rtmp[1] = x_i[k + 1];
        c_mul(alpha_i, rtmp, rtmp);
        if (rtmp[0] == 0. && rtmp[1] == 0.)
          y_i[k] = y_i[k + 1] = 0.;
        else
          c_div(f, rtmp, &y_i[k]);
        gen_y_to_cancel(n_fix2 + 1, n, conj, alpha, x, y);
      } else {                  /* Use scheme (2) */
        gen_y_to_cancel(n_fix2, n, conj, alpha, x, y);
      }
      gen_r_to_cancel(n, conj, alpha, beta, x, y, r, seed);
    }
  }

  /* Compute r_truth in double-double */
  r_truth(conj, n, alpha, x, 1, beta, y, 1, r, r_true_l, r_true_t);
}                               /* testgen_BLAS_cdot */
