#!/usr/bin/env gnuplot --persist

set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 400, 300
set output './plots/barrier.png'
set bar 1.000000 front
set logscale x 4
set logscale y 10
set logscale y2 10
set style line 1 lc rgb '#dd181f' lt 1 lw 1 pt 5 pi -1 ps 1
set style line 6 lc rgb '#0000ff' lt 1 lw 1 pt 5 pi -1 ps 1
set key inside right bottom reverse vertical Left noenhanced autotitles columnhead box linetype -1 linewidth 1.000
# set title "Comparative graph of program execution time\n"
set xlabel "Threads"
set ylabel "Time (msec)"
set y2tics
set grid
set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front bdefault
plot './data/barrier.dat' using 1:2 title 'Custom Barrier' with linespoints ls 1, \
     './data/barrier.dat' using 1:3 title 'Pthread Barrier' with linespoints ls 6
