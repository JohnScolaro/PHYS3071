set term pdf
set key below
set output "as08-scolaro-s4320824-positiontime.pdf"
set datafile separator ","
set style data line
set title "AS08 SCOLARO s4320824"
set xlabel "Time (s)"
set ylabel "Position (m)"

plot 'as08-scolaro-s4320824-datatime1.dat' using 1:2 title 'Test 1', \
'as08-scolaro-s4320824-datatime2.dat' using 1:2 title 'Test 2';

quit
