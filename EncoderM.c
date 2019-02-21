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
#define Teeth		40	//number of teeth on the encoder wheel

int CountA, Direction;
float factor;

void CountA_inc(void){
	CountA++;
}
void CheckDir(void){
	if (digitalRead(PhaseApin)==HIGH) Direction = 1;
	if (digitalRead(PhaseApin)==LOW) Direction = -1;
}

int Speed_Current (void){
	if ( wiringPiISR (PhaseApin, INT_EDGE_FALLING, &CountA_inc) < 0 ) {
		printf("CountA failed!");
	}
	CountA = 0;
	delay(100);
	return (CountA/Teeth)*600;
}

int Direction_init (void){
	if ( wiringPiISR (PhaseBpin, INT_EDGE_FALLING, &CheckDir) < 0 ) {
		printf("CheckDir failed!");
	}
	return Direction;
}
		

// RPM messurement


int motor(int pwr) {
	if(pwr>0){
		digitalWrite(motorPin1,HIGH);
		digitalWrite(motorPin2,LOW);
		printf("turn Forward...\n");
	}
	else if (pwr<0){
		digitalWrite(motorPin1,LOW);
		digitalWrite(motorPin2,HIGH);
		printf("turn Back...\n");
	}
	else {
		digitalWrite(motorPin1,LOW); 
		digitalWrite(motorPin2,LOW);
		printf("Motor Stop...\n");
	}
	softPwmWrite(enablePin,abs(pwr));
	return 0;
}

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
	Direction_init();
	
	//loop
	while(1){
		int i;
		for (i=-100;i<=100;i++) {
			motor(i);
			factor = abs(i)/Speed_Current();
			printf("Power %i%% Speed %irpm Direction %i (%f)\n",i,Speed_Current(),Direction,factor);
			delay(200);
		}
		for (i=+100;i<=-100;i--) {
			motor(i);
			printf("Power %i%% Speed %irpm Direction %i \n",i,Speed_Current(),Direction);
			delay(200);
		}
	}
	return 0;
}
