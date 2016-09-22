set term pdf
set key below
set output "as07-scolaro-s4320824-plot2.pdf"
set datafile separator ","
set style data line
set title "AS07 SCOLARO s4320824"
set xlabel "Time (s)"
set ylabel "Energy (J)"

plot 'as07-scolaro-s4320824-t1.dat' using 1:8 title 'T = 1', \
'as07-scolaro-s4320824-t2.dat' using 1:8 title 'T = 2';

quit
