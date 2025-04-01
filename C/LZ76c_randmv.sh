#!/bin/bash

# base name of this script (without extension)
scriptname=$(basename $0 .sh)

echo -e "\n*** Running batch script '"$scriptname"' ***\n"

# path to LZ76c_randmv executable
prog=~/git/fLZc/C/bin/LZ76c_randmv

# current directory (the directory this script is run in - log files will go here)
currdir=$(pwd -P)

# Matlab dynamic library nonsense
export LD_LIBRARY_PATH=$MATLAB_PATH/bin/glnxa64

# Parameters

n=10000
N=10000
s=0
D=/tmp

# run multiple concurrent Matlab sessions
for a in $(seq 2 10); do

	# the log file
	logfile=$currdir/$scriptname\_a$(printf "%02d" $a).log

	# run the program
	nohup nice $prog $n $a $N $s $D > $logfile < /dev/null 2>&1 &

done
