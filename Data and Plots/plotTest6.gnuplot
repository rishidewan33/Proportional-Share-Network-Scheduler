set term postscript
set output "plotTest6.ps"
set yrange [0:]
plot "dataTest6" using 1:2 with lines, "dataTest6" using 1:3 with lines, "dataTest6" using 1:4 with lines, "dataTest6" using 1:5 with lines