/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: s4320824
 * Assignment 9
 *
 * Write something here! 
 *
 * Made with gcc flags: -std=c99 -lm
 */

//Includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

//Function Predeclarations
void print_norm(FILE *, double *, int, double);
void print_average(FILE *, double *, int, double, double);

int main(int argc, char *argv[]) {
	//Declare all variables for our program
	double velocity, width, gb_left, gb_right, delta_t, delta_x, k;
	int num_points, num_time_steps, i;
	//File pointer for out data output
	FILE *fp;
	//File output for animations
	FILE *fp_gif;
	char f_name[64];


	//Checking usage
	if (argc != 8) {
		printf("Usage: [velocity] [width] [grid boundary left] [grid boundary right] [number of grid points] [delta_t] [number of time steps]\n");
		exit(-1);
	}

	//Populating variables from inputs
	velocity = atof(argv[1]);
	width = atof(argv[2]) / 2;
	gb_left = atof(argv[3]);
	gb_right = atof(argv[4]);
	num_points = atoi(argv[5]);
	delta_t = atof(argv[6]);
	num_time_steps = atoi(argv[7]);

	//Calculating some values from inputs
	delta_x = (gb_right - gb_left) / (num_points + 1);
	k = velocity * delta_t / (2 * delta_x);
	
	if ((k * 2) > 1) {
		printf("This system is unstable!\n");
	} else {
		printf("This system is stable!\n");
	}

	//Creating dynamically sized arrays for our calculations
	double *u_n0 = malloc(num_points * sizeof(double));
	double *u_n1 = malloc(num_points * sizeof(double));

	//Create initial matrix
	double x = gb_left;
	for (int i = 0; i < num_points; i++) {
		x += delta_x;
		if (x == 0) {
			u_n1[i] = 1.0;
		} else if ((x < width) && (x > -width)) {
			if (x > 0) {
				u_n1[i] = -x / width + 1.0;
			} else if (x < 0) {
				u_n1[i] = x / width + 1.0;
			}
		} else {
			u_n1[i] = 0.0;
		}
	}

	//Normalise the initial matrix
	double tot = 0;
	for (int i = 0; i < num_points; i++) {
		tot += ((u_n1[i] + u_n1[i + 1]) / 2) * delta_x;
	}
	for (int i = 0; i < num_points; i++) {
		u_n1[i] /= tot;
	}

	//Print initial matrix
	fp = fopen("data.dat", "w");
	i = 0;
	fprintf(fp, "Iteration Number, Current Time, Norm, Average Location of the Function\n");
	fprintf(fp, "%d,", i);
	fprintf(fp, "%lf,", i * delta_t);
	print_norm(fp, u_n1, num_points, delta_x);
	fprintf(fp, ",");
	print_average(fp, u_n1, num_points, delta_x, gb_left);
	fprintf(fp, "\n");

	//Print out first set of points
	strcpy(f_name, "plots2/datxxxx");
	sprintf((f_name + 10), "%05i", i);
	strcat(f_name, ".dat");
	fp_gif = fopen(f_name, "w");
	
	x = gb_left;
	for (int j = 0; j < num_points; j++) {
		fprintf(fp_gif, "%5.5lf %5.5lf\n", x, u_n1[j]);
		x += delta_x;
	}
	fclose(fp_gif);

	//Loop through everything, make all the arrays and print them to the data file.
	for (i = 1; i < num_time_steps; i++) {
		
		//Move arrays around. New becomes old.
		free(u_n0);
		u_n0 = u_n1;
		u_n1 = malloc(num_points * sizeof(double));

		//Populate new array
		for (int j = 0; j < num_points; j++) {
			if (j == 0) {
				u_n1[j] = ((u_n0[j + 1]) / 2.0) - (k * (u_n0[j + 1]));
			} else if (j == num_points - 1) {
				u_n1[j] = ((u_n0[j - 1]) / 2.0) + (k * (u_n0[j - 1]));
			} else {
				u_n1[j] = ((u_n0[j + 1] + u_n0[j - 1]) / 2.0) - (k * (u_n0[j + 1] - u_n0[j - 1]));
			}
		}

		//Print new data file for gif
		strcpy(f_name, "plots2/datxxxx");
		sprintf((f_name + 10), "%05i", i);
		strcat(f_name, ".dat");
		fp_gif = fopen(f_name, "w");
		x = gb_left;
		for (int j = 0; j < num_points; j++) {
			fprintf(fp_gif, "%5.5lf %5.5lf\n", x, u_n1[j]);
			x += delta_x;
		}
		fclose(fp_gif);

		//Print new array
		fprintf(fp, "%d,", i);
		fprintf(fp, "%lf,", i * delta_t);
		print_norm(fp, u_n1, num_points, delta_x);
		fprintf(fp, ",");
		print_average(fp, u_n1, num_points, delta_x, gb_left);
		fprintf(fp, "\n");
	}

	//Close the file pointer and end the program
	fclose(fp);
	free(u_n0);
	free(u_n1);
	return 0;
}

//This function prints the norm of an array that it is passed. It prints it to fp.
void print_norm(FILE *fp, double *values, int num_points, double delta_x) {
	double sum = 0;

	for (int i = 0; i < num_points; i++) {
		sum += values[i] * delta_x;
	}

	fprintf(fp, "%lf", sum);
}

//This function prints the average value of an array it is passed to the file pointer fp.
void print_average(FILE *fp, double *values, int num_points, double delta_x, double gb_left) {
	double sum_top = 0;
	double sum_bottom = 0;
	double x = gb_left;

	for (int i = 0; i < num_points; i++) {
		sum_top += x * values[i] * delta_x;
		sum_bottom += values[i] * delta_x;
		x += delta_x;
	}

	fprintf(fp, "%lf", sum_top / sum_bottom);
}
