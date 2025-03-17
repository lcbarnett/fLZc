# NOTE 1: Standalone binary executables are placed in the `bin' directory. Matlab
# mex binaries are placed in the `../matlab/mex' directory.
#
# NOTE 2: To build the  standalone binary executables and mex binaries, you will
# need to define an environmental variable called MATLAB_PATH which points to the
# root path of your Matlab installation; then build with
#
#     make
#
# NOTE 3: The maintainer binary executables (see NBIN) are not compiled by default,
# as they are mostly for the benefit of the maintainer. Build with
#
#     make maint
#
# The executables will have to be run (on Linux, MacOS) with
#
#     LD_LIBRARY_PATH=$MATLAB_PATH/bin/glnxa64 bin/<executable>
#
# (thanks Matlab! ...I tried to hard-code the library path, but failed
# miserably; for reasons unknown, -Wl, rpath= ... does not appear to work.)
#
# This targets Linux, but may well work on MacOS too; Windows probably requires
# more hoop-jumping; the maintainer would appreciate any feedback on this.

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

CC     = gcc
BINDIR = bin

MEX      = mex
MEXEXT   = $(shell mexext)
MEXDIR   = ../matlab/mex

REPDEP = sed -i -e '1s,\($*\)\.o[ :]*,\1.o \.$*.d: ,' \.$*.d

CSRC = mt64.c LZ76c.c LZ76c_matlab.c  LZ78c.c LZ78c_matlab.c
COBJ = $(patsubst %.c,.%.$(OBJEXT),$(CSRC))
CDEP = $(patsubst %.o,%.d,$(COBJ))

TSRC = LZ76c_benchmark.c LZ76c_test.c LZ78c_benchmark.c LZ78c_test.c
TOBJ = $(patsubst %.c,.%.$(OBJEXT),$(TSRC))
TDEP = $(patsubst %.o,%.d,$(TOBJ))
TBIN = LZ76c_benchmark$(BINEXT) LZ76c_test$(BINEXT) LZ78c_benchmark$(BINEXT) LZ78c_test$(BINEXT)
TBIP = $(patsubst %,$(BINDIR)/%,$(TBIN))

NSRC = LZ76c_randmv.c LZ78c_randmv.c
NBIN = LZ76c_randmv LZ78c_randmv
NBIP = $(patsubst %,$(BINDIR)/%,$(NBIN))

MSRC = LZ76c_mex.c LZ76c_x_mex.c Z78/LZ78c_mex.c LZ78c_x_mex.c
MBIN = LZ76c_mex.$(MEXEXT) LZ76c_x_mex.$(MEXEXT) LZ78c_mex.$(MEXEXT) LZ78c_x_mex.$(MEXEXT)
MBIP = $(patsubst %,$(MEXDIR)/%,$(MBIN))

MFLAGS = -std=c99 -march=native
WFLAGS = -Wall -Wextra -Wconversion -pedantic-errors
DFLAGS = -D_POSIX_C_SOURCE=199309L -D_DEFAULT_SOURCE

ifeq ($(OS),WIN)
	OFLAGS = -O3
	MEXCFLAGS = COMPFLAGS
	DFLAGS := $(DFLAGS) -DWIN
	RM = del /F /Q
	RTBIP = $(subst /,\,$(TBIP))
	RNBIP = $(subst /,\,$(NBIP))
	RMBIP = $(subst /,\,$(MBIP))
	LDFLAGS = $(OFLAGS)
else
	OFLAGS = -O3 -flto
	MEXCFLAGS = CFLAGS
	RM = rm -f
	RTBIP = $(TBIP)
	RNBIP = $(NBIP)
	RMBIP = $(MBIP)
	LDFLAGS = $(OFLAGS) -lm
endif

GCCFLAGS = $(MFLAGS) $(OFLAGS) $(WFLAGS) $(DFLAGS) $(IFLAGS)
MEXFLAGS = -O -R2018a

.PHONY: all clean diag

all: $(TBIP) $(MBIP)

test: $(TBIP)

mex: $(MBIP)

maint: $(NBIP)

clean:
	$(RM) $(COBJ) $(TOBJ) $(CDEP) $(TDEP) $(RTBIP) $(RNBIP) $(RMBIP)

$(COBJ): .%.o: %.c
	$(CC) -c -MMD -MP $(GCCFLAGS) -I$(MATLAB_PATH)/extern/include $< -o $@
	@$(REPDEP)

$(TOBJ): $(COBJ)
$(TOBJ): .%.o: %.c
	$(CC) -c -MMD -MP $(GCCFLAGS) $< -o $@
	@$(REPDEP)

$(TBIP): bin/%: .%.o
	$(CC) $(COBJ) $< $(LDFLAGS) -o $@

$(NBIP): $(COBJ)
$(NBIP): $(BINDIR)/%: %.c
	$(MEX) -client engine $(MEXFLAGS) $(MEXCFLAGS)="\$$CFLAGS $(GCCFLAGS)" -outdir $(BINDIR) $(COBJ) $<

$(MEXDIR)/%.$(MEXEXT): %.c
	$(MEX) $(MEXFLAGS) $(MEXCFLAGS)="\$$CFLAGS $(GCCFLAGS) -static" $(LIBS) -outdir $(MEXDIR) $(COBJ) $<

-include $(CDEP)
-include $(TDEP)

diag:
	@echo
	@echo "*** OS        = " $(OS) [$(ComSpec)]
	@echo
	@echo "*** CSRC      = " $(CSRC)
	@echo "*** COBJ      = " $(COBJ)
	@echo "*** CDEP      = " $(CDEP)
	@echo
	@echo "*** TSRC      = " $(TSRC)
	@echo "*** TOBJ      = " $(TOBJ)
	@echo "*** TDEP      = " $(TDEP)
	@echo "*** TBIN      = " $(TBIN)
	@echo "*** TBIP      = " $(TBIP)
	@echo
	@echo "*** NSRC      = " $(NSRC)
	@echo "*** NBIN      = " $(NBIN)
	@echo "*** NBIP      = " $(NBIP)
	@echo
	@echo "*** MSRC      = " $(MSRC)
	@echo "*** MBIN      = " $(MBIN)
	@echo "*** MBIP      = " $(MBIP)
	@echo
	@echo "*** CC        = " $(CC)
	@echo "*** GCCFLAGS  = " $(GCCFLAGS)
	@echo "*** LDFLAGS   = " $(LDFLAGS)
	@echo
	@echo "*** MEXFLAGS  = " $(MEXFLAGS)
	@echo "*** MEXCFLAGS = " $(MEXCFLAGS)
	@echo "*** MEXEXT    = " $(MEXEXT)
	@echo "*** MEXDIR    = " $(MEXDIR)
	@echo "*** MEX       = " $(MEX) $(MEXFLAGS) $(MEXCFLAGS)="\$$CFLAGS $(GCCFLAGS) -static" $(LIBS) -outdir $(MEXDIR)
	@echo
