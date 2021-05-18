/*
 * Timer_Dtypes.h
 *
 * Created: 17/04/2021 09:35:39 PM
 *  Author: Mark Fayez
 */ 

#ifndef TIMER_STRUCTURES_H_
#define TIMER_STRUCTURES_H_

#include "Bit_math.h"
#include "MCU.h"

//*********** Genral Timer SettingS / Options ***********//
typedef enum
{
    NORMAL_OP,
    CTC_TOGGLE,
    CTC_CLEAR,
    CTC_SET
} CTC_OP;

typedef enum
{
    NO_CLK,
    CLK_NO_PRESC,
    CLK_PRESC_8,
    CLK_PRESC_64,
    CLK_PRESC_256,
    CLK_PRESC_1024,
    EXT_CLK_FALLING_EDGE,
    EXT_CLK_RISSING_EDGE
} CLK_SEL;

//*********** Special for Timer_0 Setting Options ***********//
typedef enum
{
    TIMER_NORM,
    PWM_PH_CORRECT,
    TIMER_CTC,
    PWM_FAST
} OP_TYPES;

typedef enum
{
    NORMAL,         // COM_10 Mode selection
    RESERVED,       // COM_10 Mode selection
    CLR_SET,        // COM_10 Mode selection
    SET_CLR         // COM_10 Mode selection
} PWM_MOD;

//*********** Special for Timer_1 Setting Options ***********//
typedef enum
{
    NORM_PWM_PH_CO_TYPE,    // WGM1_32 Modes 0-3
    CTC_PWM_FAST_TYPE,      // WGM1_32 Modes 4-7
    PWM_PH_CO_MODE,         // WGM1_32 Modes 8-11
    CTC_PWM_FAST_MODE       // WGM1_32 Modes 12-15
} WAVE_GEN_MAIN_SETS;

typedef enum
{
    //NORMAL,           // WGM1_10 Mode 0
    PWM_PH_CO_8_BIT=1,  // WGM1_10 Mode 1
    PWM_PH_CO_9_BIT,    // WGM1_10 Mode 2
    PWM_PH_CO_10_BIT    // WGM1_10 Mode 3
} PWM_PH_CO_TYPES;

typedef enum
{
    //CTC_TOP_OCR1A,    // WGM1_10 Mode 4
    FAST_8_BIT=1,       // WGM1_10 Mode 5
    FAST_9_BIT,         // WGM1_10 Mode 6
    FAST_10_BIT         // WGM1_10 Mode 7
} PWM_FAST_TYPES;


typedef enum
{
    TOP_ICR1_UPDATE_ON_BOTTOM,      // WGM1_10 Mode 8
    TOP_OCR1A_UPDATE_ON_BOTTOM,     // WGM1_10 Mode 9
    TOP_ICR1_UPDATE_ON_TOP,         // WGM1_10 Mode 10
    TOP_OCR1A_UPDATE_ON_TOP         // WGM1_10 Mode 11
} PWM_PH_CO_MODES;

typedef enum
{
    //CTC_TOP_ICR1,   // WGM1_10 Mode 12
    //RESERVED,       // WGM1_10 Mode 13
    TOP_ICR1=2,       // WGM1_10 Mode 14
    TOP_OCR1A         // WGM1_10 Mode 15
} PWM_FAST_modes;

typedef enum
{
    //NORMAL,
    TOGGLE=1,     // COM1A/COM1B_10 Mode selection
    NON_INVERTED,    // COM1A/COM1B_10 Mode selection CLR_SET
    INVERTED     // COM1A/COM1B_10 Mode selection SET_CLR
} PWM_OP;

typedef enum
{
	NO_A_NO_B, // FOC1A = 0 & FOC1B = 0
	CH_B_ONLY, // FOC1A = 0 & FOC1B = 1
	CH_A_ONLY, // FOC1A = 1 & FOC1B = 0
	CH_A_B // Both channels are active	
}COMP_CH;

