#!/usr/bin/env bash

INPUTFILE="primes.dat"
OUTFILE="primes.png"

if [ ! -d "./plots/" ]
then
        mkdir plots
fi

gnuplot primes.plt 

