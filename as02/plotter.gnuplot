set term pdf
set key below
set output "as02-scolaro-s4320824-plot.pdf"
set datafile separator ","
set title "AS02 Scolaro s4320824"

plot 'data-4' using 1:2 title "n = 4", 'data-5' using 1:2 title "n = 5", \
	'data-6' using 1:2 title "n = 6"
quit
