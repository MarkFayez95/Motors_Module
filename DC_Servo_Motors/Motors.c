/*
 * Motors.c
 *
 * Created: 08/05/2021 2:06:02 AM
 *  Author: Mark Fayez
 */ 
#include "Motors.h"

extern TIMER1_CONF	Timer_1;
extern TIMER2_CONF	Timer_2;

void DCMotor_Init(void)
{
	#if MOTOR_A_SEL == ENABLE
		DIO_SetPinDir(H_BRIDGE_DIR_DIO_PORT, H_BRIDGE_DIR_PIN_1, DIO_PIN_OUTPUT); // H-1 Pin
		DIO_SetPinDir(H_BRIDGE_DIR_DIO_PORT, H_BRIDGE_DIR_PIN_2, DIO_PIN_OUTPUT); // H-2 Pin
	#endif
	
	#if MOTOR_B_SEL == ENABLE
		DIO_SetPinDir(H_BRIDGE_DIR_DIO_PORT, H_BRIDGE_DIR_PIN_3, DIO_PIN_OUTPUT); // H-3 Pin
		DIO_SetPinDir(H_BRIDGE_DIR_DIO_PORT, H_BRIDGE_DIR_PIN_4, DIO_PIN_OUTPUT); // H-4 Pin	
	#endif
	
	// Timer_1 / PWM_1 Configurations::
	Timer_1.OperationType_WGM1_32 = NORM_PWM_PH_CO_TYPE;
	Timer_1.Operation_Ph_Co_Type_WGM1_10 = PWM_PH_CO_10_BIT;
	
	#if ((MOTOR_A_SEL == ENABLE) && (MOTOR_B_SEL == ENABLE))
		Timer_1.PWM_OP_Mode_COM1A_10 = NON_INVERTED;
		Timer_1.PWM_OP_Mode_COM1B_10 = NON_INVERTED;
		Timer_1.OC_Ch_FOC1A_B = CH_A_B;
	#elif (MOTOR_A_SEL == ENABLE)
		Timer_1.PWM_OP_Mode_COM1B_10 = NON_INVERTED;
		Timer_1.OC_Ch_FOC1A_B = CH_B_ONLY;
	#elif (MOTOR_B_SEL == ENABLE)
		Timer_1.PWM_OP_Mode_COM1A_10 = NON_INVERTED;
		Timer_1.OC_Ch_FOC1A_B = CH_A_ONLY;
	#endif

	Timer_1.Clk_Source_CS1_2_0 = CLK_PRESC_8;
	Timer_1.prescalar = PRESC_8;

	Timer_1.Ticks = 0;
	Timer_1.Tick_Time = 0;

	Timer_1.Num_Ovf = 0;
	Timer_1.Init_Value = 0;

	Timer_1.Comp_Value_A = 0;
	Timer_1.Comp_Value_B = 0;

	PWM1_Init();
}

#if MOTOR_A_SEL == ENABLE
	void DCMotor_A_SetDir(DCMotor_Dir DC_Motor_Direction)
	{
		H_BRIDGE_DIR_PINS_PORT->PORT &= ~H_BRIDGE_DIR_MOTOR_A_MASK;
		H_BRIDGE_DIR_PINS_PORT->PORT |= (DC_Motor_Direction << H_BRIDGE_DIR_PIN_1) & H_BRIDGE_DIR_MOTOR_A_MASK;
	}
	void DCMotor_A_Start(void)
	{
		PWM1_Start();
	}
	void DCMotor_A_Stop(void)
	{
		DIO_SetPinValue(H_BRIDGE_DIR_DIO_PORT , H_BRIDGE_DIR_PIN_1 , DIO_PIN_HIGH);
		DIO_SetPinValue(H_BRIDGE_DIR_DIO_PORT , H_BRIDGE_DIR_PIN_2 , DIO_PIN_HIGH);
	}
#endif

#if MOTOR_B_SEL == ENABLE
	void DCMotor_B_SetDir(DCMotor_Dir DC_Motor_Direction)
	{
		H_BRIDGE_DIR_PINS_PORT->PORT &= ~H_BRIDGE_DIR_MOTOR_B_MASK;
		H_BRIDGE_DIR_PINS_PORT->PORT |= ((DC_Motor_Direction << H_BRIDGE_DIR_PIN_3) & H_BRIDGE_DIR_MOTOR_B_MASK);
	}
	void DCMotor_B_Start(void)
	{
		PWM1_Start();
	}
	void DCMotor_B_Stop(void)
	{
		DIO_SetPinValue(H_BRIDGE_DIR_DIO_PORT , H_BRIDGE_DIR_PIN_3, DIO_PIN_HIGH);
		DIO_SetPinValue(H_BRIDGE_DIR_DIO_PORT , H_BRIDGE_DIR_PIN_4, DIO_PIN_HIGH);
	}
