#!/usr/bin/env bash

NODE_NUM="2 4 8"
EXEFILE="pi_mpi.out"

if [ ! -d "./data/" ]
then
	mkdir data
fi

echo -e "START\n"

mpicc pi_mpi.c --output ${EXEFILE}

for node_num in ${NODE_NUM}
do
	x=0
	while [ $x -lt 4 ]
	do
		echo "Pi calculation using ${node_num} nodes (Run: #${x}):"
		OUTFILE="./data/pi_n${node_num}_${x}.dat"
		: > ${OUTFILE}
		mpirun --hostfile ../nodes-${node_num}.txt -npernode 4 ${EXEFILE} | tee -a ${OUTFILE}
		# Verify results
		PI=`cat ${OUTFILE} | grep pi | awk '{print $3}'`
		if [ "${PI}" != "3.1415926536" ]
		then
			echo -e "Result: FAILED!\n"
		else
			echo -e "Result: Successful\n"
		fi
		((x += 1))
	done
	echo -e "\n"
done
rm ${EXEFILE}

echo "END"

