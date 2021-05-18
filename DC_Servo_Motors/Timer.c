/*
 * Timer.c
 *
 * Created: 09/04/2021 1:40:40 PM
 *  Author: Mark Fayez
 */ 


#include "Timer.h"
#include <avr/interrupt.h>

/******************Timer 0******************/
/*
TIMER0_CONF	Timer_0; 

Timer_0.OperationType = TIMER_NORM;
Timer_0.Norm_CTC_Op_Mode = NORMAL;
Timer_0.PWM_Op_Mode = NORMAL;

Timer_0.Clk_Source_CS0 = CLK_PRESC_1024;
Timer_0.Reg_Size = REG_SI_8;
Timer_0.prescalar = PRESC_1024;

Timer_0.Ticks = 0;
Timer_0.Tick_Time = 0;

Timer_0.Num_Ovf = 0;

Timer_0.Init_Value = 0;

Timer_0.Comp_Value = 0;
Timer_0.CallBack_Fun_OVF = NULL;
Timer_0.CallBack_Fun_COM = NULL;
*/
volatile TIMER0_CONF	Timer_0; 

void Timer0_Init(void)
{
	// Set Timer Mode //
	TCCR0 |= WAV_GEN_0_MASK(Timer_0.OperationType);
	
	// Set the CTC mode of operation //
	TCCR0 |= Timer_0.Norm_CTC_Op_Mode & COM0_MASK;
	
	// Initialize the Timer structure' constant values.
	Timer_0.Ticks = Max_Ticks(Timer_0.Reg_Size);
	Timer_0.Tick_Time = Tick_Time_Calc(Timer_0.prescalar,CLK_FREQ);
	
	#if TIMER0_GEN_INT == ENABLE
		#if TIMER0_OVF_INT == ENABLE
			// Enable OverFlow interrupts for Timer0 //
			SetBit(TIMSK, TOIE0);
		#endif
		#if TIMER0_COMP_INT == ENABLE
			// Enable compare interrupts for Timer0 //
			SetBit(TIMSK,OCIE0);
		#endif

	// Enable Global interrupts //
	SetBit(SREG,SREG_I);
	#endif
}
void Timer0_SetDelay(uint32 Delay_Ms)
{
	if(Timer_0.OperationType == TIMER_NORM)
	{
		uint32 Num_of_Ticks = 0;
		Num_of_Ticks = (uint32)((float32)(Delay_Ms * 1000) / Timer_0.Tick_Time);
		Timer_0.Num_Ovf = Num_of_Ticks / Timer_0.Ticks;
		Timer_0.Init_Value = Timer_0.Ticks - (Num_of_Ticks % Timer_0.Ticks );
		TCNT0 = Timer_0.Init_Value;
		
		if(Timer_0.Init_Value != 0)
		{
			Timer_0.Num_Ovf ++;
		}
		#if TIMER0_COMP_INT == ENABLE
		OCR0 = Timer_0.Comp_Value;
		#endif
	}
	else
	{
		OCR0 = Timer_0.Comp_Value;
		uint32 Num_of_Ticks = 0;
		Num_of_Ticks = (uint32)((float32)(Delay_Ms * 1000) / Timer_0.Tick_Time);
		Timer_0.Num_Ovf = Num_of_Ticks / Timer_0.Comp_Value;
		Timer_0.Init_Value = Timer_0.Comp_Value - (Num_of_Ticks % Timer_0.Comp_Value );
		TCNT0 = Timer_0.Init_Value;
		
		if(Timer_0.Init_Value != 0)
		{
			Timer_0.Num_Ovf ++;
		}
	}
}
void Timer0_Start(void)
{
	// Set Clock's source to prescalar 1024 to start timer //
	TCCR0 &= ~CS0_2_0_MASK;
	TCCR0 |= (Timer_0.Clk_Source_CS0 << CS00 ) & CS0_2_0_MASK;
}
void Timer0_Stop(void)
{
	// Clear Clock source to stop timer //
	TCCR0 &= ~CS0_2_0_MASK;
}
#if TIMER0_GEN_INT == ENABLE
	#if TIMER0_OVF_INT == ENABLE
		void Set_Timer0_OVF_CallBack(void (*P)(void))
		{
			Timer_0.CallBack_Fun_OVF = P;
		}
		ISR(TIMER0_OVF_vect)
		{
			static uint32 Ovf_Count = 0;
			Ovf_Count++;
			
			if(Ovf_Count == Timer_0.Num_Ovf)
			{
				Ovf_Count = 0;
				(*Timer_0.CallBack_Fun_OVF)();
				TCNT0 = Timer_0.Init_Value;
			}
		}
	#endif
	#if TIMER0_COMP_INT == ENABLE
		void Set_Timer0_COMP_CallBack(void (*P)(void))
		{
			Timer_0.CallBack_Fun_COMP = P;
		}
		ISR(TIMER0_COMP_vect)
		{
			if(Timer_0.OperationType != TIMER_CTC)
			{
				(*Timer_0.CallBack_Fun_COMP)();
			}
			else 
			{
				static uint32 Timer0_Comp_Count = 0;
				Timer0_Comp_Count++;
				
				if(Timer0_Comp_Count == Timer_0.Num_Ovf)
				{
					Timer0_Comp_Count = 0;
					(*Timer_0.CallBack_Fun_COMP)();
					TCNT0 = Timer_0.Init_Value;
				}
			}
		}
	#endif
