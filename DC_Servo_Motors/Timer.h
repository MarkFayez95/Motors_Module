/*
 * Timer.h
 *
 * Created: 09/04/2021 1:40:49 PM
 *  Author: Mark Fayez
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "Bit_math.h"
#include "MCU.h"
#include "STD_DTypes.h"
#include "Timer_Structures.h"
#include "Timer_CFG.h"

/******************Timers Configuration options******************/
#define	PRESC_OFF	1
#define	PRESC_8		8
#define	PRESC_32	32
#define	PRESC_64	64
#define	PRESC_128	128
#define	PRESC_256	256
#define	PRESC_1024	1024

#define REG_SI_8	8
#define REG_SI_9	9
#define REG_SI_10	10
#define REG_SI_16	16
#define REG_SI_24	24
#define REG_SI_32	32

#define ENABLE	1
#define DISABLE 0

/****************** Timers Constrains / Settings ******************/

#define TIMER2_PH_CO_MAX_TICKS		(2*Max_Ticks(Timer_2.Reg_Size))
#define TIMER2_FAST_PWM_MAX_TICKS	Max_Ticks(Timer_2.Reg_Size)

/****************** Timers Constrains / Bits Masks ******************/
#define Max_Ticks(Reg_Size)			((uint32)1<<Reg_Size)
#define Tick_Time_Calc(Presc,Clk)   (float64)(Presc/Clk)

#define WGM0_MASK   0x48
#define WAV_GEN_0_MASK(Wave_Gen_mode)   (((Wave_Gen_mode<<(WGM01-1))|(Wave_Gen_mode<<WGM00)) & WGM0_MASK)
#define CS0_2_0_MASK    0x07
#define COM0_MASK       0x30

#define WGM1_10_MASK     0x03
#define WGM1_32_MASK     0x18

#define COM1A_10_MASK   0xA0
#define COM1B_10_MASK   0x30
#define CS1_2_0_MASK    0x07

#define WGM2_MASK		0b01001000
#define WAV_GEN_2_MASK(Wave_Gen_mode)   ((((Wave_Gen_mode & 0b10)<<(WGM21-1))|((Wave_Gen_mode & 0b01)<<WGM20)) & WGM2_MASK)

#define COM2_MASK		0b00110000
#define CS2_2_0_MASK	0b00000111

/******************Timer 0******************/
void Timer0_Init(void);
void Timer0_SetDelay(uint32 Delay_Ms);
void Timer0_Start(void);
void Timer0_Stop(void);
#if TIMER0_GEN_INT == ENABLE
	#if TIMER0_OVF_INT == ENABLE
		void Set_Timer0_OVF_CallBack(void (*P)(void));
	#endif
	#if TIMER0_COMP_INT == ENABLE
		void Set_Timer0_COMP_CallBack(void (*P)(void));
	#endif
#endif

/******************Timer 1******************/
void Timer1_Init(void);
void Timer1_SetDelay(uint32 Delay_Ms);
void Timer1_Start(void);
void Timer1_Stop(void);
#if TIMER1_GEN_INT == ENABLE
	#if TIMER1_OVF_INT == ENABLE
		void Set_Timer1_OVF_CallBack(void (*P)(void));
	#endif
	#if TIMER1_COMP_A_INT == ENABLE
		void Set_Timer1_COMPA_CallBack(void (*P)(void));
	#endif
	#if TIMER1_COMP_B_INT == ENABLE
		void Set_Timer1_COMPB_CallBack(void (*P)(void));
	#endif
#endif

/******************Timer 2******************/
void Timer2_Init(void);
void Timer2_SetDelay(uint32 Delay_Ms);
void Timer2_Start(void);
void Timer2_Stop(void);
#if TIMER2_GEN_INT == ENABLE
	#if TIMER2_OVF_INT == ENABLE
		void Set_Timer2_OVF_CallBack(void (*P)(void));
	#endif
	#if TIMER2_COMP_INT == ENABLE
		void Set_Timer2_COMP_CallBack(void (*P)(void));
	#endif
#endif

/******************PWM 0******************/
void PWM0_Init(void);
void PWM0_Generate(uint16 Duty_Cycle);
void PWM0_Start(void);
void PWM0_Stop(void);

/******************PWM 1******************/
void PWM1_Init(void);
void PWM1_Generate(uint16 Duty_Cycle_A, uint16 Duty_Cycle_B);
void PWM1_Start(void);
void PWM1_Stop(void);

/******************PWM 2******************/
void PWM2_Init(void);
void PWM2_Generate(uint16 Duty_Cycle);
void PWM2_Start(void);
void PWM2_Stop(void);

#endif /* TIMER_H_ */