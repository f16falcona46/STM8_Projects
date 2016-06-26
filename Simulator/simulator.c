#define F_CPU 16000000L

#include <stm8/stm8s.h>
#include <stdint.h>
#include <stm8/timing.h>

const uint8_t rom[] = {
		0x07, 0xfd, 0x0b, 0x01, 0x51, 0x38, 0xfd, 0x13,
		0x80,/*delay big*/ 0x03, 0x80,/*delay little*/ 0xc1, 0x83, 0x80, 0x0b, 0xc9,
		0x83, 0x80, 0x09, 0x07, 0xff, 0x13, 0x01, 0x62,
		0x38, 0xff, 0xbf, 0x80, 0x07
};

volatile uint8_t ram[512];

typedef struct CPU_State_s {
	uint8_t a;
	uint8_t x;
	uint8_t y;
	uint8_t sp;
	uint16_t pc;
	uint8_t cc;
	volatile uint8_t* (*func_to_get_byte_ptr_from_mem)(struct CPU_State_s*, uint16_t);
	uint8_t* rom;
	volatile uint8_t* ram;
} CPU_State;

void update_pins(CPU_State *state) {
	uint8_t pa_or = *(state->func_to_get_byte_ptr_from_mem)(state, 0xff);
	uint8_t pa_dr = *(state->func_to_get_byte_ptr_from_mem)(state, 0xfd);
	
	*(state->func_to_get_byte_ptr_from_mem)(state, 0xfe) = (PD_IDR << 1) | ((PC_IDR & 0xc0) >> 6);
	
	PD_ODR = ((pa_or & 0xfc) >> 1) | (PD_ODR & 0x03);
	PC_ODR = ((pa_or & 0x03) << 6) | (PC_ODR & 0x3f);
	
	PD_DDR = ((pa_dr & 0xfc) >> 1) | (PD_DDR & 0x03);
	PC_DDR = ((pa_dr & 0x03) << 6) | (PC_DDR & 0x3f);
	
	PD_CR1 = ((pa_dr & 0xfc) >> 1) | (PD_CR1 & 0x03);
	PC_CR1 = ((pa_dr & 0x03) << 6) | (PC_CR1 & 0x3f);
}

volatile uint8_t* get_byte_ptr_from_mem(CPU_State* state, uint16_t index) {
	if (index > 0x7fff) {
		return state->rom + index - 0x8000;
	}
	else {
		return state->ram + index;
	}
}

#ifdef ENABLE_CPU_STATE_DEBUG
void print_state(CPU_State* state) {
	printf("A:  %02x\n", state->a);
	printf("X:  %02x\n", state->x);
	printf("Y:  %02x\n", state->y);
	printf("SP: %02x\n", state->sp);
	printf("PC: %04x\n", state->pc);
	printf("CC: %02x\n", state->cc);
}
#endif

void update_cc(CPU_State* state, uint8_t result) {
	if (result) {
		state->cc &= ~0x01;
	}
	else {
		state->cc |= 0x01;
	}
	state->cc |= 0x80;
}