#endif

/******************PWM 0******************/
void PWM0_Init(void)
{
	// Set OC0 Pin Direction Output //
	SetBit(PWM0_PORT_DIR_REG, PWM0_OC0_PIN);
	
	// Set PWM operation type
	TCCR0 |= WAV_GEN_0_MASK(Timer_0.OperationType);

	// Set PWM operation Mode //
	TCCR0 |= (Timer_0.PWM_Op_Mode << COM01) & COM0_MASK;
	
	#if TIMER0_GEN_INT == ENABLE
		#if TIMER0_OVF_INT == ENABLE
			// Enable OverFlow interrupts for Timer0 //
			SetBit(TIMSK, TOIE0);
		#endif
		#if TIMER0_COMP_INT == ENABLE
			// Enable compare interrupts for Timer0 //
			SetBit(TIMSK,OCIE0);
		#endif

		// Enable Global interrupts //
		SetBit(SREG,SREG_I);
	#endif
}
void PWM0_Generate(uint16 Duty_Cycle)
{
	if(Timer_0.OperationType == PWM_PH_CORRECT)
	{
		if(Timer_0.PWM_Op_Mode == CLR_SET)
		{
			OCR0 = (Duty_Cycle * Timer_0.Ticks) / 100;
		}
		else if(Timer_0.PWM_Op_Mode == SET_CLR)
		{
			OCR0 = Timer_0.Ticks - ((Duty_Cycle * Timer_0.Ticks) / 100);
		}
	}
	else if(Timer_0.OperationType == PWM_FAST)
	{
		if(Timer_0.PWM_Op_Mode == CLR_SET)
		{
			OCR0 = ((Duty_Cycle * Timer_0.Ticks) / 100) - 1;
		}
		else if(Timer_0.PWM_Op_Mode == SET_CLR)
		{
			OCR0 = Timer_0.Ticks - (((Duty_Cycle * Timer_0.Ticks) / 100) + 1);
		}
	}
	
}
void PWM0_Start(void)
{
	// Set Clock's source to start PWM //
	TCCR0 &= ~CS0_2_0_MASK;
	TCCR0 |= (Timer_0.Clk_Source_CS0 << CS00 ) & CS0_2_0_MASK;
}
void PWM0_Stop(void)
{
	// Clear Clock source to stop PWM //
	TCCR0 &= ~CS0_2_0_MASK;
}

/******************Timer 1******************/
/*
TIMER1_CONF	Timer_1;

Timer_1.OperationType_WGM1_32 =CTC_PWM_FAST_MODE;

Timer_1.Norm_CTC_OP_Mode_COM1A_10 = CTC_CLEAR;
Timer_1.Norm_CTC_OP_Mode_COM1B_10 = NORMAL;

Timer_1.OC_Ch_FOC1A_B = CH_A_ONLY;
Timer_1.CUSTOME_TOP_ICR1 = 0;

Timer_1.Clk_Source_CS1_2_0 = CLK_PRESC_256;
Timer_1.Reg_Size = REG_SI_16;
Timer_1.prescalar = PRESC_256;

Timer_1.Ticks = 0;
Timer_1.Tick_Time = 0;

Timer_1.Num_Ovf = 0;
Timer_1.Init_Value = 0;

Timer_1.Comp_Value_A = 0;
Timer_1.Comp_Value_B = 0;

Timer_1.CallBack_Fun_OVF = NULL;
Timer_1.CallBack_Fun_COMPA = NULL;
Timer_1.CallBack_Fun_COMPB = NULL;
*/
volatile TIMER1_CONF	Timer_1;