//*********** Special for Timer_2 Setting Options ***********//
typedef enum
{
	NO_CLOCK,
	CLOCK_NO_PRESC,
	CLOCK_PRESC_8,
	CLOCK_PRESC_32,
	CLOCK_PRESC_64,
	CLOCK_PRESC_128,
	CLOCK_PRESC_256,
	CLOCK_PRESC_1024
} CLK_SEL_TIMER_2;

//*********** Timers Configuration Structures ***********//
typedef struct 
{
    volatile OP_TYPES OperationType;
    volatile CTC_OP Norm_CTC_Op_Mode;
	volatile PWM_MOD PWM_Op_Mode;

    volatile CLK_SEL Clk_Source_CS0;

    volatile uint8   Reg_Size;
    volatile uint16  prescalar;
    volatile uint32  Ticks;
    volatile float64  Tick_Time;
    
	volatile uint32 PWM_REQ_FREQ;

    volatile uint32  Num_Ovf;
    volatile uint8  Init_Value;

    volatile uint8  Comp_Value;
	volatile float64 Duty_Cycle;

    void (*CallBack_Fun_OVF)(void);
	void (*CallBack_Fun_COMP)(void);
} TIMER0_CONF;

typedef struct 
{
    volatile WAVE_GEN_MAIN_SETS  OperationType_WGM1_32;
	/***********WGM1_10 is always zero in Normal and CTC modes*************/ 
    
	volatile PWM_PH_CO_TYPES Operation_Ph_Co_Type_WGM1_10;  // WGM1_10 Modes 0-3
    volatile PWM_FAST_TYPES  Operation_Fast_Type_WGM1_10; // WGM1_10 Modes 4-7
	
    volatile PWM_PH_CO_MODES Operation_Ph_Co_Mod_WGM1_10; // WGM1_10 Modes 8-11
	volatile PWM_FAST_modes  Operation_Fast_Mod_WGM1_10; // WGM1_10 Modes 12-15
	
	volatile COMP_CH OC_Ch_FOC1A_B; 
	/* 
	These bits must be set to zero when TCCR1A is written when operating in a PWM mode.
	When writing a logical one to the FOC1A/FOC1B bit, 
	an immediate compare match is forced on the Waveform Generation unit. 
	The OC1A/OC1B output is changed according to its COM1x1:0 bits setting.
	*/
	
    volatile CTC_OP  Norm_CTC_OP_Mode_COM1A_10;
    volatile CTC_OP  Norm_CTC_OP_Mode_COM1B_10;
	
	volatile PWM_OP	PWM_OP_Mode_COM1A_10;
	volatile PWM_OP	PWM_OP_Mode_COM1B_10;
	
    volatile uint16 CUSTOME_TOP_ICR1;

    volatile CLK_SEL Clk_Source_CS1_2_0;

    volatile uint8   Reg_Size;
    volatile uint16  prescalar;
    volatile uint32  Ticks;
    volatile float64  Tick_Time;
    
	volatile uint32 PWM_REQ_FREQ;
	
    volatile uint32  Num_Ovf;
    volatile uint16  Init_Value;

    volatile uint16  Comp_Value_A;
    volatile uint16  Comp_Value_B;
	
	volatile float64 Duty_Cycle_A;
	volatile float64 Duty_Cycle_B;

	void (*CallBack_Fun_OVF)(void);
    void (*CallBack_Fun_COMPA)(void);
	void (*CallBack_Fun_COMPB)(void);
} TIMER1_CONF;

typedef struct
{
	volatile OP_TYPES OperationType;
	volatile CTC_OP Norm_CTC_Op_Mode;
	volatile PWM_OP PWM_Op_Mode;

	volatile CLK_SEL_TIMER_2 Clk_Source_CS2;

	volatile uint8   Reg_Size;
	volatile uint16  prescalar;
	volatile uint32  Ticks;
	volatile float64  Tick_Time;
	
	volatile uint32 PWM_REQ_FREQ;
	
	volatile uint32  Num_Ovf;
	volatile uint8  Init_Value;

	volatile uint8  Comp_Value;
	volatile float64 Duty_Cycle;

	void (*CallBack_Fun_OVF)(void);
	void (*CallBack_Fun_COMP)(void);
} TIMER2_CONF;

#endif /* TIMER_STRUCTURES_H_ */