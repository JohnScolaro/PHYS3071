/* PHYS3071
 * Student Name: John Scolaro
 * Student ID: s4320824
 * Assignment 5
 * 
 * This program calculates the changing density of neutron stars using the
 * half-step method.
 */

//Includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//All units are defined in SI units
#define speed_of_light (299792458)
#define electron_volt (1.602*pow(10, -19))
#define solar_mass (1.98892*pow(10, 30))
#define k (1.322*pow(10, -42))
#define B 57.0
#define PI 3.141592653589

//Function prototypes
double epsilon_derivative_newton(double, double, double);
double epsilon_derivative_tov(double, double, double);
double mass_derivative(double, double);
double mev_solar_mass_conversion(double);

//The main function
int main(int argc, char *argv[]) {
	double e0, epsilon_nought, radius, radius_newton;
	double epsilon, epsilon_derivative, enclosed_mass, enclosed_mass_derivative;
	double epsilon_tov, epsilon_derivative_tv, enclosed_mass_tov, enclosed_mass_derivative_tov;

	FILE *fp;
	fp = fopen("massvsradius.dat", "w");

	for (e0 = 4.2; e0 < 20; e0 += (15.8/80)) {
		epsilon_nought = e0 * B;
		epsilon = epsilon_nought;
		epsilon_tov = epsilon_nought;
		enclosed_mass = 0;
		enclosed_mass_tov = 0;
	
		for (radius = pow(10, 16); epsilon > (4 * B); radius += pow(10, 16)) {	
			//For the worksheet calculations
			enclosed_mass_derivative = mass_derivative(radius + (5 * pow(10, 15)), epsilon);
			enclosed_mass += enclosed_mass_derivative * pow(10, 16);
			epsilon_derivative = epsilon_derivative_newton(radius + (5 * pow(10, 15)), epsilon, enclosed_mass);	
			epsilon += epsilon_derivative * pow(10, 16);
		}
		radius_newton = radius;

		for (radius = pow(10, 16); epsilon_tov > (4 * B); radius += pow(10, 16)) {
			//For the assignment 5 calculations
			enclosed_mass_derivative_tov = mass_derivative(radius + (5 * pow(10, 15)), epsilon_tov);
			enclosed_mass_tov += enclosed_mass_derivative_tov * pow(10, 16);
			epsilon_derivative_tv = epsilon_derivative_tov(radius + (5 * pow(10, 15)), epsilon_tov, enclosed_mass_tov);
			epsilon_tov += epsilon_derivative_tv * pow(10, 16);
		}
		
		//Here, radii are measured in km, and mass is measured in solar masses
		fprintf(fp, "%10.10lf,", epsilon_nought);
		fprintf(fp, "%10.10lf,", radius_newton * pow(10, -18));
		fprintf(fp, "%10.10lf,", mev_solar_mass_conversion(enclosed_mass));
		fprintf(fp, "%10.10lf,", radius * pow(10, -18));
		fprintf(fp, "%10.10lf\n", mev_solar_mass_conversion(enclosed_mass_tov));
	}
	fclose(fp);
	return 0;
}

//Returns the derivative of the pressure according to the equation for a
//spherically symetric mass distribution
double epsilon_derivative_newton(double radius, double epsilon, double enclosed_mass) {
	return -3 * k * epsilon * enclosed_mass / (pow(radius, 2));
}

//Returns the derivative of the pressure according to the TOV equation
double epsilon_derivative_tov(double radius, double epsilon, double enclosed_mass) {
	double p = (1.0/3.0 * (epsilon - (4 * B)));
	double top_left = epsilon + p;
	double top_right = enclosed_mass + (4 * p * PI * pow(radius, 3));
	double bottom = pow(radius, 2) * (1 - (2 * k * enclosed_mass / radius));
	return 3 * -k * top_left * top_right / bottom;
}

//Returns the derivative of the mass according to the Newtonian equation
double mass_derivative(double radius, double epsilon) {
	return 4 * PI * pow(radius, 2) * epsilon;
}

//Converts between MeV to Solar Masses
double mev_solar_mass_conversion(double mev) {
	return mev/solar_mass*electron_volt/speed_of_light/speed_of_light*1000000;
}