void Timer1_Init(void)
{
	// Set Timer Operation type
	TCCR1B &= ~WGM1_32_MASK;
	TCCR1B |= (Timer_1.OperationType_WGM1_32 << WGM12) & WGM1_32_MASK;
	/***********WGM1_10 is always zero in Normal and CTC modes*************/
	TCCR1A &= ~(WGM1_10_MASK);

	// Set the CTC Mode
	TCCR1A &= ~COM1A_10_MASK;
	TCCR1A |= (Timer_1.Norm_CTC_OP_Mode_COM1A_10 << COM1A0) & COM1A_10_MASK;
	
	// Initialize the Timer structure' constant values.
	Timer_1.Ticks = Max_Ticks(Timer_1.Reg_Size);
	Timer_1.Tick_Time = Tick_Time_Calc(Timer_1.prescalar,CLK_FREQ);
	
	
	#if TIMER1_GEN_INT == ENABLE
		#if TIMER1_OVF_INT == ENABLE
			// Enable OverFlow interrupts for Timer1 //
			SetBit(TIMSK, TOIE1);
		#endif
		#if TIMER1_COMP_INT == ENABLE
			if(Timer_1.OC_Ch_FOC1A_B == CH_A_ONLY)
			{
				// Enable compare interrupts for Timer1 COM1A value//
				SetBit(TIMSK,OCIE1A);
			}
			else if (Timer_1.OC_Ch_FOC1A_B == CH_B_ONLY)
			{
				// Enable compare interrupts for Timer1 COM1B value//
				SetBit(TIMSK,OCIE1B);
			}
			else if (Timer_1.OC_Ch_FOC1A_B == CH_A_B)
			{
				// Enable compare interrupts for Timer0 COM1B value//
				TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);
			}
		#endif

		// Enable Global interrupts //
		SetBit(SREG,SREG_I);
	#endif
	
}
void Timer1_SetDelay(uint32 Delay_Ms)
{
	if(Timer_1.OperationType_WGM1_32 == NORM_PWM_PH_CO_TYPE) // Normal timer mode --> Top at max of TCN
	{
		uint32 Num_of_Ticks = ((Delay_Ms * 1000) / Timer_1.Tick_Time);
		Timer_1.Num_Ovf = Num_of_Ticks / Timer_1.Ticks;
		Timer_1.Init_Value = Timer_1.Ticks - (Num_of_Ticks % Timer_1.Ticks );
		TCNT1_VAL = Timer_1.Init_Value;
		
		if(Timer_1.Init_Value != 0)
			Timer_1.Num_Ovf ++;
	}
	else if(Timer_1.OperationType_WGM1_32 == CTC_PWM_FAST_TYPE) // CTC Timer_1-Mode 4 --> Top is the value of OCR1A 
	{
		uint32 Num_of_Ticks = ((Delay_Ms * 1000) / Timer_1.Tick_Time);
		Timer_1.Num_Ovf = Num_of_Ticks / Timer_1.Comp_Value_A;
		Timer_1.Init_Value = Timer_1.Comp_Value_A - (Num_of_Ticks % Timer_1.Comp_Value_A );
		TCNT1_VAL = Timer_1.Init_Value;
		OCR1A_VAL = Timer_1.Comp_Value_A;
		
		if(Timer_1.Init_Value != 0)
			Timer_1.Num_Ovf ++;
	}
	else if(Timer_1.OperationType_WGM1_32 == CTC_PWM_FAST_MODE) // CTC Timer_1-Mode 12 --> Top is the value of ICR1
	{
		uint32 Num_of_Ticks = ((Delay_Ms * 1000) / Timer_1.Tick_Time);
		Timer_1.Num_Ovf = Num_of_Ticks / Timer_1.CUSTOME_TOP_ICR1;
		Timer_1.Init_Value = Timer_1.CUSTOME_TOP_ICR1 - (Num_of_Ticks % Timer_1.CUSTOME_TOP_ICR1 );
		TCNT1_VAL = Timer_1.Init_Value;
		ICR1_VAL = Timer_1.CUSTOME_TOP_ICR1;
		
		if(Timer_1.Init_Value != 0)
			Timer_1.Num_Ovf ++;
	}
}
void Timer1_Start(void)
{
	// Set Clock's source to start timer //
	TCCR1B &= ~CS1_2_0_MASK;
	TCCR1B |= (Timer_1.Clk_Source_CS1_2_0 << CS10) & CS1_2_0_MASK;
}
void Timer1_Stop(void)
{
	// Clear Clock source to stop timer //
	TCCR1B &= ~CS1_2_0_MASK;
}

