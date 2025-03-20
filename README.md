# fLZc
Fast Lempel-Ziv complexity library in C, with [Matlab](https://uk.mathworks.com/) interface: computes LZ76c [^1] and LZ78c [^2], with optional normalisation.

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
clear;
LZc_demo
```

[Lionel Barnett](https://users.sussex.ac.uk/~lionelb/), Department of Informatics, University of Sussex, UK.

email: lionelb@sussex.ac.uk

[^1]: [A. Lempel and J. Ziv, On the Complexity of Finite Sequences, IEEE Trans. Info Theor. 22(1), 1976](https://ieeexplore.ieee.org/document/1055501).
[^2]: [A. Lempel and J. Ziv, Compression of Individual Sequences via Variable-Rate Coding, IEEE Trans. Info Theor. 24(5), 1976](https://ieeexplore.ieee.org/document/1055934).
[^3]: Building targets POSIX environments, and has been tested on Linux and MacOS. Not so sure about Windows (note that [Visual Studio for Windows](https://visualstudio.microsoft.com/vs/features/cplusplus/) does *not* support C99; you are probably better off with [MinGW](https://sourceforge.net/projects/mingw/)).
