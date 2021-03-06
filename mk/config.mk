# Build mode e.g. debug, profile, release.  Build specific mode flags
# can be entered in $(MK)/build-$(BUILD_MODE).mk file e.g. for debug
# following seems to be a reasonable contents
#CFLAGS   += -ggdb
#CXXFLAGS += -ggdb
#CPPFLAGS += -DDEBUG
#LDFLAGS  += -ggdb
# If you don't plan on having different build modes then just comment
# below or set it to empty.
ifeq ($(BUILD_MODE),)
  BUILD_MODE := release
endif

# To have some per directory setting automatically propagated to all
# subdirs then uncomment below.  That way you can have all project
# compiled with "global" settings and easily switch flags for some
# subtree just by setting per directory settings at the top dir of the
# subtree.  You may of course overwrite inherited values and you can
# turn inheritance in some part by just clearing INHERIT_DIR_VARS_$(d)
# This is a global inheritance flag - you might want to turn it on only
# in some directory (just set INHERIT_DIR_VARS_$(d) there).
INHERIT_DIR_VARS := INCLUDES CPPFLAGS CFLAGS LDFLAGS MPICFLAGS MPILDFLAGS

# Detect /dev/null
ifeq ($(shell if test -w /dev/null; then echo "1"; else echo "0"; fi), 1)
  DEVNULL := /dev/null
else ifeq ($(shell if test -w nul; then echo "1"; else echo "0"; fi), 1)
  DEVNULL := nul
endif

# Here's a good place to translate some of these settings into
# compilation flags/variables.  As an example a preprocessor macro for
# target endianess
ifeq ($(ENDIAN),big)
  CPPFLAGS += -DBIG_ENDIAN
else
  CPPFLAGS += -DLITTLE_ENDIAN
endif

# Detect C compiler in the following order if CC hasn't been set
ifeq ($(CC),)
  ifeq ($(shell test -x "$(shell which gcc 2>$(DEVNULL))"; echo $$?), 0)
    CC := gcc
  else ifeq ($(shell test -x "$(shell which icc 2>$(DEVNULL))"; echo $$?), 0)
    CC := icc
  else ifeq ($(shell test -x "$(shell which pgcc 2>$(DEVNULL))"; echo $$?), 0)
    CC := pgcc
  else ifeq ($(shell test -x "$(shell which craycc 2>$(DEVNULL))";  echo $$?), 0)
    CC := craycc
  else ifeq ($(shell test -x "$(shell which clang 2>$(DEVNULL))";  echo $$?), 0)
    CC := clang
  else
    CC := cc
  endif
endif

# Default optimization flags.
ifeq ($(OPTFLAGS),)
  OPTFLAGS := -O3
endif

# Detect MPI C compiler in the following order
ifeq ($(MPICC),)
  ifeq ($(shell test -x "$(shell which mpicc 2>$(DEVNULL))"; echo $$?), 0)
    MPICC := mpicc
  else
    MPICC :=
  endif
endif

# Detect MPI C compiler flags in the following order if MPICFLAGS hasn't been set
ifeq ($(MPICFLAGS),)
  ifeq ($(MPICC), mpicc)
    ifeq ("$(shell mpicc --showme:compile &>$(DEVNULL); echo $$?)", 0)
      MPICFLAGS := $(shell $(MPICC) --showme:compile)
    else ifeq ("$(shell mpicc -compile_info &>$(DEVNULL); echo $$?)", 0)
      MPICFLAGS := $(shell $(MPICC) -compile_info)
    endif
  endif
endif

# Detect MPI C linker flags in the following order if MPILDFLAGS hasn't been set
ifeq ($(MPILDFLAGS),)
  ifeq ($(MPICC), mpicc)
    ifeq ("$(shell mpicc --showme:link &>$(DEVNULL); echo $$?)", 0)
      MPILDFLAGS := $(shell $(MPICC) --showme:link)
    else ifeq ("$(shell mpicc -link_info &>$(DEVNULL); echo $$?)", 0)
      MPILDFLAGS := $(shell $(MPICC) -link_info)
    endif
  endif
endif

# Detect python in the following order if PYTHON hasn't been set
ifeq ($(PYTHON),)
  ifeq ($(shell test -x "$(shell which python3 2>$(DEVNULL))"; echo $$?), 0)
    PYTHON := python3
  else ifeq ($(shell test -x "$(shell which python 2>$(DEVNULL))"; echo $$?), 0)
    ifeq ($(shell test "$(shell python -V 2>&1)" \> "Python 2.7.0"; echo $$?), 0)
      PYTHON := python
    endif
  endif
endif

# Use vectorization flags to determine compiler flags. MTARGET_ARCH is used to determine the target architectures vectorization settings.

ifeq ($(strip $(SSE2)),false)
  CFLAGS += -DreproBLAS_no__SSE2__
endif
ifeq ($(strip $(AVX)),false)
  CFLAGS += -DreproBLAS_no__AVX__
endif

ifeq ($(MTARGET_ARCH),)
  CFLAGS += -march=native
else
  CFLAGS += -march=$(strip $(MTARGET_ARCH))
endif

CALL_PYTHON = PYTHONPATH=$(TOP) $(PYTHON)

# Create cog compiler
COG = $(CALL_PYTHON) -m scripts.cogapp $(COGFLAGS)

# Create cog compiler
PSEUDOCOG = $(TOP)/scripts/pseudocog.sh

# select coloring (uncomment if you don't like coloring of the output)
COLOR_TTY := false