#if TIMER1_GEN_INT == ENABLE
	#if TIMER1_OVF_INT == ENABLE
		void Set_Timer0_OVF_CallBack(void (*P)(void))
		{
			Timer_1.CallBack_Fun_OVF = P;
		}
		ISR(TIMER0_OVF_vect)
		{
			static uint32 Ovf_Count = 0;
			Ovf_Count++;
			
			if(Ovf_Count == Timer_1.Num_Ovf)
			{
				Ovf_Count = 0;
				(*Timer_1.CallBack_Fun_OVF)();
				TCNT0 = Timer_1.Init_Value;
			}
		}
	#endif
	#if TIMER1_COMP_A_INT == ENABLE
		void Set_Timer1_COMPA_CallBack(void (*P)(void))
		{
			Timer_1.CallBack_Fun_COMPA = P;
		}
		ISR(TIMER1_COMPA_vect)
		{
			(*Timer_1.CallBack_Fun_COMPA)();
		}
	#endif
	#if TIMER1_COMP_B_INT == ENABLE
		void Set_Timer1_COMPB_CallBack(void (*P)(void))
		{
			Timer_1.CallBack_Fun_COMPB = P;
		}
		ISR(TIMER1_COMPB_vect)
		{
			(*Timer_1.CallBack_Fun_COMPB)();
		}
	#endif
#endif

