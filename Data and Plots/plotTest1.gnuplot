set term postscript
set output "plotTest1.ps"
set yrange [0:]
plot "dataTest1" using 1:2 with lines, "dataTest1" using 1:3 with lines