/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: s4320824
 * Assignment 6
 * 
 * This program simulates a specified amount of stochastic simulations, and
 * calculates the averages of a bunch of variables to compare them to the
 * analytical solution.
 *
 * Made with gcc flags: -std=c99 -lm
 */

//Includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//Function Pre-declarations
double gauss_rand(double);

//Main
int main(int argc, char *argv[]) {
	double initial_x, final_t, gamma, error_strength, p, x, delta_t;
	double x_average, x_2_average, y_average, y_2_average = 0;
	int num_t, ntrej;

	if (argc != 8) {
		printf("Usage: [initial_position] [final_time] [num_timesteps] [damping strength] [error strength] [p] [number of trajectories]\n");
		exit(-1);
	}
	
	//Setting arguements from input variables
	initial_x = atof(argv[1]);
	final_t = atof(argv[2]);
	num_t = atoi(argv[3]);
	gamma = atof(argv[4]);
	error_strength = atof(argv[5]);
	p = atof(argv[6]);
	ntrej = atoi(argv[7]);
	delta_t = final_t / (double) num_t;

	//Loops through all trajectories
	for (int j = 0; j < ntrej; j++) {
		x = initial_x;

		//Loops through all the points in one trajectory
		for (double i = 0; i <= final_t; i += delta_t) {
			x += (((-x * gamma) + (error_strength * gauss_rand(pow(delta_t, p)))) * delta_t);
		}

		//Sums all the final values of each trajectory
		x_average += x;
		x_2_average += pow(x, 2.0);
		y_average += x * exp(gamma * final_t);
		y_2_average += pow(x * exp(gamma * final_t), 2.0);
	}

	//Calculates the averages from the sums of each trajectory
	x_average = x_average / (double) ntrej;
	x_2_average = x_2_average / (double) ntrej;
	y_average = y_average / (double) ntrej;
	y_2_average = y_2_average / (double) ntrej;

	//Prints final results
	printf("  <x>       <x^2>      V(x)        <y>          <y^2>         V(y)      Analytical Solution\n");
	printf("%5.5lf   %5.5lf   %5.5lf   %5.5le   %5.5le   %5.5le  %5.5le\n",
			x_average, x_2_average, x_2_average - pow(x_average, 2.0), y_average, y_2_average,
			y_2_average - pow(y_average, 2.0), (pow(error_strength, 2.0) / gamma) * (exp(2.0 * gamma * final_t) - 1.0));

	return 0;
}

//A function that gives gaussian distributed random variables.
//Also note that I don't seed rand(), so I have a seed of 1.
double gauss_rand(double width_input) {
	double r1, r2, s, r, g_rand;

	while (1) {
		r1 = (double) rand() / ((double) RAND_MAX + 1.0);
		r2 = (double) rand() / ((double) RAND_MAX + 1.0);

		r1 = (r1 * 2.0) - 1.0;
		r2 = (r2 * 2.0) - 1.0;

		r = pow(r1, 2.0) + pow(r2, 2.0);
		if (r < 1.0) {
			s = sqrt(-2.0 * log(r)) / r;
			break;
		}
	}

	g_rand = sqrt(width_input) * r1 * s;
	return g_rand;
}
