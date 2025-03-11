#!/bin/bash

# base name of this script (without extension)
scriptname=$(basename $0 .sh)

echo -e "\n*** Running batch script '"$scriptname"' ***\n"

# path to Matlab code directory
codedir=~/git/fLZc/matlab

# current directory (the directory this script is run in - log files will go here)
currdir=$(pwd -P)

# Parameters

n=100000
N=100000

# run multiple concurrent Matlab sessions
for d in $(seq 9 10); do

	# the log file
	logfile=$currdir/$scriptname\_a$(printf "%02d" $d).log

	# Matlab commands
	matcmds="clear; LZc_gen_rand($n,$d,$N); quit"

	# run Matlab
	cd $codedir && nohup nice matlab -nojvm -nodisplay -r "$matcmds" > $logfile < /dev/null 2>&1 &

done
