#!/usr/bin/env bash

NUM_THREADS="1 4 16"
CHUNK_SIZE="1 10 100 1000 10000 100000"
TMPFILE=`mktemp pi.XXXXXX`
EXEFILE='pi_parallel.out'

gcc -pthread pi_parallel.c --output ${EXEFILE}

echo -e "START\n"

for chnk in ${CHUNK_SIZE}
do
	OUTFILE="./data/pi_${chnk}.dat"
	: > ${OUTFILE}
	for nthr in ${NUM_THREADS}
	do
		x=0
		: > ${TMPFILE}
		while [ $x -lt 4 ]
		do
			echo "Pi calculation using ${nthr} thread(s) (Run: #${x}):"
			./${EXEFILE} ${nthr} ${chnk} | tee -a ${TMPFILE}
			((x += 1))
			PI=`cat ${TMPFILE} | head -1 | awk '{print $9}'`
			if [ "${PI}" != "3.1415926536" ]
			then
				echo -e "Result: FAILED!\n"
			else
				echo -e "Result: Successful\n"
			fi
		done
		SUM=`cat ${TMPFILE} | awk '{print $11}' | paste -sd+ | bc -l`
		echo -e "${nthr}\t${chnk}\t`echo ${SUM}/4 | bc -l | sed '/\./ s/\.\{0,1\}0\{1,\}$//'`" >> ${OUTFILE}
		echo -e "\n"
	done
#	gnuplot pi.plt pi_${chnk}.png ${chnk} ${OUTFILE}   # For gnuplot 4.6.6 and earlier.
#	gnuplot -c pi.plt pi_${chnk}.png ${chnk} ${OUTFILE}
done

rm ${TMPFILE} ${EXEFILE}

echo "END"
