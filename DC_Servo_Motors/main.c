/*
 * Motors_Drivers.c
 *
 * Created: 18/04/2021 8:11:34 PM
 * Author : Mark Fayez
 */ 

#include "Motors.h"


int main(void)
{
	
	DCMotor_Init();
	DCMotor_B_SetDir(ANTI_CLOCK_WISE_DIR);
	
    Servo_Motor_Init();
	
	DCMotor_B_SetSpeed(100);
	
	DCMotor_B_Start();
	Servo_Motor_Move(50);
	
    while (1) 
    {
    }
}

