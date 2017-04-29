#!/usr/bin/env bash

SCHED_POLICIES="static dynamic"
OUTFILE="./data/matmul.dat"

if [ ! -d "./data/" ]
then
	mkdir data
fi

echo -e "START\n"

export OMP_DYNAMIC="false"

for loop in {0..2}
do
	TMPFILE=`mktemp matmul.XXXXXX`
	EXEFILE="matmul_parallel_l${loop}.out"

	gcc -fopenmp matmul_parallel_l${loop}.c --output ${EXEFILE}

	echo "Parallelization of loop #${loop}"
	echo -e "--------------------------\n"
	OUT="${OUT}${loop}"

	: > ${OUTFILE}
	for policy in ${SCHED_POLICIES}
	do
		x=0
		: > ${TMPFILE}
		while [ $x -lt 4 ]
		do
			echo "Matrix Multiplication using ${policy} scheduling (Run: #${x}):"
			./${EXEFILE} ${policy} | tee -a ${TMPFILE}
			# Verify results
			if ! diff -q './Cmat1024' '../../matrices/Cmat1024'
			then
				echo -e "Result: FAILED!\n"
			else
				echo -e "Result: Successful\n"
			fi
			((x += 1))
		done
		SUM=`cat ${TMPFILE} | awk '{print $6}' | paste -sd+ | bc -l`
		OUT="${OUT}\t\t`echo ${SUM}/4 | bc -l | sed '/\./ s/\.\{0,1\}0\{1,\}$//'`"
		echo -e "\n"
	done
	OUT="${OUT}\n"
	rm ${TMPFILE} ${EXEFILE} './Cmat1024'
done

echo -e ${OUT} > ${OUTFILE}

echo "END"

