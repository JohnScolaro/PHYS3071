/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: s4320824
 * Assignment 3
 * 
 * This assignment calculates the area under an integral using the trapezoidal
 * method. It takes the highest value of v_{max}, and an accuracy, and
 * calculates the integral repeatedly until the desired accuracy is obtained.
 */

//#DEFINE to switch between the advanced task, and the intermediate task.
#define ADVANCED

//Includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//Function Prototypes:
double calculate_a(void);
double equation(double);
double integral(unsigned int, double, double, double);


//Runs the algorithm that calculates the integral
int main(int argc, char *argv[]) {
	double v_max;
	double v_min;
	double accuracy;
	unsigned int points;
	double difference;
	double a = calculate_a();
	double i;
	double j;

	printf("Please enter an upper bound: ");	
	if (scanf("%lf", &v_max) < 0) {
		printf("There was an error reading in v_max\n");
		exit(1);
	}

	printf("Please enter a lower bound: ");
	if (scanf("%lf", &v_min) < 0) {
		printf("There was an error reading in v_min\n");
		exit(1);
	}

	printf("Please enter the accuracy to calculate to: ");
	if (scanf("%lf", &accuracy) < 0) {
		printf("There was an error reading in the accuracy\n");
		exit(1);
	}
	
	printf("+-------------+-----------------+-----------------+\n");
	printf("|     Points  |      Value      |    Difference   |\n");
	printf("+-------------+-----------------+-----------------+\n");

	points = 40;
	i = integral(10, v_max, v_min, a);
	printf("|         10  |  %11.11lf  |                 |\n", i);
	j = integral(20, v_max, v_min, a);
	difference = i - j;
	printf("|         20  |  %11.11lf  |  %11.11lf  |\n", j, difference);

	while ((difference > accuracy) || (-difference > accuracy))  {
		i = j;
		j = integral(points, v_max, v_min, a);
		difference = i - j;
		printf("| %10.0u  |  %11.11lf  |  %11.11lf  |\n", points, j, difference);
		points *= 2;
	}

	printf("+-------------+-----------------+-----------------+\n");
	printf("The final converged upon value is: %0.15lf\n\n", j);		
	return j;
}

//Used to calculate alpha accurately.
double calculate_a(void) {
	return (double) 2.949444/(pow(2, 3.0/2.0));
}

//Given a position x, this calculates the value y for the function y(x)
double equation(double x) {
	#ifndef ADVANCED
	return pow(x, 2.0)/(1 + exp(pow(x, 2.0)/2.0));
	#else
	double var = pow(((1.0/x) - 1), 2.0);
	return var/(pow(x, 2.0) * (1 + exp(0.5 * var)));
	#endif
}

//Calculates the integral with a given number of rectangles
double integral(unsigned int points, double v_max, double v_min, double a) {
	unsigned int i;
	double x;
	double sum = 0;
	double h;

	h = (v_max - v_min)/points;

	for (i = 0; i <= points; i++) {
		x = v_min + (i * (v_max - v_min)/points);
		if ((i == 0) || (i == points)) {
			sum += equation(x);
		} else {
			sum += 2 * equation(x);
		}
	}
	return (sum * h * a) / 2;
}