/******************PWM 1******************/
void PWM1_Init(void)
{
	// Set Timer_1 Operation //
	TCCR1B &= ~WGM1_32_MASK;
	TCCR1B |= (Timer_1.OperationType_WGM1_32 << WGM12) & WGM1_32_MASK;
	
	// Set Timer_1 type and mode //
	if(Timer_1.OperationType_WGM1_32 == NORM_PWM_PH_CO_TYPE)
	{
		TCCR1A &= ~WGM1_10_MASK;
		TCCR1A |= (Timer_1.Operation_Ph_Co_Type_WGM1_10 << WGM10) & WGM1_10_MASK;
		switch(Timer_1.Operation_Ph_Co_Type_WGM1_10)
		{
			case PWM_PH_CO_8_BIT:
				Timer_1.Reg_Size = REG_SI_8;
			break;
			case PWM_PH_CO_9_BIT:
				Timer_1.Reg_Size = REG_SI_9;
			break;
			case PWM_PH_CO_10_BIT:
				Timer_1.Reg_Size = REG_SI_10;
			break;
			
			default:
			break;
		}
	}
	else if(Timer_1.OperationType_WGM1_32 == CTC_PWM_FAST_TYPE)
	{
		TCCR1A &= ~WGM1_10_MASK;
		TCCR1A |= (Timer_1.Operation_Fast_Type_WGM1_10 << WGM10) & WGM1_10_MASK;
		switch(Timer_1.Operation_Fast_Type_WGM1_10)
		{
			case FAST_8_BIT:
			Timer_1.Reg_Size = REG_SI_8;
			break;
			case FAST_9_BIT:
			Timer_1.Reg_Size = REG_SI_9;
			break;
			case FAST_10_BIT:
			Timer_1.Reg_Size = REG_SI_10;
			break;
			default:
			break;
		}
	}
	else if(Timer_1.OperationType_WGM1_32 == PWM_PH_CO_MODE)
	{
		TCCR1A &= ~WGM1_10_MASK;
		TCCR1A |= (Timer_1.Operation_Ph_Co_Mod_WGM1_10 << WGM10) & WGM1_10_MASK;
	}
	else
	{
		TCCR1A &= ~WGM1_10_MASK;
		TCCR1A |= (Timer_1.Operation_Fast_Mod_WGM1_10 << WGM10) & WGM1_10_MASK;
		
		// Set Value of ICR1 //
		ICR1 = Timer_1.CUSTOME_TOP_ICR1;
	}
	
	// Set PWM Mode (Inverted - Non-Inverted) //
	if(Timer_1.OC_Ch_FOC1A_B == CH_A_ONLY)
	{
		// Set OC1A Pin Direction Output //
		SetBit(PWM1_A_PORT_DIR_REG,PWM1_A_OC1A_PIN);
		
		TCCR1A &= ~COM1A_10_MASK;
		TCCR1A |= (Timer_1.PWM_OP_Mode_COM1A_10 << COM1A0) & COM1A_10_MASK;
	}
	else if(Timer_1.OC_Ch_FOC1A_B == CH_B_ONLY)
	{
		// Set OC1B Pin Direction Output //
		SetBit(PWM1_B_PORT_DIR_REG,PWM1_B_OC1B_PIN);
		
		TCCR1A &= ~COM1B_10_MASK;
		TCCR1A |= (Timer_1.PWM_OP_Mode_COM1B_10 << COM1B0) & COM1B_10_MASK;
	}
	else
	{
		// Set OC1A Pin Direction Output //
		SetBit(PWM1_A_PORT_DIR_REG,PWM1_A_OC1A_PIN);
		
		// Set OC1B Pin Direction Output //
		SetBit(PWM1_A_PORT_DIR_REG,PWM1_B_OC1B_PIN);
		
		TCCR1A &= ~COM1A_10_MASK & ~COM1B_10_MASK;
		TCCR1A |= ((Timer_1.PWM_OP_Mode_COM1A_10 << COM1A0) & COM1A_10_MASK) | ((Timer_1.PWM_OP_Mode_COM1B_10 << COM1B0) & COM1B_10_MASK);
	}

	// Initialize the Timer structure' constant values.
	Timer_1.Ticks = Max_Ticks(Timer_1.Reg_Size);
	Timer_1.Tick_Time = Tick_Time_Calc(Timer_1.prescalar,CLK_FREQ);
	
	#if TIMER1_GEN_INT == ENABLE
		#if TIMER1_OVF_INT == ENABLE
			// Enable OverFlow interrupts for Timer1 //
			SetBit(TIMSK, TOIE1);
		#endif
		#if TIMER1_COMP_INT == ENABLE
			if(Timer_1.OC_Ch_FOC1A_B == CH_A_ONLY)
			{
				// Enable compare interrupts for Timer1 COM1A value//
				SetBit(TIMSK,OCIE1A);
			}
			else if (Timer_1.OC_Ch_FOC1A_B == CH_B_ONLY)
			{
				// Enable compare interrupts for Timer1 COM1B value//
				SetBit(TIMSK,OCIE1B);
			}
			else if (Timer_1.OC_Ch_FOC1A_B == CH_A_B)
			{
				// Enable compare interrupts for Timer0 COM1B value//
				TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);
			}
		#endif

	// Enable Global interrupts //
	SetBit(SREG,SREG_I);
	#endif

}
void PWM1_Generate(uint16 Duty_Cycle_A, uint16 Duty_Cycle_B)
{
	if ((Timer_1.OC_Ch_FOC1A_B == CH_A_ONLY) || (Timer_1.OC_Ch_FOC1A_B == CH_A_B))
	{
		switch(Timer_1.OperationType_WGM1_32)
		{
			case NORM_PWM_PH_CO_TYPE:
				if(Timer_1.Operation_Ph_Co_Type_WGM1_10 > 0)
				{
					switch(Timer_1.PWM_OP_Mode_COM1A_10)
					{
						case TOGGLE:
						case NON_INVERTED:
							Timer_1.Comp_Value_A = (Timer_1.Ticks * Duty_Cycle_A) / 100;
						break;
					
						case INVERTED:
							Timer_1.Comp_Value_A = Timer_1.Ticks - ((Timer_1.Ticks * Duty_Cycle_A) / 100);
						break;
					
						default:
						break;
					}
				}
			break;
		
			case CTC_PWM_FAST_TYPE:
				if(Timer_1.Operation_Fast_Type_WGM1_10 > 0)
				{
					switch(Timer_1.PWM_OP_Mode_COM1A_10)
					{
						case TOGGLE:
							Timer_1.Comp_Value_A = Timer_1.Ticks/2;
						break;
						case NON_INVERTED:
							Timer_1.Comp_Value_A = ((Timer_1.Ticks * Duty_Cycle_A) / 100) - 1;
						break;
					
						case INVERTED:
							Timer_1.Comp_Value_A = Timer_1.Ticks - (((Timer_1.Ticks * Duty_Cycle_A) / 100) - 1);
						break;
					
						default:
						break;
					}
				
				}
			break;
		
			case PWM_PH_CO_MODE:
				if(Timer_1.Operation_Ph_Co_Mod_WGM1_10 % 2 == 0)
				{
					switch(Timer_1.PWM_OP_Mode_COM1A_10)
					{
						case TOGGLE:
						case NON_INVERTED:
							Timer_1.Comp_Value_A = (Timer_1.CUSTOME_TOP_ICR1 * Duty_Cycle_A) / 100;
						break;
					
						case INVERTED:
							Timer_1.Comp_Value_A = Timer_1.CUSTOME_TOP_ICR1 - ((Timer_1.CUSTOME_TOP_ICR1 * Duty_Cycle_A) / 100);
						break;
					
						default:
						break;
					}
				
				}
			break;
		
			case CTC_PWM_FAST_MODE:
				if(Timer_1.Operation_Fast_Type_WGM1_10 == 2)
				{
					switch(Timer_1.PWM_OP_Mode_COM1A_10)
					{
						case TOGGLE:
						case NON_INVERTED:
							Timer_1.Comp_Value_A = ((Timer_1.CUSTOME_TOP_ICR1 * Duty_Cycle_A) / 100) - 1;
						break;
					
						case INVERTED:
							Timer_1.Comp_Value_A = Timer_1.CUSTOME_TOP_ICR1 - (((Timer_1.CUSTOME_TOP_ICR1 * Duty_Cycle_A) / 100) - 1);
						break;
					
						default:
						break;
					}
				}
			break;
		
			default:
			break;
		}
		OCR1A_VAL = Timer_1.Comp_Value_A;
		if(Timer_1.OC_Ch_FOC1A_B == CH_A_B)
			OCR1B_VAL = Timer_1.Comp_Value_B;
	}
	if ((Timer_1.OC_Ch_FOC1A_B == CH_B_ONLY) || (Timer_1.OC_Ch_FOC1A_B == CH_A_B))
	{
		switch(Timer_1.OperationType_WGM1_32)
		{
			case NORM_PWM_PH_CO_TYPE:
			switch(Timer_1.PWM_OP_Mode_COM1B_10)
			{
				case NON_INVERTED:
				Timer_1.Comp_Value_B = (Timer_1.Ticks * Duty_Cycle_B) / 100;
				break;
				
				case INVERTED:
				Timer_1.Comp_Value_B = Timer_1.Ticks - ((Timer_1.Ticks * Duty_Cycle_B) / 100);
				break;
				
				default:
				break;
			}
			break;
			
			case CTC_PWM_FAST_TYPE:
			if(Timer_1.Operation_Fast_Type_WGM1_10 > 0)
			{
				switch(Timer_1.PWM_OP_Mode_COM1B_10)
				{
					case NON_INVERTED:
					Timer_1.Comp_Value_B = ((Timer_1.Ticks * Duty_Cycle_B) / 100) - 1;
					break;
					
					case INVERTED:
					Timer_1.Comp_Value_B = Timer_1.Ticks - (((Timer_1.Ticks * Duty_Cycle_B) / 100) - 1);
					break;
					
					default:
					break;
				}
				
			}
			break;
			
			case PWM_PH_CO_MODE:
			if(Timer_1.Operation_Ph_Co_Mod_WGM1_10 % 2 == 0)
			{
				switch(Timer_1.PWM_OP_Mode_COM1B_10)
				{
					case NON_INVERTED:
					Timer_1.Comp_Value_B = (Timer_1.CUSTOME_TOP_ICR1 * Duty_Cycle_B) / 100;
					break;
					
					case INVERTED:
					Timer_1.Comp_Value_B = Timer_1.CUSTOME_TOP_ICR1 - ((Timer_1.CUSTOME_TOP_ICR1 * Duty_Cycle_B) / 100);
					break;
					
					default:
					break;
				}
				
			}
			break;
			
			case CTC_PWM_FAST_MODE:
			if(Timer_1.Operation_Fast_Type_WGM1_10 == 2)
			{
				switch(Timer_1.PWM_OP_Mode_COM1B_10)
				{
					case NON_INVERTED:
					Timer_1.Comp_Value_B = ((Timer_1.CUSTOME_TOP_ICR1 * Duty_Cycle_B) / 100) - 1;
					break;
					
					case INVERTED:
					Timer_1.Comp_Value_B = Timer_1.CUSTOME_TOP_ICR1 - (((Timer_1.CUSTOME_TOP_ICR1 * Duty_Cycle_B) / 100) - 1);
					break;
					
					default:
					break;
				}
			}
			break;
			
			default:
			break;
		}
		OCR1B_VAL = Timer_1.Comp_Value_B;
		if(Timer_1.OC_Ch_FOC1A_B == CH_A_B)
			OCR1A_VAL = Timer_1.Comp_Value_A;
	}
}
void PWM1_Start(void)
{
	// Set Clock's source to start PWM //
	TCCR1B &= ~CS1_2_0_MASK;
	TCCR1B |= (Timer_1.Clk_Source_CS1_2_0 & CS1_2_0_MASK);
}
void PWM1_Stop(void)
{
	// clear Clock's source to stop PWM //
	TCCR1B &= ~CS1_2_0_MASK;
}

