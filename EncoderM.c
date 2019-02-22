#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define motorPin1	21	//define the pins connected to Motor Driver (L293D)
#define motorPin2	22
#define enablePin	23
#define PhaseApin   	24	//pins directly connected to Motor
#define PhaseBpin   	25
#define Teeth		32	//number of teeth on the encoder wheel

int CountA, Direction, i;
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
	return (CountA/Teeth)*600;
}


		

// RPM messurement


int motor(int pwr) {
	if(pwr>0){
		digitalWrite(motorPin1,HIGH);
		digitalWrite(motorPin2,LOW);
		//printf("turn Forward...\n");
	}
	else if (pwr<0){
		digitalWrite(motorPin1,LOW);
		digitalWrite(motorPin2,HIGH);
		//printf("turn Back...\n");
	}
	else {
		digitalWrite(motorPin1,LOW); 
		digitalWrite(motorPin2,LOW);
		//printf("Motor Stop...\n");
	}
	softPwmWrite(enablePin,abs(pwr));
	return 0;
}

int main(void) {
	//setup
	if(wiringPiSetup() == -1){ 
		printf("Setup wiringPi failed !");
		return 1;
	}
	pinMode(enablePin,OUTPUT);
	pinMode(motorPin1,OUTPUT);
	pinMode(motorPin2,OUTPUT);
	softPwmCreate(enablePin,0,100);//define PMW pin
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
}
	
	//loop
	printf(" Check Pin %i and %i for input\n",PhaseApin,PhaseBpin);
	i=0;
	while (i=0) {
		i=digitalRead(PhaseApin);
		printf(" Phase A: %i ",i);
		i=digitalRead(PhaseBpin);
		printf(" Phase B: %i \n",i);
	}
	delay(1000);
	while(1){
		
		for (i=-100;i<=100;i++) {
			motor(i);
			v = Speed_Current();
			if (v!=0) factor = abs(i)/v;
			printf("Power %i%% Speed %irpm Direction %i (%f)\n",i,v,Direction,factor);
			delay(200);
		}
		for (i=+100;i<=-100;i--) {
			motor(i);
			v = Speed_Current();
			if (v!=0) factor = abs(i)/v;
			printf("Power %i%% Speed %frpm Direction %i (%f)\n",i,v,Direction,factor);
			delay(200);
		}
	}
	return 0;
}
