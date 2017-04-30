#!/usr/bin/env bash

NUM_TASKS="16 256 1024"
TMPFILE=`mktemp matmul.XXXXXX`
EXEFILE='matmul_parallel.out'

gcc -fopenmp matmul_parallel.c --output ${EXEFILE} -lm

if [ ! -d "./data/" ]
then
        mkdir data
fi

echo -e "START\n"

export OMP_DISPLAY_ENV="false"
export OMP_DYNAMIC="false"

OUTFILE="./data/matmul.dat"
: > ${OUTFILE}
for ntasks in ${NUM_TASKS}
do
	x=0
	: > ${TMPFILE}
	while [ $x -lt 4 ]
	do
		echo "Matrix Multiplication using ${ntasks} OpenMP tasks (Run: #${x}):"
		./${EXEFILE} ${ntasks} | tee -a ${TMPFILE}
		# Verify results
		if ! diff -q './Cmat1024' '../../matrices/Cmat1024'
		then
			echo -e "Result: FAILED!\n"
		else
			echo -e "Result: Successful\n"
		fi
		((x += 1))
	done
	SUM=`cat ${TMPFILE} | awk '{print $2}' | paste -sd+ | bc -l`
	echo -e "${ntasks}\t`echo ${SUM}/4 | bc -l | sed '/\./ s/\.\{0,1\}0\{1,\}$//'`" >> ${OUTFILE}
	echo -e "\n"
done

rm ${TMPFILE} ${EXEFILE} './Cmat1024'

echo "END"

