/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: s4320824
 * Assignment 4
 * 
 * This assignment calculates a differential equation, and uses it to find out
 * how long it would take for a fuel talk to drain.
 */

//Includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//Function Prototypes
double delta_h(double, double, double);

//Comment explaining the main function.
int main(int argc, char *argv[]) {
	double ratio, initial_height, current_height, delta_t, last_time, time_step;
	double time = 0;
	FILE *fp;

	//Reading in values
	printf("Please enter an initial height: ");
	if (scanf("%lf", &initial_height) < 0) {
		printf("There was an error reading in the initial height\n");
		return -1;
	}
	printf("Please enter the ratio S_a/S_b: ");
	if (scanf("%lf", &ratio) < 0) {
		printf("There was an error reading in the ratio S_a/S_b\n");
		return -1;
	}
	printf("Please enter the time step delta_t (For calculation precision): ");
	if (scanf("%lf", &delta_t) < 0) {
		printf("There was an error reading in delta_t\n");
		return -1;
	}
	printf("Please enter the time step to print out at (Limits the size of the output file): ");
	if (scanf("%lf", &time_step) < 0) {
		printf("There was an error reading in the time step to print\n");
		return -1;
	}
	
	//Checking to make sure all the entered values are valid.
	if (initial_height <= 0) {
		printf("Please enter a positive height\n");
		return -1;
	}
	if (ratio <= 1) {
		printf("Please enter a ratio greater or equal to 1\n");
		return -1;
	}
	if (delta_t <= 0) {
		printf("Please enter a positive time step\n");
		return -1;
	}
	
	last_time = 0;
	current_height = initial_height;
	fp = fopen("data.txt", "w");
	while (current_height > 0) {
		time += delta_t;
		current_height -= delta_h(current_height, delta_t, ratio);
		if (current_height < 0) {
			break;
		}
		if ((time - time_step) > last_time) {
			fprintf(fp, "%lf,%lf,%lf\n", time, current_height, sqrt(2*9.81*current_height));
			last_time += time_step;
		}
	}
	fclose(fp);

	return 0;
}

double delta_h(double height, double time, double ratio) {
	return (sqrt((2 * 9.81 * height)/(pow(ratio, 2.0) - 1)) * time);
}
