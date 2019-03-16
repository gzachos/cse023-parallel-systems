#!/usr/bin/env gnuplot --persist

set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 400, 300
set output './plots/matmul.png'
set bar 1.000000 front
set style line 1 lc rgb '#dd181f' lt 1 lw 1 pt 5 pi -1 ps 1
set style fill transparent solid 0.5 noborder
set title "Integer Matrix multiplication\n(Dimensions: 1024x1024)"
set xlabel "MPI Nodes\n(x4 MPI processes)"
set ylabel "Time (sec)"
set y2tics
set grid
set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front bdefault
set style line 1 lc rgb '#dd181f' lt 1 lw 1.5 pt 7 pi -1 ps 1.4
plot './data/final.dat' using 1:($3+$6) title 'Comm.' with filledcurves x1 lc rgb "#FF7F50", \
     './data/final.dat' using 1:3 title 'Calc.' with filledcurves x1 lc rgb "#0000CD", \
     './data/final.dat' using 1:2 title 'Total' with linespoints ls 1
