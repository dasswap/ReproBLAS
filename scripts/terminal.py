import argparse
import json
import multiprocessing
import os
import re
import subprocess
import sys

from scripts.cpuinfo import cpuinfo

import config

def callsafe(command, verbose="false"):
  if(verbose == "true"):
    print(command)
  rc = 0
  try:
    if(sys.stdout.encoding):
      env = os.environ.copy()
      env["PYTHONIOENCODING"] = sys.stdout.encoding
      out = subprocess.check_output(command, env=env, stderr=subprocess.STDOUT, shell=True).decode(sys.stdout.encoding)
    else:
      out = subprocess.check_output(command, stderr=subprocess.STDOUT, shell=True)
  except subprocess.CalledProcessError as e:
    rc = -e.returncode
    out = e.output.decode(sys.stdout.encoding)
  if(verbose == "true"):
    print(out)
  return (rc, out)

def call(command, verbose="false"):
  if(verbose == "true"):
    print(command)
  if sys.stdout.encoding:
    env = os.environ.copy()
    env["PYTHONIOENCODING"] = sys.stdout.encoding
    out = subprocess.check_output(command, env=env, shell=True).decode(sys.stdout.encoding)
  else:
    out = subprocess.check_output(command, shell=True)
  if(verbose == "true"):
    print(out)
  return out

def make_call(command, verbose="false"):
  output = call(command, verbose=verbose).split("\n")
  for line in output[::-1]:
    if not re.match("make\[\d+\]:", line) and line != "":
      return line

top = make_call("make top")

def in_directory(path, directory):
  path = os.path.realpath(path)
  directory = os.path.join(os.path.realpath(directory), '')
  return os.path.commonprefix([path, directory]) == directory;

made = {}

def make_clean(directory, verbose="false"):
  call("cd {0}; make clean".format(os.path.join(top, directory)), verbose=verbose)
  unclean = list(made.keys())
  for executable in unclean:
    if in_directory(executable, directory):
      made.pop(executable)

def make(executable, args = None, id = None, remake = False, verbose="false"):
  if(executable not in made):
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
      if remake:
        callsafe("rm -f {}".format(result), verbose=verbose)
      env = ""
      if args:
        env = "ARGS={}".format(args)
      callsafe("make -j {} {} {}".format(multiprocessing.cpu_count(), result, env), verbose=verbose)
      assert os.path.isfile(result), "Error: make unsuccessful."
      if id:
        call("cp {} {}".format(result, build), verbose=verbose)
      assert os.path.isfile(build), "Error: make unsuccessful."
    made[executable] = build
  return made[executable]
make.build_dir = {}

def flags(params, args):
  params = [list(param) if type(param) == tuple else [param] for param in params]
  args = [list(arg) if type(arg) == tuple else [arg] for arg in args]
  params = [param for l in params for param in l]
  args = [arg for l in args for arg in l]
  return " ".join(['-{0} "{1}"'.format(param, arg) if len(param) == 1 else '--{0} "{1}"'.format(param, arg) for (param, arg) in zip(params, args)])

def get_vectorization(verbose="false"):
  if not get_vectorization.vectorization:
    try:
      getter_file = open(os.path.join(top, "scripts/getter.json"), "r")
    except (IOError, FileNotFoundError):
      print("Error: {} not found.".format(os.path.join(top, "scripts/getter.json")))
      print('Hint: Did you forget to run "make update"?')
      raise
    getter = json.load(getter_file)
    getter_file.close()
    get_vectorization.vectorization = getter["vectorization"]
  return get_vectorization.vectorization
get_vectorization.vectorization = None

def get_dimaxindex(verbose="false"):
  if not get_dimaxindex.dimaxindex:
    try:
      getter_file = open(os.path.join(top, "scripts/getter.json"), "r")
    except (IOError, FileNotFoundError):
      print("Error: {} not found.".format(os.path.join(top, "scripts/getter.json")))
      print('Hint: Did you forget to run "make update"?')
      raise
    getter = json.load(getter_file)
    getter_file.close()
    get_dimaxindex.dimaxindex = getter["dimaxindex"]
  return get_dimaxindex.dimaxindex
get_dimaxindex.dimaxindex = None

def get_simaxindex(verbose="false"):
  if not get_simaxindex.simaxindex:
    try:
      getter_file = open(os.path.join(top, "scripts/getter.json"), "r")
    except (IOError, FileNotFoundError):
      print("Error: {} not found.".format(os.path.join(top, "scripts/getter.json")))
      print('Hint: Did you forget to run "make update"?')
      raise
    getter = json.load(getter_file)
    getter_file.close()
    get_simaxindex.simaxindex = getter["simaxindex"]
  return get_simaxindex.simaxindex
get_simaxindex.simaxindex = None

