#!/usr/bin/env bash

NODE_NUM="2 4 8"
EXEFILE="matmul_distributed.out"

if [ ! -d "./data/" ]
then
	mkdir data
fi

echo -e "START\n"

mpicc matmul_distributed.c --output ${EXEFILE}

for node_num in ${NODE_NUM}
do
	x=0
	while [ $x -lt 4 ]
	do
		echo "Matrix Multiplication using ${node_num} nodes (Run: #${x}):"
		OUTFILE="./data/matmul_n${node_num}_${x}.dat"
		: > ${OUTFILE}
		mpirun --hostfile ../nodes-${node_num}.txt -npernode 4 ${EXEFILE} | tee -a ${OUTFILE}
		# Verify results
		if ! diff -q './Cmat1024' '../../matrices/Cmat1024'
		then
			echo -e "Result: FAILED!\n"
		else
			echo -e "Result: Successful\n"
		fi
		((x += 1))
	done
	echo -e "\n"
done
rm ${EXEFILE} './Cmat1024'

echo "END"

