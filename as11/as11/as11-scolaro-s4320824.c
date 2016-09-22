/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: s4320824
 * Assignment 11
 * 
 * This program calculates the diffusion equation and evolves a diffusive
 * system through time. It can do so with an arbitrary potential to stop
 * the diffusion. This is pretty cool because we can make the potential
 * anything we want, and also start with this diffusive 'gas' in any
 * configuration we want, and see how the system evolves.
 *
 * This program outputs a series of snapshots of the system at constant points
 * in time so that an animation can be created to visualise the diffusion.
 *
 * Made with gcc flags: -std=c99 -lm
 */


//Includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//Defines
#define KB 1.38E-23


//Function Pre-declarations
double x_value(int i, int total_steps, double length);
double v_value(int i, int total_steps, double max_velocity);


//The beginning of our function.
int main(int argc, char *argv[]) {

	//File output for animations
	FILE *fp_gif;
	int frame = 0;
	char f_name[64];

	//Initialise values for our calculations
	double length = 2.0;
	int n_length_steps = 100;
	double velocity_max = 0.5;
	int n_velocity_steps = 100;
	double omega = 1.0;
	double final_time = 2.0;
	int n_time_steps = 200;
	double mass = 1.0;
	double temperature = 1.0;
	double gamma = 1.0;

	//Calculated values for our calculations
	double d_t = final_time / (float) n_time_steps;
	double d_x = length / (float) n_length_steps;
	double d_v = velocity_max * 2 / (float) n_velocity_steps;
	double g = pow(2 * gamma * mass * KB * temperature, 0.5);

	//Create our 2D arrays
	double **new = malloc(sizeof(double *) * n_length_steps + 1);
	for (int i = 0; i < n_length_steps + 1; i++) {
		new[i] = malloc(sizeof(double) * n_velocity_steps + 1);
	}
	double **old;

	//Populate our inital array with values
	//Random Gaussian
	double total = 0;
	for (int i = 0; i < n_length_steps + 1; i++) {
		for (int j = 0; j < n_velocity_steps + 1; j++) {
			new[i][j] = exp(-pow(x_value(i, n_length_steps, length) * 6, 2)) *
					exp(-pow(v_value(j, n_velocity_steps, velocity_max) * 6, 2));
			total += new[i][j] * d_x * d_v;
		}
	}

	//Gibbs Ensemble Gaussian
	//double total = 0;
	//for (int i = 0; i < n_length_steps + 1; i++) {
	//	for (int j = 0; j < n_velocity_steps + 1; j++) {
	//		new[i][j] = exp(-(gamma / g / g) * pow(x_value(i, n_length_steps,
	//				length) * omega * mass, 2)) * exp(-(gamma / g / g) * pow(v_value(j,
	//				n_velocity_steps, velocity_max) * mass, 2));
	//		total += new[i][j] * d_x;
	//	}
	//}

	//Dirac Delta
	//double total = 0;
	//for (int i = 0; i < n_length_steps + 1; i++) {
	//	for (int j = 0; j < n_velocity_steps + 1; j++) {
	//		if ((j == n_velocity_steps / 2) && (i == n_length_steps / 2)) {
	//			new[i][j] = 1;
	//		} else {
	//			new[i][j] = 0;
	//		}
	//		total += new[i][j] * d_x;
	//	}
	//}

	//Normalise our initial array.
	for (int i = 0; i < n_length_steps + 1; i++) {
		for (int j = 0; j < n_velocity_steps + 1; j++) {
			new[i][j] = new[i][j] / total;
		}
	}

	//Print initial array
	strcpy(f_name, "plots/datxxxxx");
	sprintf((f_name + 9), "%05i", frame);
	strcat(f_name, ".dat");
	fp_gif = fopen(f_name, "w");
	frame++;
	
	for (int i = 0; i < n_length_steps + 1; i++) {
		fprintf(fp_gif, "%5.5lf,%5.5lf\n", x_value(i, n_length_steps, length),
				new[i][n_velocity_steps / 2]);
	}
	fclose(fp_gif);

	//Print initial stats of the system and the titles
	printf("Norm\t\t<x>\t\t<x^2>\t\t<p>\t\t<p^2>\t\tVar(x)\t\tVar(p)\n");
	
	//Calculate and print the first set of values
	double prob = 0;
	double tot_prob = 0;
	for (int i = 0; i < n_length_steps + 1; i++) {
		for (int j = 0; j < n_velocity_steps + 1; j++) {
			prob += new[i][j];
		}
		tot_prob += prob * d_x * d_v;
		prob = 0;
	}
	printf("%5.5lf\n", tot_prob);

	//Move our new matrix to become our old matrix, and create a new 'new' matrix.
	old = new;
	new = malloc(sizeof(double *) * n_length_steps + 1);
	for (int i = 0; i < n_length_steps + 1; i++) {
		new[i] = malloc(sizeof(double) * n_velocity_steps + 1);
	}
	
	//This is where the main part of the program starts, and we evolve the
	//states over time. This is a loop over each segment in time.
	for (int t = 0; t < n_time_steps; t++) {

		//For all positions in the new array
		for (int i = 0; i < n_length_steps + 1; i++) {
			for (int j = 0; j < n_velocity_steps + 1; j++) {
		
				//Calculate constants
				double c0 = gamma - (pow(g, 2.0) / pow(mass * d_v, 2.0));
				double ap = -v_value(j, n_velocity_steps, velocity_max) / (2.0 * d_x);
				double am = v_value(j, n_velocity_steps, velocity_max) / (2.0 * d_x);
				double bp = (pow(g, 2.0) / (2.0 * pow(mass * d_v, 2))) +
						((pow(omega, 2.0) * x_value(i, n_length_steps, length) +
						(gamma * v_value(j, n_velocity_steps, velocity_max))) /
						(2.0 * d_v));
				double bm = (pow(g, 2.0) / (2.0 * pow(mass * d_v, 2.0))) -
						((pow(omega, 2.0) * x_value(i, n_length_steps, length) +
						(gamma * v_value(j, n_velocity_steps, velocity_max))) /
						(2.0 * d_v));

				double ap_term;
				double am_term;
				double bp_term;
				double bm_term;

				//These if statements make sure that if I am going to ask for a
				//term outside the range of my arrays, to return a 0 to it.
				if (i == 0) {
					am_term = 0;
				} else {
					am_term = ap * old[i - 1][j];
				}

				if (i == n_length_steps) {
					ap_term = 0;
				} else {
					ap_term = am * old[i + 1][j];
				}

				if (j == 0) {
					bm_term = 0;
				} else {
					bm_term = bp * old[i][j - 1];
				}

				if (j == n_velocity_steps) {
					bp_term = 0;
				} else {
					bp_term = bm * old[i][j + 1];
				}

				//Calculate the new array point from the old one.
				new[i][j] = old[i][j] + d_t * ((c0 * old[i][j]) +
						(ap_term) + (am_term) +	(bp_term) + (bm_term));
			}
		}

		//Print the new array
		strcpy(f_name, "plots/datxxxxx");
		sprintf((f_name + 9), "%05i", frame);
		strcat(f_name, ".dat");
		fp_gif = fopen(f_name, "w");
		frame++;

		for (int i = 0; i < n_length_steps + 1; i++) {
			fprintf(fp_gif, "%5.5lf,%5.5lf\n", x_value(i,
					n_length_steps, length), new[i][n_velocity_steps / 2]);
		}
		fclose(fp_gif);

		//Print out the statistics for this time slice. Like, the norm.
		//Calculate and print the first set of values
		double prob = 0;
		double tot_prob = 0;
		for (int i = 0; i < n_length_steps + 1; i++) {
			for (int j = 0; j < n_velocity_steps + 1; j++) {
				prob += new[i][j];
			}
			tot_prob += prob * d_x * d_v;
			prob = 0;
		}
		printf("%5.5lf\n", tot_prob);

		//Rearrange our time slices by:
		//freeing old
		for (int i = 0; i < n_length_steps + 1; i++) {
			free(old[i]);
		}
		free(old);
		//making 'new' 'old'
		old = new;
		//making a new 'new'
		new = malloc(sizeof(double *) * n_length_steps + 1);
		for (int i = 0; i < n_length_steps + 1; i++) {
			new[i] = malloc(sizeof(double) * n_velocity_steps + 1);
		}
	}

	//Free our arrays: 'Old' and 'New'
	for (int i = 0; i < n_length_steps + 1; i++) {
		free(new[i]);
		free(old[i]);
	}
	free(new);
	free(old);

	return 0;
}


//This function is an abstration to make our code more readable. We give it
//the int i, which is the ith itteration through the array, the total number
//of steps, and the total length, and this returns the position value of the
//i'th position in the array.
double x_value(int i, int total_steps, double length) {
	double value;

	value = ( (float) i / (float) total_steps - 0.5) * length;

	return value;
}


//Almost identical to the previous function, this also acts as an abstraction.
//You give it the i'th time through the array, the total number of steps and
//the maximum velocity, and it returns the velocity value of that specific
//position in the array.
double v_value(int i, int total_steps, double max_velocity) {
	double value;

	value = ( (float) i / (float) total_steps - 0.5) * 2 * max_velocity;

	return value;
}
