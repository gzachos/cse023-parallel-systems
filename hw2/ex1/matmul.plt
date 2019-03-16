#!/usr/bin/env gnuplot --persist

set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 400, 300
set output './plots/matmul.png'
set bar 1.000000 front
set logscale y 10
set logscale y2 10
set style line 1 lc rgb '#dd181f' lt 1 lw 1 pt 5 pi -1 ps 1
set style line 6 lc rgb '#0000ff' lt 1 lw 1 pt 5 pi -1 ps 1
set key inside top left reverse vertical Left noenhanced box linetype -1 linewidth 1.000
#set title "Integer Matrix multiplication (Dimensions: 1024x1024)\n"
set xlabel "Parallelized Loop"
set ylabel "Time (sec)"
set y2tics
set xtics 1
set grid
set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front bdefault
set style line 1 lc rgb '#dd181f' lt 1 lw 1.5 pt 7 pi -1 ps 1.4
set style line 2 lc rgb '#0000ff' lt 1 lw 1.5 pt 7 pi -1 ps 1.4
plot './data/matmul.dat' using 2 title 'Static Scheduling' with linespoints ls 1, \
     './data/matmul.dat' using 3 title 'Dynamic Scheduling' with linespoints ls 2
