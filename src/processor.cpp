#include "processor.h"
#include <stdlib.h>

using namespace ch8;

Processor::Processor(Mapper &mapper, Screen &screen):
	m_mapper(mapper),
	m_screen(screen) {
	for (int i = 0; i < 0x10; i++) V[i] = 0;
	PC = 0x200; SP = 0x000; I = 0x000;
	ST = 0x00; DT = 0x00;
}

void Processor::tick() {
	word op = read();
	int alu = 0;
	
	if (op & 0xffff == 0x00e0) { /* clear screen */ } else
	if (op & 0xffff == 0x00ee) { PC = stk[SP]; SP--; } else
	if (op & 0xf000 == 0x1000) { PC = nnn(op); } else
	if (op & 0xf000 == 0x2000) { SP++; stk[SP] = PC; PC = nnn(op); } else
	if (op & 0xf000 == 0x3000) { if (V[x(op)] == kk(op)) read(); } else
	if (op & 0xf000 == 0x4000) { if (V[x(op)] != kk(op)) read(); } else
	if (op & 0xf00f == 0x5000) { if (V[x(op)] == V[x(op)]) read(); } else
	if (op & 0xf000 == 0x6000) { V[x(op)] = kk(op); } else
	if (op & 0xf000 == 0x7000) { V[x(op)] += kk(op); } else
	if (op & 0xf00f == 0x8000) { V[x(op)] = V[y(op)]; } else
	if (op & 0xf00f == 0x8001) { V[x(op)] |= V[y(op)]; } else
	if (op & 0xf00f == 0x8002) { V[x(op)] &= V[y(op)]; } else
	if (op & 0xf00f == 0x8003) { V[x(op)] ^= V[y(op)]; } else
	if (op & 0xf00f == 0x8004) {
		alu = V[x(op)] + V[y(op)];
		V[0x0f] = (alu > 0xff); // carry
		V[x(op)] = alu & 0xff;
	} else
	if (op & 0xf00f == 0x8005) {
		alu = V[x(op)] - V[y(op)];
		V[0x0f] = (alu < 0x00); // borrow
		V[x(op)] = alu & 0xff;
	} else
	if (op & 0xf00f == 0x8006) {
		V[0x0f] = V[x(op)] & 0x01;
		V[x(op)] = V[x(op)] >> 1;
	} else 
	if (op & 0xf00f == 0x8007) {
		alu = V[y(op)] - V[x(op)];
		V[0x0f] = !(alu < 0x00); // borrow
		V[x(op)] = alu & 0xff;
	} else 
	if (op & 0xf00f == 0x800e) {
		V[0x0f] = V[x(op)] & 0x80;
		V[x(op)] = V[x(op)] << 1;
	} else 
	if (op & 0xf00f == 0x9000) {
		if (V[x(op)] != V[y(op)]) read();
	} else 
	if (op & 0xf000 == 0xa000) {
		I = nnn(op);
	} else 
	if (op & 0xf000 == 0xb000) {
		PC = nnn(op) + V[0];
	} else 
	if (op & 0xf000 == 0xc000) {
		V[x(op)] = (rand() % 256) & kk(op);
	} else 
	if (op & 0xf000 == 0xd000) {
		/* Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision. 
		
		The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites. */
	} else 
	if (op & 0xf0ff == 0xe09e) {
		/* Skip next instruction if key with the value of Vx is pressed.

		Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2. */
	} else 
	if (op & 0xf0ff == 0xe0a1) {
		/* Skip next instruction if key with the value of Vx is not pressed.

		Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2. */
	} else 
	if (op & 0xf0ff == 0xf007) {
		V[x(op)] = DT;
	} else 
	if (op & 0xf0ff == 0xf00a) {
		/* Wait for a key press, store the value of the key in Vx.

		All execution stops until a key is pressed, then the value of that key is stored in Vx. */
	} else 
	if (op & 0xf0ff == 0xf015) {
		DT = V[x(op)];
	} else 
	if (op & 0xf0ff == 0xf018) {
		ST = V[x(op)];
	} else 
	if (op & 0xf0ff == 0xf01e) {
		I += V[x(op)];
	} else 
	if (op & 0xf0ff == 0xf029) {
		/* Set I = location of sprite for digit Vx. */
	} else 
	if (op & 0xf0ff == 0xf033) {
		/* Store BCD representation of Vx in memory locations I, I+1, and I+2. */
	} else 
	if (op & 0xf0ff == 0xf055) {
		for (int i = 0; i < 0x10; i++) m_mapper.put((I+i) & 0xfff, V[i]);
	} else 
	if (op & 0xf0ff == 0xf065) {
		for (int i = 0; i < 0x10; i++) V[i] = m_mapper.peek((I+i) & 0xfff);
	}
}

word Processor::read() {
	word result = (m_mapper.peek(PC) << 8) | m_mapper.peek(PC + 1);
	PC = (PC + 2) & 0xfff;
	return result;
}

word Processor::nnn(word opcode) {
	return opcode & 0x0fff;
}

byte Processor::n(word opcode) {
	return opcode & 0x000f;
}

byte Processor::x(word opcode) {
	return (opcode & 0x0f00) >> 8;
}

byte Processor::y(word opcode) {
	return (opcode & 0x00f0) >> 4;
}

byte Processor::kk(word opcode) {
	return opcode & 0x00ff;
}