#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define motorPin1	21	//define the pins connected to Motor Driver (L293D)
#define motorPin2	22
#define enablePin	23

#define PhaseApin   24
#define PhaseBpin   25

int main(void) {
	//setup
	if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
		printf("setup wiringPi failed !");
		return 1;
	}
	pinMode(enablePin,OUTPUT);
	pinMode(motorPin1,OUTPUT);
	pinMode(motorPin2,OUTPUT);
	softPwmCreate(enablePin,0,100);//define PMW pin
	pinMode(PhaseApin,INPUT);
	pinMode(PhaseBpin,INPUT);
	
	//loop
	while(1){
	}
