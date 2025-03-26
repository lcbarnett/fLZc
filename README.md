# fLZc
 [Lempel-Ziv complexity](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv_complexity) (LZc) is a measure associated with sequences of symbols, and is related (but _not_ identical) to [Kolmogorov complexity](https://en.wikipedia.org/wiki/Kolmogorov_complexity). Note that it does not necessarily measure "complexity" in the colloquial sense; rather, it measures roughly how simple a sequence is to describe, or how "compressible" a sequence is (indeed, Lempel-Ziv algorithms are widely used for compressing data). The more "random" a sequence, the higher it tends to score.

The __fLZc (fast Lempel-Ziv complexity) library__ is written in C, with a [Matlab](https://uk.mathworks.com/) interface. LZc comes in many flavours; __fLZc__ computes the canonical LZ76c [^1] and LZ78c [^2] versions, with optional normalisation.

### Building
You will need the [Make](https://www.gnu.org/software/make/) build tool installed on your system, a suitable C99 compiler (e.g., [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/)), and the Matlab `mex` and `makemex` executables on your system execution path. You will also need to set up an environmental variable `MATLAB_PATH` which contains the root path of your Matlab installation. [^3]

To build the required binary code, in a terminal navigate to the `<fLZc_root>/C` directory and run
```
make clean && make
```
This builds some C standalone executables (for testing and benchmarking) in the `<fLZc_root>/C/bin` directory, and the Matlab .mex C interface binaries in the `<fLZc_root>/matlab/mex` directory.

### Binary normalisation data
For availability of LZc normalisation, download the file [fLZc_data.zip](https://users.sussex.ac.uk/~lionelb/downloads/fLZc_data.zip), and unzip into the  `<fLZc_root>/matlab/data` directory.

### Running the Matlab demonstration script
Start Matlab in the `<fLZc_root>/matlab` directory (or make sure at least that `<fLZc_root>/matlab/startup.m` is run), and run
```
clear; LZc_demo
```
### Implementation
LZ76c is implemented as a slightly amended version of the algorithm given in [^4]. LZ78c is implemented using the excellent hash-map [khashl](https://github.com/attractivechaos/khashl) as a dictionary. Alphabet size is arbitrary, although normalisation (by mean LZc of random sequences) is only currently available up to alphabets of <= 10 symbols. There is also a Matlab utlity for "quantisation" (discretisation by quantiles) of continuous-valued data; in particular, by default binarisation is around the median rather than the mean (this arguably makes more sense anyway). Versions of both LZ76c and LZ78c are available that optionally return the actual dictionary, and/or "running" LZc values along the sequence length of the supplied symbol sequence.

For long sequences, LZ78c is considerably faster; in the worst-case scenario it scales in approximately linear time with sequence length, although the memory overhead may be high for very long sequences. LZ76c scales in approximately quadratic time in the worst-case scenario, but has a negligible memory footprint.

LZ76c is arguably somewhat closer to complexity in the Kolmogorov sense; LZ78c, by contrast, may assign surprisingly large complexity values to sequences containing long subsequences of repeated symbols.

### Developer
[Lionel Barnett](https://users.sussex.ac.uk/~lionelb/), Department of Informatics, University of Sussex, UK.

email: lionelb@sussex.ac.uk

[^1]: [A. Lempel and J. Ziv, On the Complexity of Finite Sequences, IEEE Trans. Info Theor. 22(1), 1976](https://ieeexplore.ieee.org/document/1055501).
[^2]: [A. Lempel and J. Ziv, Compression of Individual Sequences via Variable-Rate Coding, IEEE Trans. Info Theor. 24(5), 1978](https://ieeexplore.ieee.org/document/1055934).
[^3]: Building targets POSIX environments, and has been tested on Linux and MacOS. Not so sure about Windows (note that [Visual Studio for Windows](https://visualstudio.microsoft.com/vs/features/cplusplus/) does *not* support C99; you are probably better off with [MinGW](https://sourceforge.net/projects/mingw/)).
[^4]: [F. Kaspar and H. G. Schuster, Easily calculable measure for the complexity of spatiotemporal patterns, Phys. Rev. A 36(2), 1987](1987)

