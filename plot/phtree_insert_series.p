set autoscale
unset log
unset label
set xrange[0:*]
set xtic auto
set y2tic auto
set ytic auto
set ylabel "operation time [CPU cycles]"
set y2label "node count"
set xlabel "inserted entry number"
set key left top

set terminal qt size 1300,1000

set title "PH-Tree insert series" 
plot \
  "plot/data/phtree_insert_series.dat" using 1:2 title 'insert time' with linespoints, \
  "plot/data/phtree_insert_series.dat" using 1:3 title 'lookup time' with linespoints, \
  "plot/data/phtree_insert_series.dat" using 1:4 title '#AHC nodes' with linespoints axes x1y2, \
  "plot/data/phtree_insert_series.dat" using 1:5 title '#LHC nodes' with linespoints axes x1y2
unset output

