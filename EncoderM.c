#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pca9685.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define DEV_ID0		0x40
#define PIN_BASE0 	64
#define PWM_MAX 	4096
#define HERTZ		50

#define motorPin1	0 + PIN_BASE0		//Motor Vorwärts        out digital
#define motorPin2	1 + PIN_BASE0	  	//Motor Rückwärts       out digital
#define enablePin	2 + PIN_BASE0  		//Motor Geschwindigkeit out PWM

#define PhaseApin   	2	//pins directly connected to Motor
#define PhaseBpin   	3

#define Teeth		32	//number of teeth on the encoder wheel




int CountA=2, Direction=2, i=0;
float factor=1, v;

void CountA_inc(void){
	CountA++;
}
void CheckDir(void){
	if (digitalRead(PhaseApin)==HIGH) Direction = 1;
	if (digitalRead(PhaseApin)==LOW) Direction = -1;
}

float Speed_Current (void){
	
	CountA = 0;
	delay(100);
	return (CountA/Teeth)*6000;
}


int motor(int pwr) {
	if(pwr>0){
		pwmWrite(motorPin1,PWM_MAX);
		pwmWrite(motorPin2,0);
		//printf("turn Forward...\n");
	}
	else if (pwr<0){
		pwmWrite(motorPin1,0);
		pwmWrite(motorPin2,PWM_MAX);
		//printf("turn Back...\n");
	}
	else {
		pwmWrite(motorPin1,0); 
		pwmWrite(motorPin2,0);
		//printf("Motor Stop...\n");
	}
	pwmWrite(enablePin,abs(pwr));
	return 0;
}

int main(void) {
	//setup
	if(wiringPiSetup() == -1){ 
		printf("Setup wiringPi failed !");
		return 1;
	}
	if(wiringPiI2CSetup(DEV_ID0) == -1){
		printf("setup wiringPi I2C faiservo !");
		return 1;
	}
	int fd = pca9685Setup(PIN_BASE0, DEV_ID0, HERTZ);
	if (fd < 0) {
		printf("Error in setup\n");
		return fd;
	}
	pinMode(7,OUTPUT);
	digitalWrite(7,LOW);
	
	pinMode(enablePin,OUTPUT);
	pinMode(motorPin1,OUTPUT);
	pinMode(motorPin2,OUTPUT);
	
	pinMode(PhaseApin,INPUT);
	pinMode(PhaseBpin,INPUT);
	// Direction_init
	if ( wiringPiISR (PhaseBpin, INT_EDGE_FALLING, &CheckDir) < 0 ) {
		printf("CheckDir failed!");
	}
	// Speed_init
	
	if ( wiringPiISR (PhaseApin, INT_EDGE_FALLING, &CountA_inc) < 0 ) {
		printf("CountA failed!");
	}
	
	//loop
	
	printf(" Check Pin %i and %i for input\n",PhaseApin,PhaseBpin);
	i=0;
/*	while (0) {
		i=digitalRead(PhaseApin);
		printf(" Phase A: %i ",i);
		i=digitalRead(PhaseBpin);
		printf(" Phase B: %i \n",i);
	}
*/
	delay(1000);
	while(1){
		for (i=-PWM_MAX;i<=PWM_MAX;i++) {
			motor(i);
			v = Speed_Current();
			factor = 1;
			if (v!=0) factor = abs(i)/v;
			printf("Power %i%% Speed %frpm Direction %i (%f)\n",i,v,Direction,factor);
			delay(200);
		}
		for (i=+PWM_MAX;i<=-PWM_MAX;i--) {
			motor(i);
			v = Speed_Current();
			factor = 1;
			if (v!=0) factor = abs(i)/v;
			printf("Power %i%% Speed %frpm Direction %i (%f)\n",i,v,Direction,factor);
			delay(200);
		}
	}
	return 0;
}
