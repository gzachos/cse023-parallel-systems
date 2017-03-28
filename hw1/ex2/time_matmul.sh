#!/usr/bin/env bash

NUM_THREADS="1 2 4 8 12 16"
TMPFILE=`mktemp matmul.XXXXXX`
EXEFILE='matmul_parallel.out'

gcc -pthread matmul_parallel.c --output ${EXEFILE}

echo -e "START\n"

OUTFILE="./data/matmul.dat"
: > ${OUTFILE}
for nthr in ${NUM_THREADS}
do
	x=0
	: > ${TMPFILE}
	while [ $x -lt 4 ]
	do
		echo "Matrix Multiplication using ${nthr} thread(s) (Run: #${x}):"
		./${EXEFILE} ${nthr} | tee -a ${TMPFILE}
		# Verify results
		if ! diff -q './Cmat1024' '../../matrices/Cmat1024'
		then
			echo -e "Result: FAILED!\n"
		else
			echo -e "Result: Successful\n"
		fi
		((x += 1))
	done
	SUM=`cat ${TMPFILE} | awk '{print $5}' | paste -sd+ | bc -l`
	echo -e "${nthr}\t`echo ${SUM}/4 | bc -l | sed '/\./ s/\.\{0,1\}0\{1,\}$//'`" >> ${OUTFILE}
	echo -e "\n"
done

rm ${TMPFILE} ${EXEFILE} './Cmat1024'

echo "END"

