# Makefile for LZc mex and standalone targets
#
# NOTE: to compile the Matlab-based standalones (see NSRC below)
# MATLABSTANDALONES must be defined when calling `make'.

ifdef ComSpec
	OS = WIN
else
	UNAME = $(shell uname)
	ifeq ($(UNAME),Linux)
		OS = LINUX
	else
		ifeq ($(UNAME),Darwin)
			OS = MAC
		else
			OS = UNKNOWN
		endif
	endif
endif

# Linux
ifeq ($(OS),LINUX)
	BINEXT  =
	OBJEXT  = o
endif

# Darwin
ifeq ($(OS),MAC)
	BINEXT  =
	OBJEXT  = o
endif

# Windows
ifeq ($(OS),WIN)
	BINEXT  = .exe
	OBJEXT  = obj
endif

MEXEXT = $(shell mexext)
MEXDIR = ../matlab/mex

CSRC = LZ78c.c mt64.c LZ78c_matlab.c

ZSRC = LZ78c.c mt64.c
ZOBJ = $(patsubst %.c,.%.$(OBJEXT),$(ZSRC))

MSRC = LZ78c_mex.c LZ78c_x_mex.c
MBIN = $(patsubst %.c,$(MEXDIR)/%.$(MEXEXT),$(MSRC))
MDEP = $(CSRC) $(patsubst %.c,%.h,$(CSRC))

ifdef MATLABSTANDALONES
	NSRC = LZ78c_randmv.c
	NBIN = $(patsubst %.c,%$(BINEXT),$(NSRC))
	NDEP = $(CSRC) $(NSRC) $(patsubst %.c,%.h,$(CSRC))
endif

TSRC = LZ78c_test.c LZ78c_benchmark.c
TDEP = LZ78c.h
TOBJ = $(patsubst %.c,.%.$(OBJEXT),$(TSRC))
TBIN = $(patsubst %.c,%$(BINEXT),$(TSRC))

MFLAGS = -std=c99 -march=native
WFLAGS = -Wall -Wextra -Wconversion -pedantic-errors
DFLAGS = -D_POSIX_C_SOURCE=199309L -D_DEFAULT_SOURCE
IFLAGS =

ifdef MATLABSTANDALONES
	IMFLAGS = -I$(MATLAB_PATH)/extern/include
endif

LIBS   =

ifeq ($(OS),WIN)
	OFLAGS = -O3
	MEXCFLAGS = COMPFLAGS
	DFLAGS := $(DFLAGS) -DWIN
	RM = del /F /Q
	RMBIN = $(subst /,\,$(MBIN))
else
	OFLAGS = -O3 -flto
	MEXCFLAGS = CFLAGS
	RM = rm -f
	RMBIN = $(MBIN)
	LIBS:= $(LIBS) -lm
endif

GCCFLAGS = $(MFLAGS) $(OFLAGS) $(WFLAGS) $(DFLAGS) $(IFLAGS)
MEXFLAGS = -O -R2018a

CC  = gcc
MEX = mex

.PHONY: all mex test clean mex-clean test-clean diag

all: $(MBIN) $(NBIN) $(TBIN)

mex: $(MBIN)

test: $(TBIN)

clean:
	$(RM) $(RMBIN) $(NBIN) $(TOBJ) $(ZOBJ) $(TBIN)

mex-clean:
	$(RM) $(RMBIN)

test-clean:
	$(RM) $(TOBJ) $(ZOBJ) $(TBIN)

diag:
	@echo "*** OS        = " $(OS) [$(ComSpec)]
	@echo
	@echo "*** CSRC      = " $(CSRC)
	@echo
	@echo "*** TSRC      = " $(TSRC)
	@echo "*** TBIN      = " $(TBIN)
	@echo "*** TOBJ      = " $(TOBJ)
	@echo "*** TDEP      = " $(TDEP)
	@echo
	@echo "*** ZSRC      = " $(ZSRC)
	@echo "*** ZOBJ      = " $(ZOBJ)
	@echo
	@echo "*** MSRC      = " $(MSRC)
	@echo "*** MBIN      = " $(MBIN)
	@echo "*** RMBIN     = " $(RMBIN)
	@echo "*** MDEP      = " $(MDEP)
	@echo
	@echo "*** NSRC      = " $(NSRC)
	@echo "*** NBIN      = " $(NBIN)
	@echo "*** NDEP      = " $(NDEP)
	@echo
	@echo "*** MEXFLAGS  = " $(MEXFLAGS)
	@echo "*** MEXCFLAGS = " $(MEXCFLAGS)
	@echo "*** MEXEXT    = " $(MEXEXT)
	@echo "*** MEXDIR    = " $(MEXDIR)
	@echo
	@echo "*** CC        = " $(CC) -c $(GCCFLAGS)
	@echo "*** MEX       = " $(MEX) $(MEXFLAGS) $(MEXCFLAGS)="\$$CFLAGS $(GCCFLAGS) -static" $(LIBS) -outdir $(MEXDIR)

$(MEXDIR)/%.$(MEXEXT): %.c $(MDEP)
	$(MEX) $(MEXFLAGS) $(MEXCFLAGS)="\$$CFLAGS $(GCCFLAGS) -static" $(LIBS) -outdir $(MEXDIR) $< $(CSRC)

ifdef MATLABSTANDALONES
$(NBIN): $(NDEP)
	$(MEX) -client engine $(MEXFLAGS) $(MEXCFLAGS)="\$$CFLAGS $(GCCFLAGS) $(IMFLAGS)" $(LIBS) $(NSRC) $(CSRC)
endif

$(ZOBJ): $(TDEP)

$(ZOBJ): .%.$(OBJEXT): %.c
	$(CC) -c $(GCCFLAGS) $< -o $@

$(TOBJ): $(ZOBJ)

$(TOBJ): .%.$(OBJEXT): %.c
	$(CC) -c $(GCCFLAGS) $< -o $@

$(TBIN): %$(BINEXT): .%.$(OBJEXT)
	$(CC) $(GCCFLAGS) $< $(LIBS) $(ZOBJ) -o $@
