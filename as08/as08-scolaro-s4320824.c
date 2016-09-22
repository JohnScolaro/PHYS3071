/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: s4320824
 * Assignment 8
 * 
 * Solves a boundary value problem of a human head against an air bag in a car
 * crash scenario. Values can be tweaked in order to optimally slow the
 * heads velocity.
 *
 * Made with gcc flags: -std=c99 -lm
 */

//Includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//Function Pre-declarations
int tridag(double *, double *, double *, double *, double *, int);

int main(int argc, char *argv[]) {
	
	//Prints usage
	if (argc != 8) {
		printf("Usage: [mass] [initial airbag force] [decay rate] [final_time] [final_position] [n] [\"name of file\"] \n");
		exit(-1);
	}

	//Declares and initialised variables
	double mass = atof(argv[1]);
	double initial_airbag_force = atof(argv[2]);
	double decay_rate = atof(argv[3]);
	double final_time = atof(argv[4]);
	double final_position = atof(argv[5]);
	int n = atoi(argv[6]);
	double initial_position = 0.0;

	int size = n;
	double delta_t = final_time / (double) n;
	FILE *fp;
	fp = fopen(argv[7], "w");

	double diag[size - 1], lower[size - 1], upper[size - 1];
	double rhs[size - 1], soln[size - 1];
	double acceleration, time = 0;

	//Populate tridiagonal matrix, and other matrixes to solve the problem
	for (int i = 0; i < size - 1; i++) {
		diag[i] = -2;
		upper[i] = 1;
		lower[i] = 1;
		time += delta_t;
		acceleration = -initial_airbag_force * exp(-decay_rate * time) / mass;
		if (i == 0) {
			rhs[i] = acceleration * pow(delta_t, 2) - initial_position;
		} else if (i == size - 2) {
			rhs[i] = acceleration * pow(delta_t, 2) - final_position;
		} else {
			rhs[i] = acceleration * pow(delta_t, 2);
		}
	}

	//Solve matrix
	int err = tridag(diag, lower, upper, rhs, soln, size);

	//Solve velocity either via the forwards finite difference, the backwards
	//finite difference, or the central finite difference depending on whether
	//it's the first point, the last point, or the middle points respectivly.
	double velocity;
	if (err == -1) {
		printf("No solution found");
		exit(-1);
	} else {
		fprintf(fp, "0.000000,0.000000,%lf\n", (soln[0]) / delta_t);
		for (int index = 0; index < size - 1; index++) {
			if (index != size - 2) {
				velocity = (soln[index + 1] - soln[index - 1]) / (delta_t * 2);
			} else {
				velocity = (final_position - soln[index - 1]) / (delta_t * 2);
			}
			fprintf(fp, "%lf,%lf,%lf\n", (double) (index + 1) * delta_t, soln[index], velocity);
		}
	}
	fprintf(fp, "%lf,%lf,%lf\n", final_time, final_position, (final_position - soln[size - 2]) / delta_t);

	fclose(fp);
	return 0;
}

//This is the function that solves the tridiagonal matrix 
int tridag(double adiag[], double alower[], double aupper[], double rrhs[], double xsoln[], int n) {
	int j;
	double bet;
	double *gam;

	if (fabs(adiag[0]) == 0.0) {
		return -1;    // Error
	}

	/* one vector of workspace is needed */
	gam = malloc(n * sizeof(double));

	bet = adiag[0];   // bet this is used as workspace
	xsoln[0] = rrhs[0] / bet;

	for (j = 1; j < n; j++) {
		gam[j] = aupper[j - 1] / bet;
		bet = adiag[j] - alower[j] * gam[j];
		if (fabs(bet) == 0.0) {
			return -1;   // Pivoting Error
		}
		xsoln[j] = (rrhs[j] - alower[j] * xsoln[j - 1]) / bet;
	}
	for (j = n - 2; j >= 0; j--) {
    	xsoln[j] -= gam[j + 1] * xsoln[j + 1];  // Backsubstition
	}
	free(gam);
	return 0;
}
