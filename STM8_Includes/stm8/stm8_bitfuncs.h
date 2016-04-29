#ifndef STM8_ASM_BIT_FUNCS
#define STM8_ASM_BIT_FUNCS

#define STM8_ASM_BIT_FUNCS_HASH #
#define SETBIT(addr, bit) __asm BSET addr,STM8_ASM_BIT_FUNCS_HASH bit __endasm;
#define CLEARBIT(addr, bit) __asm BRES addr,STM8_ASM_BIT_FUNCS_HASH bit __endasm;
#define TOGGLEBIT(addr, bit) __asm bcpl addr,STM8_ASM_BIT_FUNCS_HASH bit __endasm;

#endif