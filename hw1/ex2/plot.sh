#!/usr/bin/env bash

INPUTFILE="matmul.dat"
OUTFILE="matmul.png"

gnuplot -c matmul.plt ./plots/${OUTFILE} ./data/${INPUTFILE}

