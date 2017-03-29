#!/usr/bin/env bash

CHUNK_SIZE="1 10 100 1000 10000 100000"

for chnk in ${CHUNK_SIZE}
do
	INPUTFILE="pi_${chnk}.dat"
	OUTFILE="pi_c${chnk}.png"
	gnuplot -c pi.plt ./plots/${OUTFILE} ${chnk} ./data/${INPUTFILE}
done

