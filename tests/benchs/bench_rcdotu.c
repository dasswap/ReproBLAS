#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <idxdBLAS.h>
#include <reproBLAS.h>

#include "../common/test_opt.h"
#include "../common/test_time.h"
#include "../common/test_metric.h"

#include "../../config.h"

#include "bench_vecvec_fill_header.h"

static opt_option fold;

static void bench_rcdotu_options_initialize(void){
  fold._int.header.type       = opt_int;
  fold._int.header.short_name = 'k';
  fold._int.header.long_name  = "fold";
  fold._int.header.help       = "fold";
  fold._int.required          = 0;
  fold._int.min               = 2;
  fold._int.max               = idxd_SIMAXFOLD;
  fold._int.value             = SIDEFAULTFOLD;
}

int bench_vecvec_fill_show_help(void){
  bench_rcdotu_options_initialize();

  opt_show_option(fold);

  return 0;
}

const char* bench_vecvec_fill_name(int argc, char** argv){
  (void)argc;
  (void)argv;
  static char name_buffer[MAX_LINE];

  bench_rcdotu_options_initialize();
  opt_eval_option(argc, argv, &fold);

  snprintf(name_buffer, MAX_LINE * sizeof(char), "Benchmark [rcdotu] (fold = %d)", fold._int.value);
  return name_buffer;
}

int bench_vecvec_fill_test(int argc, char** argv, int N, int FillX, double RealScaleX, double ImagScaleX, int incX, int FillY, double RealScaleY, double ImagScaleY, int incY, int trials){
  (void)argc;
  (void)argv;
  int rc = 0;
  int i;
  float complex res = 0.0;

  bench_rcdotu_options_initialize();
  opt_eval_option(argc, argv, &fold);

  util_random_seed();

  float complex *X = util_cvec_alloc(N, incX);
  float complex *Y = util_cvec_alloc(N, incY);

  //fill X and Y
  util_cvec_fill(N, X, incX, FillX, RealScaleX, ImagScaleX);
  util_cvec_fill(N, Y, incY, FillY, RealScaleY, ImagScaleY);

  time_tic();
  for(i = 0; i < trials; i++){
    reproBLAS_rcdotu_sub(fold._int.value, N, X, incX, Y, incY, &res);
  }
  time_toc();

  double dN = (double)N;
  metric_load_double("time", time_read());
  metric_load_float("res_real", crealf(res));
  metric_load_float("res_imag", cimagf(res));
  metric_load_double("trials", (double)trials);
  metric_load_double("input", 2.0 * dN);
  metric_load_double("output", 1.0);
  metric_load_double("normalizer", dN);
  metric_load_double("s_mul", 4.0 * dN);
  metric_load_double("s_add", (3 * fold._int.value - 2) * 4.0 * dN);
  metric_load_double("s_orb", fold._int.value * 4.0 * dN);
  metric_dump();

  free(X);
  free(Y);
  return rc;
}
