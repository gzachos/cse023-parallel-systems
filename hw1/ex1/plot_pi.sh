#!/usr/bin/env bash

NUM_THREADS="1 4 16"
CHUNK_SIZE="1 10 100 1000 10000 100000"

echo -e "START\n"

for chnk in ${CHUNK_SIZE}
do
	OUTFILE="pi_${chnk}.dat"
	gnuplot -c pi.plt ./plots/pi_c${chnk}.png ${chnk} ./data/${OUTFILE}
done

echo -e "END\n"
