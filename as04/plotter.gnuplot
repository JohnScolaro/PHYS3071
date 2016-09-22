set term pdf
set key below
set output "as04-scolaro-s4320824-plot.pdf"
set datafile separator ","
set style data line
set title "AS04 SCOLARO s4320824"
set xlabel "Time (s)"

plot 'data.txt' using 1:2 title 'Height (m)', \
'data.txt' using 1:3 title 'Rate (m/s)'
quit
