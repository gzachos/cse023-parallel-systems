#!/usr/bin/env bash

NUM_THREADS="1 2 4 8 32 128 512 1024"
TMPFILE=`mktemp barrier.XXXXXX`

make test

echo -e "START\n"

for nthr in ${NUM_THREADS}
do
	x=0
	: > ${TMPFILE}
	while [ $x -lt 4 ]
	do
		echo -e "Barrier timing using ${nthr} thread(s) (Run: #${x})"
		./test ${nthr} | awk '{print $7}' | bc -l >> ${TMPFILE}
		((x += 1))
	done

	SUM=`cat ${TMPFILE} | paste -sd+ | bc -l`
	echo -e "\nAverage: `echo ${SUM}/4 | bc -l | sed '/\./ s/\.\{0,1\}0\{1,\}$//'` msec."
	echo -e "\n"
done

rm ${TMPFILE}

echo "END"

