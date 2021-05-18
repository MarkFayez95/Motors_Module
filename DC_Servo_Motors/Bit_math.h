#ifndef BIT_MATH_H_INCLUDED
#define BIT_MATH_H_INCLUDED

#define SetBit(Var,bit) (Var|=(1<<bit))
#define ClearBit(Var,bit) (Var&=~(1<<bit))
#define ToggleBit(Var,bit) (Var^=(1<<bit))

//#define GetBit(Var,bit) ((Var&(1<<bit))>>bit)

#define GetBit(Var,bit) ((Var>>bit)&(0x01))


#endif // BIT_MATH_H_INCLUDED