def get_dimaxfold(verbose="false"):
  if not get_dimaxfold.dimaxfold:
    try:
      getter_file = open(os.path.join(top, "scripts/getter.json"), "r")
    except (IOError, FileNotFoundError):
      print("Error: {} not found.".format(os.path.join(top, "scripts/getter.json")))
      print('Hint: Did you forget to run "make update"?')
      raise
    getter = json.load(getter_file)
    getter_file.close()
    get_dimaxfold.dimaxfold = getter["dimaxfold"]
  return get_dimaxfold.dimaxfold
get_dimaxfold.dimaxfold = None

def get_simaxfold(verbose="false"):
  if not get_simaxfold.simaxfold:
    try:
      getter_file = open(os.path.join(top, "scripts/getter.json"), "r")
    except (IOError, FileNotFoundError):
      print("Error: {} not found.".format(os.path.join(top, "scripts/getter.json")))
      print('Hint: Did you forget to run "make update"?')
      raise
    getter = json.load(getter_file)
    getter_file.close()
    get_simaxfold.simaxfold = getter["simaxfold"]
  return get_simaxfold.simaxfold
get_simaxfold.simaxfold = None

def get_didefaultfold(verbose="false"):
  if not get_didefaultfold.didefaultfold:
    try:
      getter_file = open(os.path.join(top, "scripts/getter.json"), "r")
    except (IOError, FileNotFoundError):
      print("Error: {} not found.".format(os.path.join(top, "scripts/getter.json")))
      print('Hint: Did you forget to run "make update"?')
      raise
    getter = json.load(getter_file)
    getter_file.close()
    get_didefaultfold.didefaultfold = getter["didefaultfold"]
  return get_didefaultfold.didefaultfold
get_didefaultfold.didefaultfold = None

def get_sidefaultfold(verbose="false"):
  if not get_sidefaultfold.sidefaultfold:
    try:
      getter_file = open(os.path.join(top, "scripts/getter.json"), "r")
    except (IOError, FileNotFoundError):
      print("Error: {} not found.".format(os.path.join(top, "scripts/getter.json")))
      print('Hint: Did you forget to run "make update"?')
      raise
    getter = json.load(getter_file)
    getter_file.close()
    get_sidefaultfold.sidefaultfold = getter["sidefaultfold"]
  return get_sidefaultfold.sidefaultfold
get_sidefaultfold.sidefaultfold = None

def get_diendurance(verbose="false"):
  if not get_diendurance.diendurance:
    try:
      getter_file = open(os.path.join(top, "scripts/getter.json"), "r")
    except (IOError, FileNotFoundError):
      print("Error: {} not found.".format(os.path.join(top, "scripts/getter.json")))
      print('Hint: Did you forget to run "make update"?')
      raise
    getter = json.load(getter_file)
    getter_file.close()
    get_diendurance.diendurance = getter["diendurance"]
  return get_diendurance.diendurance
get_diendurance.diendurance = None

def get_siendurance(verbose="false"):
  if not get_siendurance.siendurance:
    try:
      getter_file = open(os.path.join(top, "scripts/getter.json"), "r")
    except (IOError, FileNotFoundError):
      print("Error: {} not found.".format(os.path.join(top, "scripts/getter.json")))
      print('Hint: Did you forget to run "make update"?')
      raise
    getter = json.load(getter_file)
    getter_file.close()
    get_siendurance.siendurance = getter["siendurance"]
  return get_siendurance.siendurance
get_siendurance.siendurance = None

def my_get_cpu_info():
  try:
    return cpuinfo.get_cpu_info()
  except Exception:
    return None

def get_cpu_freq(verbose="false"):
  info = my_get_cpu_info()
  if info and "freq" not in config.cpu_info(verbose="false"):
    return info["hz_actual_raw"][0] * 10**(info["hz_actual_raw"][1])
  else:
    assert "freq" in config.cpu_info(verbose=verbose), "ReproBLAS error: CPU frequency not found. Clarify in cpu_info() in config.py"
    return config.cpu_info()["freq"]

def get_fma(verbose="false"):
  info = my_get_cpu_info()
  if info and "fma" not in config.cpu_info(verbose="false"):
    return "fma" in info["flags"]
  else:
    assert "fma" in config.cpu_info(verbose=verbose), "ReproBLAS error: fma status not found. Clarify in cpu_info() in config.py"
    return config.cpu_info()["fma"]

def get_cache(verbose="false"):
  info = my_get_cpu_info()
  if info and "cache" not in config.cpu_info(verbose="false"):
    return int(info['l2_cache_size']) * 1024
  else:
    assert "cache" in config.cpu_info(verbose=verbose), "ReproBLAS error: cache size not found. Clarify in cpu_info() in config.py"
    return config.cpu_info()["cache"]

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
  data["fma"] = get_fma(verbose=verbose);
  for key in data:
    if key in output:
      data[key] = output[key]
  return config.peak_time(data)

def get_flop_count(output, verbose="false"):
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
  data["fma"] = get_fma(verbose=verbose);
  for key in data:
    if key in output:
      data[key] = output[key]
  return config.flop_count(data)
