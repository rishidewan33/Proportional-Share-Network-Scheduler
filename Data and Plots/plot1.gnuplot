set term postscript
set output "plot1.ps"
set yrange [0:]
plot "data1" using 1:2 with lines, "data1" using 1:3 with lines, "data1" using 1:4 with lines, "data1" using 1:5 with lines, "data1" using 1:6 with lines, "data1" using 1:7 with lines



