#include <stm8/stm8s.h>

void update_pins(CPU_State *state) {
	unsigned char PA_OR = (state->mem)[0xff];
	unsigned char PA_DR = (state->mem)[0xfd];
	
	(state->mem)[0xfe] = (PD_IDR << 1) | ((PC_IDR & 0xc0) >> 6);
	
	PD_ODR = ((PA_OR & 0xfc) >> 1) | (PD_ODR & 0x03);
	PC_ODR = ((PA_OR & 0x03) << 6) | (PC_ODR & 0x3f);
	
	PD_DDR = ((PA_DR & 0xfc) >> 1) | (PD_DDR & 0x03);
	PC_DDR = ((PA_DR & 0x03) << 6) | (PC_DDR & 0x3f);
	
	PD_CR1 = ((PA_DR & 0xfc) >> 1) | (PD_CR1 & 0x03);
	PC_CR1 = ((PA_DR & 0x03) << 6) | (PC_CR1 & 0x3f);
}

typedef struct {
	unsigned char a;
	unsigned char x;
	unsigned char y;
	unsigned char sp;
	unsigned short pc;
	unsigned char cc;
	unsigned char* mem;
} CPU_State;

#ifdef ENABLE_CPU_STATE_DEBUG
void print_state(CPU_State *state) {
	printf("A:  %02x\n", state->a);
	printf("X:  %02x\n", state->x);
	printf("Y:  %02x\n", state->y);
	printf("SP: %02x\n", state->sp);
	printf("PC: %04x\n", state->pc);
	printf("CC: %02x\n", state->cc);
}
#endif

void update_cc(CPU_State *state, unsigned char result) {
	if (result) {
		state->cc &= ~0x01;
	}
	else {
		state->cc |= 0x01;
	}
	state->cc |= 0x80;
}

void simulate_step(CPU_State *state) {
	unsigned char instruction = (state->mem)[state->pc];
	unsigned char prefix = (instruction&0xc0)>>6;
	unsigned char pc_increment = 1;
	switch (prefix) {
	case 0: //mov
	{
		unsigned char dest_loc = (instruction&0x38)>>3;
		unsigned char source_loc = (instruction&0x07);
		unsigned char source = 0;
		switch (source_loc&0x03) {
		case 0: source = state->a; //source is a
		break;
		case 1: source = state->x; //source is x
		break;
		case 2: source = state->y; //source is y
		break;
		case 3: source = (state->mem)[state->pc + 1]; //source is immediate
			++pc_increment;
		break;
		default: break;
		}
		if (source_loc&0x04) source = (state->mem)[source]; //source is indirect
		if (dest_loc&0x04) {
			switch (dest_loc&0x03) {
			case 0: (state->mem)[state->a] = source; break; //dest is indirect a
			case 1: (state->mem)[state->x] = source; break; //dest is indirect x
			case 2: (state->mem)[state->y] = source; break; //dest is indirect y
			case 3: (state->mem)[(state->mem)[state->pc + 1]] = source; ++pc_increment; break; //dest is indirect imm
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
		unsigned char source = 0;
		unsigned char dest = 0;
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
		unsigned char bit = (instruction&0x1c)>>2;
		unsigned char timeToJump = ((state->cc)&(1<<bit))>>bit;
		if (!(instruction&0x20)) timeToJump = !timeToJump;

		if (timeToJump) {
			state->pc = ((state->mem)[state->pc+1]<<8)|((state->mem)[state->pc+2]);
			pc_increment = 0;
		}
		else {
			pc_increment += 2;
		}
	}
	break;
	case 3: //arith, pushpop (not implemented yet)
	{
		switch (instruction&0x30) {
		case 0:
		case 1:
		{
			unsigned char source = 0;
			unsigned char dest = 0;
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
		case 2:
		break;
		default:
		break;
		}
	}
	break;
	default: break;
	}
	state->pc += pc_increment;
}

unsigned char rom[] = {
		0x07, 0xfd, 0x0b, 0x01, 0x51, 0x38, 0xfd, 0x13,
		0x00, 0x03, 0x00, 0xc1, 0x83, 0x00, 0x2b, 0xc9,
		0x83, 0x00, 0x29, 0x07, 0xff, 0x13, 0x01, 0x62,
		0x38, 0xff, 0xbf, 0x00, 0x27
};

unsigned char mem[1024] = {0};

void main() {
	CPU_State state;
	int i = 0;
	for (i = 0x20; i <= 0x3c; ++i) {
		mem[i] = rom[i-0x20];
	}
	state.a = 0;
	state.x = 0;
	state.y = 0;
	state.sp = 0;
	state.cc = 0;
	state.pc = 0x20;
	state.mem = mem;
	while (1) {
		simulate_step(&state);
		update_pins(&state);
	}
}
