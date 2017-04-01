#!/usr/bin/env bash

TMPFILE=`mktemp barrier.XXXXXX`

echo -e "START\n"

x=0
: > ${TMPFILE}
while [ $x -lt 10 ]
do
	echo -e "\nRun #$x:"
	./test | awk '{print $7}' | bc -l | tee -a ${TMPFILE}
	((x += 1))
done

SUM=`cat ${TMPFILE} | paste -sd+ | bc -l`
echo -e "\nAverage: `echo ${SUM}/10 | bc -l | sed '/\./ s/\.\{0,1\}0\{1,\}$//'` sec."
echo -e "\n"

rm ${TMPFILE}

echo "END"