#endif

#if ((MOTOR_A_SEL == ENABLE) && (MOTOR_B_SEL == ENABLE))
	void DCMotors_SetSpeed(uint16 Speed_A, uint16 Speed_B)
	{
		if ((Speed_A == 0) || (Speed_B == 0))
		{
			if((Speed_A == 0) && (Speed_B == 0))
				PWM1_Generate(1,1);
			else if(Speed_A == 0)
				PWM1_Generate(Speed_B,1);
			else if(Speed_B == 0)
				PWM1_Generate(1, Speed_A);
		}
		else if((Speed_A == 100) || (Speed_B == 100))
		{
			if((Speed_A == 100) && (Speed_B == 100))
				PWM1_Generate(99,99);
			else if(Speed_A == 100)
				PWM1_Generate(Speed_B,99);
			else if(Speed_B == 100)
				PWM1_Generate(99, Speed_A);
		}
		else
			PWM1_Generate(Speed_B , Speed_A);
	}
#elif (MOTOR_A_SEL == ENABLE)
	void DCMotor_A_SetSpeed(uint16 Speed_A)
	{
		if(Speed_A == 0)
			PWM1_Generate(0,1);
		else if (Speed_A == 100)
			PWM1_Generate(0,99);
		else
			PWM1_Generate(0,Speed_A);
	}
#elif (MOTOR_B_SEL == ENABLE)
	void DCMotor_B_SetSpeed(uint16 Speed_B)
	{
		if(Speed_B == 0)
			PWM1_Generate(1,0);
		else if (Speed_B == 100)
			PWM1_Generate(99,0);
		else
			PWM1_Generate(Speed_B,0);
	}
#endif

#if SERVO_MOTOR_SEL == ENABLE
	void Servo_OVF_CallBack(void)
	{
		static uint8 Ovf_count = 0;
		if(Ovf_count%2 == 0)
		{
			DIO_SetPinValue(SERVO_DIR_DIO_PORT,SERVO_PIN, DIO_PIN_HIGH);
		}
		Ovf_count++;
	}
	void Servo_Comp_CallBack(void)
	{
		static uint8 Compare_count = 0;
		if(Compare_count%2 == 0)
		{
			DIO_SetPinValue(SERVO_DIR_DIO_PORT,SERVO_PIN, DIO_PIN_LOW);
		}
		Compare_count++;
	}
	void Servo_Motor_Init(void)
	{
		DIO_SetPinDir(SERVO_DIR_DIO_PORT, SERVO_PIN, DIO_PIN_OUTPUT);
		
		Timer_2.OperationType = TIMER_NORM;
		Timer_2.Norm_CTC_Op_Mode = NORMAL_OP;

		Timer_2.Clk_Source_CS2 = CLOCK_PRESC_1024;
		Timer_2.Reg_Size = REG_SI_8;
		Timer_2.prescalar = PRESC_1024;

		Timer_2.Ticks = 0;
		Timer_2.Tick_Time = 0;
		Timer_2.PWM_REQ_FREQ = SERVO_OP_FREQ; // Servo Motor Require frequency of 50Hz to work
		Timer_2.Num_Ovf = 0;

		Timer_2.Init_Value = 0;

		Timer_2.Comp_Value = 0;
		
		Timer2_Init();

		Set_Timer2_OVF_CallBack(&Servo_OVF_CallBack);
		Set_Timer2_COMP_CallBack(&Servo_Comp_CallBack);
	}
	
	void Servo_Motor_Move(sint8 Degrees)
	{
		// set the duty cycle of the Servo motor connected on Timer2-OC2 Pin //
		Timer_2.Duty_Cycle = (float32)(SERVO_ANGLE_OFFSET + Degrees) ;
		Timer_2.Duty_Cycle *= SERVO_SLOPE;
		Timer_2.Duty_Cycle += (SERVO_MIN_DUTY_CYCLE);
		
		Timer2_SetDelay(SERVO_SIGNAL_PERIODIC_TIME);
		Timer_2.Comp_Value = 2 * (Timer_2.Duty_Cycle / 100) * (Timer_2.Ticks - Timer_2.Init_Value);
		Timer_2.Comp_Value += Timer_2.Init_Value;
		Timer2_SetDelay(SERVO_SIGNAL_PERIODIC_TIME);
		Timer2_Start();
	}
#endif