void simulate_step(CPU_State* state) {
	uint8_t instruction = *(state->func_to_get_byte_ptr_from_mem)(state, state->pc);
	uint8_t prefix = (instruction&0xc0)>>6;
	uint8_t pc_increment = 1;
	switch (prefix) {
	case 0: //mov
	{
		uint16_t dest_loc = (instruction&0x38)>>3;
		uint16_t source_loc = (instruction&0x07);
		uint8_t source = 0;
		switch (source_loc&0x03) {
		case 0: source = state->a; //source is a
		break;
		case 1: source = state->x; //source is x
		break;
		case 2: source = state->y; //source is y
		break;
		case 3: source = *(state->func_to_get_byte_ptr_from_mem)(state, state->pc + 1); //source is immediate
			++pc_increment;
		break;
		default: break;
		}
		if (source_loc&0x04) source = *(state->func_to_get_byte_ptr_from_mem)(state, source); //source is indirect
		if (dest_loc&0x04) {
			switch (dest_loc&0x03) {
			case 0: *(state->func_to_get_byte_ptr_from_mem)(state, state->a) = source; break; //dest is indirect a
			case 1: *(state->func_to_get_byte_ptr_from_mem)(state, state->x) = source; break; //dest is indirect x
			case 2: *(state->func_to_get_byte_ptr_from_mem)(state, state->y) = source; break; //dest is indirect y
			case 3: *(state->func_to_get_byte_ptr_from_mem)(state, *(state->func_to_get_byte_ptr_from_mem)(state, state->pc + 1)) = source; ++pc_increment; break; //dest is indirect imm
			default: break;
			}
		}
		else {
			switch (dest_loc&0x03) {
			case 0: state->a = source; break; //dest is a
			case 1: state->x = source; break; //dest is x
			case 2: state->y = source; break; //dest is y
			default: break;
			}
		}
		update_cc(state, source);
	}
	break;
	case 1: //bitop
	{
		uint8_t source = 0;
		uint8_t dest = 0;
		switch (instruction&0x03) {
		case 0: source = state->a; break;
		case 1: source = state->x; break;
		case 2: source = state->y; break;
		default: break;
		}
		switch ((instruction&0x0c)>>2) {
		case 0: dest = state->a; break;
		case 1: dest = state->x; break;
		case 2: dest = state->y; break;
		default: break;
		}
		switch ((instruction&0x30)>>4) {
		case 0: dest &= source; break; //and
		case 1: dest |= source; break; //or
		case 2: dest ^= source; break; //xor
		case 3: dest = ~source; break; //not
		default: break;
		}
		switch ((instruction&0x0c)>>2) {
		case 0: state->a = dest; break;
		case 1: state->x = dest; break;
		case 2: state->y = dest; break;
		default: break;
		}
		update_cc(state, dest);
	}
	break;
	case 2: //jmp
	{
		uint8_t bit = (instruction&0x1c)>>2;
		uint8_t timeToJump = ((state->cc)&(1<<bit))>>bit;
		if (!(instruction&0x20)) timeToJump = !timeToJump;

		if (timeToJump) {
			state->pc = (*(state->func_to_get_byte_ptr_from_mem)(state, state->pc+1)<<8)|(*(state->func_to_get_byte_ptr_from_mem)(state, state->pc+2));
			pc_increment = 0;
		}
		else {
			pc_increment += 2;
		}
	}
	break;
	case 3: //arith, pushpop (not implemented yet)
	{
		switch (instruction&0x20) {
		case 0:
		{
			uint8_t source = 0;
			uint8_t dest = 0;
			switch (instruction&0x03) {
			case 0: source = state->a; break;
			case 1: source = state->x; break;
			case 2: source = state->y; break;
			default: break;
			}
			switch ((instruction&0x0c)>>2) {
			case 0: dest = state->a; break;
			case 1: dest = state->x; break;
			case 2: dest = state->y; break;
			default: break;
			}
			switch ((instruction&0x10)>>4) {
			case 0: dest += source; break;
			case 1: dest -= source; break;
			default: break;
			}
			switch ((instruction&0x0c)>>2) {
			case 0: state->a = dest; break;
			case 1: state->x = dest; break;
			case 2: state->y = dest; break;
			default: break;
			}
			update_cc(state, dest);
		}
		break;
		case 1: break;
		default:
		break;
		}
	}
	break;
	default: break;
	}
	state->pc += pc_increment;
}

int main() {
	CPU_State state;
	uint16_t i = 0;
	uint8_t blink_divider = 0;
	CLK_CKDIVR = 0x00;
	CLK_PCKENR1 = 0xff;
	CLK_PCKENR2 = 0xff;
	PB_DDR |= 0x20;
	PB_CR1 |= 0x20;
	PB_ODR |= 0x20;
	state.a = 0;
	state.x = 0;
	state.y = 0;
	state.sp = 0;
	state.cc = 0;
	state.pc = 0x8000;
	state.func_to_get_byte_ptr_from_mem = get_byte_ptr_from_mem;
	state.rom = (uint8_t*)rom;
	state.ram = ram;
	for (i = 0; i < 512; ++i) {
		ram[i] = 0;
	}
	while (1) {
		simulate_step(&state);
		update_pins(&state);
		++blink_divider;
		if (!blink_divider) PB_ODR ^= 0x20;
	}
}
