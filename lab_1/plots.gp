set datafile separator ","

set terminal pngcairo size 1000,600
set output "waiting_orders.png"

set title "Waiting orders"
set xlabel "Tick"
set ylabel "Number of waiting orders"

set grid
set key off

plot "statistics.csv" using 1:2 with linespoints linewidth 2

set output "on_time_percentage.png"

set title "Orders completed on time"
set xlabel "Tick"
set ylabel "Completed on time (%)"

set yrange [0:100]
set grid

plot "statistics.csv" using 1:3 with linespoints linewidth 2

set output

set output "resource_utilization.png"

set title "Resource utilization"
set xlabel "Tick"
set ylabel "Utilization (%)"

set yrange [0:100]
set grid
set key top right

plot "statistics.csv" using 1:4 with lines linewidth 2 title "Cooks", \
     "statistics.csv" using 1:5 with lines linewidth 2 title "Workplaces"

set output