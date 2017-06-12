#!/usr/bin/env bash

OUTFILE="./data/final.dat"

function calc() {
	SUM=`paste  ./data/matmul_n${1}* | grep "${2}" | awk '{print $3" "$6" "$9" "$12}' | tr " " "+" | bc -l`
	RES=`echo ${SUM}/4 | bc -l | sed '/\./ s/\.\{0,1\}0\{1,\}$//'`
}

function stats() {
	OUT="${1} "
	calc ${1} 'Total' 
	OUT="$OUT $RES"
	calc ${1} 'Avg Calc'
	OUT="$OUT $RES"
	calc ${1} 'Min Calc'
	OUT="$OUT $RES"
	calc ${1} 'Max Calc'
	OUT="$OUT $RES"
	calc ${1} 'Avg Comm'
	OUT="$OUT $RES"
	calc ${1} 'Min Comm'
	OUT="$OUT $RES"
	calc ${1} 'Max Comm'
	OUT="$OUT $RES"
}

if [ ! -d "./plots/" ]
then
        mkdir plots
fi

stats 2 
echo `echo $OUT | tr "\n" " "` > ${OUTFILE}
stats 4
echo `echo $OUT | tr "\n" " "` >> ${OUTFILE}
stats 8
echo `echo $OUT | tr "\n" " "` >> ${OUTFILE}

gnuplot matmul.plt