/******************Timer 2******************/
volatile TIMER2_CONF Timer_2;
/*
Timer_2.OperationType = TIMER_NORM;
Timer_2.Norm_CTC_Op_Mode = NORMAL;
Timer_2.PWM_Op_Mode = NORMAL;

Timer_2.Clk_Source_CS2 = CLK_PRESC_1024;
Timer_2.Reg_Size = REG_SI_8;
Timer_2.prescalar = PRESC_1024;

Timer_2.Ticks = 0;
Timer_2.Tick_Time = 0;

Timer_2.Num_Ovf = 0;

Timer_2.Init_Value = 0;

Timer_2.Comp_Value = 0;
Timer_2.CallBack_Fun_OVF = NULL;
Timer_2.CallBack_Fun_COMP = NULL;
*/
void Timer2_Init(void)
{
	// Set Timer General Mode //
	TCCR2 = WAV_GEN_2_MASK(Timer_2.OperationType);
	
	// Set the CTC mode of operation //
	TCCR2 |= (Timer_2.Norm_CTC_Op_Mode & COM2_MASK);
	
	// Initialize the Timer structure' constant values.
	Timer_2.Ticks = Max_Ticks(Timer_2.Reg_Size);
	Timer_2.Tick_Time = Tick_Time_Calc(Timer_2.prescalar,CLK_FREQ);
	
	#if TIMER2_GEN_INT == ENABLE
		#if TIMER2_OVF_INT == ENABLE
			// Enable OverFlow interrupts for Timer0 //
			SetBit(TIMSK, TOIE2);
		#endif
		#if TIMER2_COMP_INT == ENABLE
			// Enable compare interrupts for Timer0 //
			SetBit(TIMSK,OCIE2);
		#endif

		// Enable Global interrupts //
		SetBit(SREG,SREG_I);
	#endif
}
void Timer2_SetDelay(uint32 Delay_Ms)
{
	if(Timer_2.OperationType != TIMER_CTC)
	{
		uint32 Num_of_Ticks = 0;
		Num_of_Ticks = (uint32)((float32)(Delay_Ms * 1000) / Timer_2.Tick_Time);
		Timer_2.Num_Ovf = Num_of_Ticks / Timer_2.Ticks;
		Timer_2.Init_Value = Timer_2.Ticks - (Num_of_Ticks % Timer_2.Ticks );
		TCNT2 = Timer_2.Init_Value;
		
		if(Timer_2.Init_Value != 0)
		{
			Timer_2.Num_Ovf ++;
		}
		#if TIMER2_COMP_INT == ENABLE
			OCR2 = Timer_2.Comp_Value;
		#endif
	}
	else
	{
		OCR2 = Timer_2.Comp_Value;
		uint32 Num_of_Ticks = 0;
		Num_of_Ticks = (uint32)((float32)(Delay_Ms * 1000) / Timer_2.Tick_Time);
		Timer_2.Num_Ovf = Num_of_Ticks / Timer_2.Comp_Value;
		Timer_2.Init_Value = Timer_2.Comp_Value - (Num_of_Ticks % Timer_2.Comp_Value );
		TCNT2 = Timer_2.Init_Value;
		
		if(Timer_2.Init_Value != 0)
		{
			Timer_2.Num_Ovf ++;
		}
	}
}
void Timer2_Start(void)
{
	// Set Clock's source to prescalar 1024 to start timer //
	TCCR2 &= ~CS2_2_0_MASK;
	TCCR2 |= (Timer_2.Clk_Source_CS2 << CS20 ) & CS2_2_0_MASK;
}
void Timer2_Stop(void)
{
	// Clear Clock source to stop timer //
	TCCR2 &= ~CS2_2_0_MASK;
}
#if TIMER2_GEN_INT == ENABLE
	#if TIMER2_OVF_INT == ENABLE
		void Set_Timer2_OVF_CallBack(void (*P)(void))
		{
			Timer_2.CallBack_Fun_OVF = P;
		}
		ISR(TIMER2_OVF_vect)
		{
			static uint32 Timer2_Ovf_Count = 0;
			Timer2_Ovf_Count++;
			
			if(Timer2_Ovf_Count == Timer_2.Num_Ovf)
			{
				Timer2_Ovf_Count = 0;
				(*Timer_2.CallBack_Fun_OVF)();
				TCNT2 = Timer_2.Init_Value;
			}
		}
	#endif
	#if TIMER2_COMP_INT == ENABLE
		void Set_Timer2_COMP_CallBack(void (*P)(void))
		{
			Timer_2.CallBack_Fun_COMP = P;
		}
		ISR(TIMER2_COMP_vect)
		{
			if(Timer_2.OperationType == TIMER_NORM)
			{
				(*Timer_2.CallBack_Fun_COMP)();
			}
			else
			{
				static uint32 Timer2_Comp_Count = 0;
				Timer2_Comp_Count++;
				
				if(Timer2_Comp_Count == Timer_2.Num_Ovf)
				{
					Timer2_Comp_Count = 0;
					(*Timer_2.CallBack_Fun_COMP)();
					TCNT2 = Timer_2.Init_Value;
				}
			}
		}
	#endif
