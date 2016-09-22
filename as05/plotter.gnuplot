set term pdf
set key below
set output "as05-scolaro-s4320824-plot.pdf"
set datafile separator ","
set style data line
set title "AS05 SCOLARO s4320824"
set xlabel "Radius (km)"
set ylabel "Mass (Solar Masses)"

plot 'massvsradius.dat' using 2:3 title 'Newtons Method', \
'massvsradius.dat' using 4:5 title 'TOV Method'

quit
