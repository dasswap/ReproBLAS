import argparse
import multiprocessing
import os
import subprocess
import sys
import config
import re

from scripts.cpuinfo import cpuinfo

def callsafe(command, verbose="false"):
  if(verbose == "true"):
    print(command)
  rc = 0
  try:
    out = subprocess.check_output(command, stderr=subprocess.STDOUT, shell=True).decode(sys.stdout.encoding)
  except subprocess.CalledProcessError as e:
    rc = e.returncode
    out = e.output.decode(sys.stdout.encoding)
  if(verbose == "true"):
    print(out)
  return (rc, out)

def call(command, verbose="false"):
  if(verbose == "true"):
    print(command)
  out = subprocess.check_output(command, shell=True).decode(sys.stdout.encoding)
  if(verbose == "true"):
    print(out)
  return out

def make_call(command, verbose="false"):
  output = call(command, verbose=verbose).split("\n")
  for line in output[::-1]:
    if not re.match("make\[\d+\]:", line) and line != "":
      return line

top = make_call("make top")

def make_clean(location, verbose="false"):
  call("cd {0}; make clean".format(os.path.join(top, location)), verbose=verbose)

def make(executable, args = None, id = None, remake = False, verbose="false"):
  executable_dir = os.path.join(top, os.path.split(executable)[0])
  executable_name = os.path.split(executable)[1]
  if executable_dir not in make.build_dir:
    make.build_dir[executable_dir] = make_call("cd {0}; make pbd".format(executable_dir), verbose=verbose)
  build_dir = make.build_dir[executable_dir]
  build_name = executable_name
  if id:
    build_name = "{}__{}{}".format(os.path.splitext(executable_name)[1], id, os.path.splitext(executable_name[0]))
  build = os.path.join(build_dir, build_name)
  if not os.path.isfile(build) or remake:
    result = os.path.join(build_dir, executable_name)
    callsafe("rm -f {}".format(result), verbose=verbose)
    env = ""
    if args:
      env = "ARGS={}".format(args)
    callsafe("make -j {} {} {}".format(multiprocessing.cpu_count(), result, env), verbose=verbose)
    assert os.path.isfile(result), "Error: make unsuccessful."
    if id:
      call("cp {} {}".format(result, build), verbose=verbose)
    assert os.path.isfile(build), "Error: make unsuccessful."
  return build
make.build_dir = {}

def flags(params, args):
  params = [list(param) if type(param) == tuple else [param] for param in params]
  args = [list(arg) if type(arg) == tuple else [arg] for arg in args]
  params = [param for l in params for param in l]
  args = [arg for l in args for arg in l]
  return " ".join(['-{0} "{1}"'.format(param, arg) if len(param) == 1 else '--{0} "{1}"'.format(param, arg) for (param, arg) in zip(params, args)])

def get_vectorization(verbose="false"):
  if get_vectorization.vectorization == "":
    make("scripts/get_vectorization.c", remake = False, verbose=verbose)
    get_vectorization.vectorization = call(make("scripts/get_vectorization", remake = False, verbose=verbose), verbose=verbose).split()[0]
  return get_vectorization.vectorization
get_vectorization.vectorization = ""

def get_max_fold(verbose="false"):
  if get_max_fold.max_fold == 0:
    get_max_fold.max_fold = int(call(make("scripts/get_max_fold", remake = False, verbose=verbose), verbose=verbose).split()[0])
  return get_max_fold.max_fold
get_max_fold.max_fold = 0

def get_default_fold(verbose="false"):
  if get_default_fold.default_fold == 0:
    get_default_fold.default_fold = int(call(make("scripts/get_default_fold", remake = False, verbose=verbose), verbose=verbose).split()[0])
  return get_default_fold.default_fold
get_default_fold.default_fold = 0

def get_cpu_freq(verbose="false"):
  info = cpuinfo.get_cpu_info()
  return info["hz_actual_raw"][0] * 10**(info["hz_actual_raw"][1])

def get_fma(verbose="false"):
  info = cpuinfo.get_cpu_info()
  return "fma" in info["flags"]

def get_peak_time(output, verbose="false"):
  data = {}
  data["s_add"] = 0;
  data["s_mul"] = 0;
  data["s_fma"] = 0;
  data["s_cmp"] = 0;
  data["s_orb"] = 0;
  data["d_add"] = 0;
  data["d_mul"] = 0;
  data["d_fma"] = 0;
  data["d_cmp"] = 0;
  data["d_orb"] = 0;
  data["vec"] = get_vectorization(verbose=verbose);
  data["freq"] = get_cpu_freq(verbose=verbose);
  for key in data:
    if key in output:
      data[key] = output[key]
  return config.peak_time(data)

