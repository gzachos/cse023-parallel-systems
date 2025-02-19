#!/usr/bin/env gnuplot --persist

# ARG1: Output file
# ARG2: Input file

set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 400, 300
set output ARG1
set bar 1.000000 front
set logscale x 2
set style line 1 lc rgb '#dd181f' lt 1 lw 1 pt 5 pi -1 ps 1
set title "Integer Matrix multiplication using static scheduling\n(Dimensions: 1024x1024)"
set xlabel "Threads"
set ylabel "Time (sec)"
set y2tics
set grid
set yrange [0:]
set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front bdefault
set style line 1 lc rgb '#dd181f' lt 1 lw 1.5 pt 7 pi -1 ps 1.4
plot ARG2 using 1:2 notitle with linespoints ls 1
