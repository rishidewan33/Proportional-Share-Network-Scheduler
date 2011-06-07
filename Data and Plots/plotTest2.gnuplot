set term postscript
set output "plotTest2.ps"
set yrange [0:]
plot "dataTest2" using 1:2 with lines, "dataTest2" using 1:3 with lines, "dataTest2" using 1:4 with lines