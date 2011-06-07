set term postscript
set output "plotTest5.ps"
set yrange [0:]
plot "dataTest5" using 1:2 with lines, "dataTest5" using 1:3 with lines, "dataTest5" using 1:4 with lines