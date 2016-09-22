/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: s4320824
 * Assignment 2
 * 
 * This function asks for a number 'n' from stdin, and then calculates a bunch
 * of values of x for that particular hermine polynomial. These are then used
 * to plot a nice graph of these values using GNUplot.
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//Function prototypes
int calculate_a_n(int, int);
float hermite(float, int);

int main(int argc, char* argv[]) {

	//Creates out array of 101 x values between -1 and 1.
	float array[100];
	for (int i = 0; i < 101; i++) {
		float b = (i / 50.0) - 1;
		array[i] = b;
	}

	//Asks the user for an integer, to use as 'n' in our calculations.
	int n;
	printf("Please enter an integer to use as 'n': ");
	scanf("%d", &n);
	if (n > 20) {
		printf("You entered an integer greater than 20, these results are not \
				guarenteed to be accurate\n");
	}

	//Opens our file to write to, and also handles errors.
	FILE *fp;
	fp = fopen("./as02-scolaro-s4320824-data", "w");
	if (fp == NULL) {
		printf("Error opening file to write\n");
	}

	//Here we start doing actual maths to compute the problem.	
	for (int i = 0; i < 101; i++) {
		fprintf(fp, "%f", array[i]);
		fprintf(fp, ",");
		fprintf(fp, "%f\n", hermite(array[i], n));
	}	

	//Close the file, and report any errors
	if (fclose(fp) != 0) {
		printf("There was an error closing your file.\n");
	}

	return 0;
}

int calculate_a_n(int n, int k) {
	if ((n == 0) && (k == 0)) {
		return 1;
	} else if (k == 0) {
		return -1 * calculate_a_n(n - 1, k + 1);
	} else if (k > n) {
		return 0;
	} else {
		return (2 * calculate_a_n(n - 1, k - 1)) -
				((k + 1) * calculate_a_n(n - 1, k + 1));
	}
}

float hermite(float x, int n) {
	float sum = 0;
	int k;
	float k2;

	for (k = 0; k <= n; k++) {
		k2 = k;
		sum += calculate_a_n(n, k) * powf(x, k2);
	}	

	return sum;
}
