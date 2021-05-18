/*
 * DIO.h
 *
 * Created: 08/05/2021 2:06:02 AM
 *  Author: Mark Fayez
 */ 

#ifndef MOTORS_H_
#define MOTORS_H_

#include "Timer.h"
#include "DIO.h"
#include "Motors_CFG.h"

#define DISABLE 0
#define ENABLE  1

#if ((MOTOR_A_SEL == ENABLE) || (MOTOR_B_SEL == ENABLE))
    
    void DCMotor_Init(void);

    #if (MOTOR_A_SEL == ENABLE)
        void DCMotor_A_SetDir(DCMotor_Dir dir);
        void DCMotor_A_Start(void);
        void DCMotor_A_Stop(void);
    #endif

    #if (MOTOR_B_SEL == ENABLE)
        void DCMotor_B_SetDir(DCMotor_Dir dir);
        void DCMotor_B_Start(void);
        void DCMotor_B_Stop(void);
    #endif

    #if ((MOTOR_A_SEL == ENABLE) && (MOTOR_B_SEL == ENABLE))
        void DCMotors_SetSpeed(uint16 Speed_A, uint16 Speed_B);
    #elif (MOTOR_A_SEL == ENABLE)
        void DCMotor_A_SetSpeed(uint16 Speed_A);
    #elif (MOTOR_B_SEL == ENABLE)
        void DCMotor_B_SetSpeed(uint16 Speed_B);
    #endif
#endif

#if SERVO_MOTOR_SEL == ENABLE
    void Servo_Motor_Init(void);
    void Servo_Motor_Move(sint8 Degrees);
#endif

#endif