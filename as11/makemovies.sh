#!/bin/bash
make;
./as11-scolaro-s4320824 > data_out.dat;
cd plots/;
for i in dat*.dat;
	do cp $i data;
	gnuplot "../plot.gpi" > $i.gif;
	done
rm data
convert dat*.gif animation.gif
rm dat*.gif
cd ../;
