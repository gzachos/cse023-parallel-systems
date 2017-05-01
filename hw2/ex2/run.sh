#!/usr/bin/env bash

SCHEDULES="static dynamic guided"
CHUNK_SIZE="0 10 100 1000 10000 100000"
TMPFILE=`mktemp primes.XXXXXX`
EXEFILE='primes.out'
OUTFILE="./data/primes.dat"

gcc -fopenmp primes.c --output ${EXEFILE}

if [ ! -d "./data/" ]
then
        mkdir data
fi

echo -e "START\n"

export OMP_DISPLAY_ENV="false"
export OMP_DYNAMIC="false"

: > ${OUTFILE}
for chunk in ${CHUNK_SIZE}
do
	OUT="${chunk}"
	for schedule in ${SCHEDULES}
	do
		x=0
		: > ${TMPFILE}
		SCHED_POLICY="${schedule},${chunk}"

		export OMP_SCHEDULE="${SCHED_POLICY}"

		while [ $x -lt 4 ]
		do
			echo "Primes calculation using \"${SCHED_POLICY}\" schedule (Run: #${x}):"
			./${EXEFILE} | tee -a ${TMPFILE}
			((x += 1))
			RES=`cat ${TMPFILE} | tail -1 | awk '{print $4 $7}'`
			if [ "${RES}" != "664579,9999991" ]
			then
				echo -e "Result: FAILED!\n"
			else
				echo -e "Result: Successful\n"
			fi
		done
		SUM=`cat ${TMPFILE} | awk '{print $11}' | paste -sd+ | bc -l`
		OUT="${OUT}\t`echo ${SUM}/4 | bc -l | sed '/\./ s/\.\{0,1\}0\{1,\}$//'`"
		echo -e "\n"
	done
	echo -e "${OUT}" >> ${OUTFILE}
done

rm ${TMPFILE} ${EXEFILE}

echo "END"

