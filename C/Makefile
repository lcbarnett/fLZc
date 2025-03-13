# Makefile for LZ78c mex and test targets

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

MSRC = LZ78c_mex.c LZ78c_mex_x.c LZ78c_rand_mex.c LZ78c_rand_x_mex.c
CSRC = LZ78c.c mt64.c LZ78c_matlab.c
MBIN = $(patsubst %.c,$(MEXDIR)/%.$(MEXEXT),$(MSRC))
MDEP = $(CSRC) $(patsubst %.c,%.h,$(CSRC))

ZSRC = LZ78c.c mt64.c
ZOBJ = $(patsubst %.c,.%.$(OBJEXT),$(ZSRC))

TSRC = LZ78c_test.c LZ78c_benchmark.c
TDEP = LZ78c.h
TOBJ = $(patsubst %.c,.%.$(OBJEXT),$(TSRC))
TBIN = $(patsubst %.c,%$(BINEXT),$(TSRC))

MFLAGS = -std=c99 -march=native
# WFLAGS = -Wall -Wextra -Wconversion -Winline -pedantic-errors
# khashl may well whinge about inlining failures
WFLAGS = -Wall -Wextra -Wconversion -pedantic-errors
DFLAGS = -D_POSIX_C_SOURCE=199309L -D_DEFAULT_SOURCE
IFLAGS =
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
MEXFLAGS = -O -largeArrayDims

CC  = gcc
MEX = mex

.PHONY: all mex test clean mex-clean test-clean diag

all: $(MBIN) $(TBIN)

mex: $(MBIN)

test: $(TBIN)

clean:
	$(RM) $(RMBIN) $(TOBJ) $(ZOBJ) $(TBIN)

mex-clean:
	$(RM) $(RMBIN)

test-clean:
	$(RM) $(TOBJ) $(ZOBJ) $(TBIN)

diag:
	@echo "*** OS     = " $(OS) [$(ComSpec)]
	@echo
	@echo "*** CSRC   = " $(CSRC)
	@echo
	@echo "*** TSRC   = " $(TSRC)
	@echo "*** TBIN   = " $(TBIN)
	@echo "*** TOBJ   = " $(TOBJ)
	@echo "*** TDEP   = " $(TDEP)
	@echo
	@echo "*** ZSRC   = " $(ZSRC)
	@echo "*** ZOBJ   = " $(ZOBJ)
	@echo
	@echo "*** MSRC   = " $(MSRC)
	@echo "*** MBIN   = " $(MBIN)
	@echo "*** RMBIN  = " $(RMBIN)
	@echo "*** MDEP   = " $(MDEP)
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

$(ZOBJ): $(TDEP)

$(ZOBJ): .%.$(OBJEXT): %.c
	$(CC) -c $(GCCFLAGS) $< -o $@

$(TOBJ): $(ZOBJ)

$(TOBJ): .%.$(OBJEXT): %.c
	$(CC) -c $(GCCFLAGS) $< -o $@

$(TBIN): %$(BINEXT): .%.$(OBJEXT)
	$(CC) $(GCCFLAGS) $< $(LIBS) $(ZOBJ) -o $@
