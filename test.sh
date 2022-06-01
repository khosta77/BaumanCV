#!/bin/bash

make
./main

gnuplot -persist <<-EOFMarker#
    set xlabel "picture №"
    set ylabel "r"
    set title "Graph correlation"
    set grid
    plot "test_Pirson_data.txt" using 1:2 with lines title "coefficient Pearson"
EOFMarker#