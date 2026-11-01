set datafile separator ","

set terminal pngcairo size 1000,600
set output "fifo_vs_edf.png"

set title "FIFO vs EDF: On-time completion"
set xlabel "Tick"
set ylabel "Completed on time (%)"

set yrange [0:100]
set grid
set key top right

plot "fifo.csv" using 1:3 with lines linewidth 2 title "FIFO", \
     "edf.csv" using 1:3 with lines linewidth 2 title "EDF"

set output


set output "fifo_vs_edf_waiting.png"

set title "FIFO vs EDF: Waiting orders"
set xlabel "Tick"
set ylabel "Number of waiting orders"

set autoscale y
set grid
set key top left

plot "fifo.csv" using 1:2 with lines linewidth 2 title "FIFO", \
     "edf.csv" using 1:2 with lines linewidth 2 title "EDF"

set output


set output "fifo_vs_edf_resources.png"

set title "FIFO vs EDF: Cook utilization"
set xlabel "Tick"
set ylabel "Cook utilization (%)"

set yrange [0:100]
set grid
set key top right

plot "fifo.csv" using 1:4 with lines linewidth 2 title "FIFO", \
     "edf.csv" using 1:4 with lines linewidth 2 title "EDF"

set output