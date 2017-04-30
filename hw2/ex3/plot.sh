#!/usr/bin/env bash

INPUTFILE="matmul.dat"
OUTFILE="matmul.png"

if [ ! -d "./plots/" ]
then
        mkdir plots
fi

gnuplot -c matmul.plt ./plots/${OUTFILE} ./data/${INPUTFILE}

