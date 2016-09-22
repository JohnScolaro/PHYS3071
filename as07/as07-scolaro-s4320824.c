/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: s4320824
 * Assignment 7
 * 
 * This program simulates particles confined to a 1-d box, moving due to
 * brownian motion. It shows that the average of a large amount of these
 * particles, over their indivudual trajectories, is equal to the macroscopic
 * result that we observe.
 *
 * Input: initial velocity in m/s.
 *
 * Made with gcc flags: -std=c99 -lm
 */

//Includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//Defines

//Boltzman's Constant
#define k 1.0
//Mass
#define m 1.0
//Radius of a particle
#define a 1.0
//PI
#define pi 3.1415926535

//Function Pre-declarations
double gauss_rand(double);

int main(int argc, char *argv[]) {
	//These are the declarations of all variables used.
	int num_points, ntrej;
	double radius, gamma, n, mass, final_time, delta_t, temp, velocity_initial;
	double x_current, velocity_current, acceleration_current, g, box_size;

	if (argc != 2) {
		printf("Please input an initial velocity for the particles.\n");
		exit(-1);
	}
	velocity_initial = atof(argv[1]);

	//Setting all constants to be used in calculations. Just change and recompile
	//if you want to re-run with different values.
	radius = 1.0;
	n = 1.0;
	mass = 1.0;
	ntrej = 500;
	final_time = 2.0;
	num_points = 1000;
	temp = 2.0;
	box_size = 1.0;
	delta_t = final_time / (double) num_points;
	g = sqrt(12.0 * pi * n * radius * k * temp);
	gamma = 6.0 * pi * radius * n / mass;

	//Checking our initial velocity, and making sure it isn't too fast.
	if ((velocity_initial * delta_t) > (box_size / 2.0)) {
		printf("Your initial velocity is too fast!\n");
		exit(-2);
	}

	//Initialising our 2D arrays. Unfortunately due to this implimentation, the program uses
	//up all the availible stack if we try to take the average of lots of points, so don't
	//make the sizes num_points, or ntrej too large.
	double x_storage[ntrej][num_points + 1];
	double v_storage[ntrej][num_points + 1];

	//Loops through all trajectories
	for (int j = 0; j < ntrej; j++) {
		x_current = 0;
		velocity_current = velocity_initial;
		acceleration_current = 0;
		v_storage[j][0] = velocity_current;
		x_storage[j][0] = x_current;

		//Loops through all the points in one trajectory and populates our arrays
		for (int i = 0; i < num_points; i++) {
			x_current += velocity_current * delta_t;
			acceleration_current = -(gamma * velocity_current) + (g * gauss_rand(pow(delta_t, 0.5)) / mass);
			velocity_current += acceleration_current * delta_t;
			//Checking to see if the particle has moved outside of the box
			if (x_current > (box_size / 2)) {
				x_current = (box_size / 2) - (x_current - (box_size / 2));
				velocity_current = -velocity_current;
			}
			if (x_current < (-box_size / 2)) {
				x_current = -(box_size / 2) + fabs(x_current) - (box_size / 2);
				velocity_current = -velocity_current;
			}
			x_storage[j][i + 1] = x_current;
			v_storage[j][i + 1] = velocity_current;
		}
	}

	//Calculation of averages, and printing to a file.
	double time, x_av, x_2_av, v_av, v_2_av;
	FILE *fp;
	fp = fopen("thermalisation2.dat", "w");

	time = 0;

	//So this time we loop through the points in all the arrays, and calculate the averages.
	for (int i = 0; i <= num_points; i++) {
		x_av = 0;
		x_2_av = 0;
		v_av = 0;
		v_2_av = 0;

		//Loop through all the trajectories at a constant timestep.
		for (int j = 0; j < ntrej; j++) {
			x_av += x_storage[j][i];
			x_2_av += pow(x_storage[j][i], 2.0);
			v_av += v_storage[j][i];
			v_2_av += pow(v_storage[j][i], 2.0);
		}

		//Calculate and print the averages.
		x_av = x_av / (double) ntrej;
		x_2_av = x_2_av / (double) ntrej;
		v_av = v_av / (double) ntrej;
		v_2_av = v_2_av / (double) ntrej;
		fprintf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", time, x_av, x_2_av, x_2_av - pow(x_av, 2), v_av, v_2_av, v_2_av - pow(v_av, 2), mass / 2 * v_2_av);
		time += delta_t;
	}
	
	fclose(fp);
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
			s = sqrt(-2.0 * log(r) / r);
			break;
		}
	}

	g_rand = sqrt(width_input) * r1 * s;
	return g_rand;
}
