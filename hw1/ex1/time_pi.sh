#!/usr/bin/env bash

NUM_THREADS="1 4 16"
CHUNK_SIZE="1 10 100 1000 10000 100000"

gcc -pthread pi_parallel.c --output pi

for nthr in ${NUM_THREADS}
do
	for chnk in ${CHUNK_SIZE}
	do
		x=0
		while [ $x -lt 4 ]
		do
			./pi ${nthr} ${chnk}
			((x += 1))
		done
		echo -e "\n"
	done
done
