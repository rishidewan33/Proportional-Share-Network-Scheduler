set data style linespoints
set yrange [0:]
plot "tmp.dat" using 1:2, "tmp.dat" using 1:3
pause -1

set term postscript
set output "tmp.ps"
replot
