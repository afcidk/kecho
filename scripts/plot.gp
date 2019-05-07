set terminal png

set xlabel "Concurrent threads"
set ylabel "time (sec)"
set output "output/avg.png"
plot "output/Average" title "Average"

set xlabel "Concurrent threads"
set ylabel "time (sec)"
set output "output/worst.png"
plot "output/Worst" title "Worst time"