#endif

/******************PWM 2******************/
void PWM2_Init(void)
{
	// Set OC2 Pin Direction Output //
	SetBit(PWM2_PORT_DIR_REG, PWM2_OC2_PIN);
	
	// Set PWM operation type
	TCCR2 = WAV_GEN_2_MASK(Timer_2.OperationType);

	// Set PWM operation Mode //
	TCCR2 |= (Timer_2.PWM_Op_Mode << COM20) & COM2_MASK;

	// Set Max Ticks
	Timer_2.Ticks = Max_Ticks(Timer_2.Reg_Size);
	
	#if TIMER2_GEN_INT == ENABLE
		#if TIMER2_OVF_INT == ENABLE
			// Enable OverFlow interrupts for Timer0 //
			SetBit(TIMSK, TOIE2);
		#endif
		#if TIMER2_COMP_INT == ENABLE
			// Enable compare interrupts for Timer0 //
			SetBit(TIMSK,OCIE2);
		#endif

		// Enable Global interrupts //
		SetBit(SREG,SREG_I);
	#endif
}
void PWM2_Generate(uint16 Duty_Cycle)
{
	if(Timer_2.OperationType == PWM_PH_CORRECT)
	{
		if(Timer_2.PWM_Op_Mode == NON_INVERTED)
		{
			Timer_2.Comp_Value = (Duty_Cycle * Timer_2.Ticks) / 100;
		}
		else if(Timer_2.PWM_Op_Mode == INVERTED)
		{
			Timer_2.Comp_Value = Timer_2.Ticks - ((Duty_Cycle * Timer_2.Ticks) / 100);
		}
		OCR2 = Timer_2.Comp_Value;
	}
	else if(Timer_2.OperationType == PWM_FAST)
	{
		if(Timer_2.PWM_Op_Mode == NON_INVERTED)
		{
			Timer_2.Comp_Value = ((Duty_Cycle * Timer_2.Ticks) / 100) - 1;
		}
		else if(Timer_2.PWM_Op_Mode == INVERTED)
		{
			Timer_2.Comp_Value = Timer_2.Ticks - (((Duty_Cycle * Timer_2.Ticks) / 100) + 1);
		}
		OCR2 = Timer_2.Comp_Value;
	}
}
void PWM2_Start(void)
{
	// Set Clock's source to start PWM //
	TCCR2 &= ~CS2_2_0_MASK;
	TCCR2 |= (Timer_2.Clk_Source_CS2 << CS20 ) & CS2_2_0_MASK;
}
void PWM2_Stop(void)
{
	// Clear Clock source to stop PWM //
	TCCR2 &= ~CS2_2_0_MASK;
}