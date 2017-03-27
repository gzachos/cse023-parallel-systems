#!/usr/bin/env bash

NUM_THREADS="1 4 16"
CHUNK_SIZE="1 10 100 1000 10000 100000"
TMPFILE=`mktemp pi.XXXXXX`

gcc -pthread pi_parallel.c --output pi

for nthr in ${NUM_THREADS}
do
	for chnk in ${CHUNK_SIZE}
	do
		x=0
		OUTFILE="pi_${nthr}_${chnk}.dat"
		: > ${OUTFILE}
		: > ${TMPFILE}
		while [ $x -lt 4 ]
		do
			./pi ${nthr} ${chnk} | tee -a ${TMPFILE}
			((x += 1))
		done
		PI=`cat ${TMPFILE} | head -1 | awk '{print $9}'`
		if [ "${PI}" != "3.1415926536" ]
		then
			echo "Pi calculation failed!"
			echo "Script will now exit!"
			exit 1
		fi
		SUM=`cat ${TMPFILE} | awk '{print $11}' | paste -sd+ | bc -l`
		echo -e "${nthr}\t${chnk}\t`echo ${SUM}/4 | bc -l | sed '/\./ s/\.\{0,1\}0\{1,\}$//'`" | tee -a ${OUTFILE}
		echo -e "\n"
	done
	gnuplot -c pi.plt pi_${nthr}_${chnk}.png ${nthr} ${OUTFILE}
done
