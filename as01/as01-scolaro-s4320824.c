/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: 43208241
 * Assignment 1
 */

/* Includes */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

/* Main function
 * @brief: Performs maths on two variables, and prints the answer.
 * @param: Two ints to calculate with
 * @retval: None
 */
int main(int argc, char* argv[]) {
	//Initialises two strings to hold our numbers. 100 Digits is long enough.
	char xstring[100];
	char ystring[100];

	//Saves the arguements so we can use them in calculations.
	printf("Please input the number to use for x: ");
	scanf("%s", &xstring);
	printf("Please input the number to use for y: ");
	scanf("%s", &ystring);

	//Converts inputted cariables to doubles. Floats are not big enough to hold
	//the numbers in this question.
	double x = atof(xstring);
	double y = atof(ystring);

	//I break the function up into 4 parts. The front part of the equations has
	//the sin, and the top and bottom are the numerator and denominator of the
	//fraction.
	double front;
	double top;
	double bottom;
	double final;
	
	front = sin(x*x + y);
	top = (1 - pow(2, pow(x, 2) + pow(y, 2)));
	bottom = (1 + pow(2, pow(x, 2) + pow(y, 2)));
	final = front + (top/bottom);

	//Here I print the answer.
	printf("The answer is: %lf\n", final);
	
	return 0;
}
