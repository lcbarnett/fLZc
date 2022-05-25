# Makefile for LZc mex and test targets

$(info )
ifdef GLIBVER
ifdef DEBUG
$(info *** Building with Glib hash table (DEBUG) ***)
else
$(info *** Building with Glib hash table ***)
endif
else
ifdef DEBUG
$(info *** Building with Khash hash table (DEBUG) ***)
else
$(info *** Building with Khash hash table ***)
endif
endif
$(info )

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

MSRC = LZc_mex.c LZc_mex_x.c LZc_mex_cmax.c LZc_mex_cmax_x.c LZc_mex_id.c LZ76c_mex.c LZ76c_mex_x.c
CSRC = LZc.c mt64.c LZc_matlab.c
MBIN = $(patsubst %.c,$(MEXDIR)/%.$(MEXEXT),$(MSRC))
MDEP = $(CSRC) $(patsubst %.c,%.h,$(CSRC))

ZSRC = LZc.c mt64.c
ZOBJ = $(patsubst %.c,.%.$(OBJEXT),$(ZSRC))

TSRC = LZc_test.c LZc_benchmark.c LZc_check.c
TDEP = LZc.h
TOBJ = $(patsubst %.c,.%.$(OBJEXT),$(TSRC))
TBIN = $(patsubst %.c,%$(BINEXT),$(TSRC))

MFLAGS = -std=c99 -march=native
ifdef GLIBVER
	WFLAGS = -Wall -Wextra -Wconversion -Winline -pedantic-errors -Werror
	DFLAGS = -D_POSIX_C_SOURCE=199309L -D_DEFAULT_SOURCE -DGLIBVER
	IFLAGS = $(shell pkg-config --cflags glib-2.0)
	LIBS   = $(shell pkg-config --libs glib-2.0)
else
	WFLAGS = -Wall -Wextra -Wconversion -Winline -pedantic-errors
	DFLAGS = -D_POSIX_C_SOURCE=199309L -D_DEFAULT_SOURCE
	IFLAGS =
	LIBS   =
endif

ifdef DEBUG
	DFLAGS := $(DFLAGS) -DDEBUG
endif

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
