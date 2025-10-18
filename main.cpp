#include <stdio.h>
#include <cstdlib>
#include "capacitor.h"

/*
V(t+1) = V(t) + I(t)*(delta-t)*(1/C)
rearrange:
V(t+1-1) = V(t-1) + I(t-1)*(delta-t)*(1/C)
    V(t) = V(t-1) + I(t-1)*(delta-t)*(1/C)

I(t+1) = I(t) − [I(t)/(R*C)]*delta-t
=>
I(t) = I(t-1) − [I(t-1)/(R*C)]*delta-t
*/
void cal_voltage_current(Capacitor *capacitor, double dt, double final_time, double R, double C, int num_step, int interval) {	
	for(int i = 1; i < num_step; i++) {
		capacitor->time[i] = i*dt;
		capacitor->current[i] = capacitor->current[i-1] - ((capacitor->current[i-1])/(R*C))*dt;
		capacitor->voltage[i] = capacitor->voltage[i-1] + (capacitor->current[i-1])*(1/C)*dt;		
	}
}

int main() {
	Capacitor capacitor;
	
	// initial values	
	double dt = 1e-10; // 0.0000000001; //1e-10 s;
	double initial_time = 0.0; // t0
    double final_time = 5e-6; // 0.000005; // 5e-6 s
    int num_step = 50000; // Number of timesteps = 5e-6/1e-10 = 50000
    double R = 1000; // R = 1 kOhm;
    double C = 1e-10; // 0.0000000001; // 0.01*100; // C = 100e-12 F		= 1e-10    
    double V_zero = 10.0; // V 0 = 10.0 V (for constant voltage supply)
	double I = V_zero/R;
	int interval = 200;
	
	//allocate enough memory to each structure component for 50k doubles; cast to type *double
	capacitor.time = (double*)malloc(num_step*sizeof(double));
	capacitor.current = (double*)malloc(num_step*sizeof(double));
	capacitor.voltage = (double*)malloc(num_step*sizeof(double));

	// input initial values into the vector
	capacitor.time[0] = initial_time;
	capacitor.current[0] = I; // becomes V_zero/R right at time t=0 [s]
	capacitor.voltage[0] = 0; // before t = 0, switch is open, thus capacitor receiving no current and Vc = 0
	capacitor.C = C;
	
	cal_voltage_current(&capacitor, dt, final_time, R, C, num_step, interval);
	
	printf("\n");
	
	// print out every 200 values of Vc and ic 
	for(int i = 0; i < num_step; i+=interval) {
		printf("TIME = %5.8f [s]\t", capacitor.time[i]);		
		printf("Current = %5.4f [A]\t", capacitor.current[i]);
		printf("Voltage = %5.4f [V]\n", capacitor.voltage[i]);
	}
	
	free(capacitor.time);		// free dynamically allocated memory
	free(capacitor.current);
	free(capacitor.voltage);
	
	return 0;
}

