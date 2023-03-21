#include "processor.h"
#include <stdlib.h>
#include <stdio.h>

using namespace ch8;

Processor::Processor(Mapper &mapper, Screen &screen, Keyboard &keyboard):
	m_mapper(mapper),
	m_screen(screen),
	m_keyboard(keyboard) {
	for (int i = 0; i < 0x10; i++) V[i] = 0;
	PC = 0x200; SP = 0x000; I = 0x000;
	ST = 0x00; DT = 0x00;
}

void Processor::tick() {
	word op = read();
	int alu = 0;
	
	//printf("%04x %04x\n", PC-2, op);
	
	if ((op & 0xffff) == 0x00e0) { m_screen.clear(); } else
	if ((op & 0xffff) == 0x00ee) { PC = stk[SP]; SP--; } else
	if ((op & 0xf000) == 0x1000) { PC = nnn(op); } else
	if ((op & 0xf000) == 0x2000) { SP++; stk[SP] = PC; PC = nnn(op); } else
	if ((op & 0xf000) == 0x3000) { if (V[x(op)] == kk(op)) read(); } else
	if ((op & 0xf000) == 0x4000) { if (V[x(op)] != kk(op)) read(); } else
	if ((op & 0xf00f) == 0x5000) { if (V[x(op)] == V[x(op)]) read(); } else
	if ((op & 0xf000) == 0x6000) { V[x(op)] = kk(op); } else
	if ((op & 0xf000) == 0x7000) { V[x(op)] += kk(op); } else
	if ((op & 0xf00f) == 0x8000) { V[x(op)] = V[y(op)]; } else
	if ((op & 0xf00f) == 0x8001) { V[x(op)] |= V[y(op)]; } else
	if ((op & 0xf00f) == 0x8002) { V[x(op)] &= V[y(op)]; } else
	if ((op & 0xf00f) == 0x8003) { V[x(op)] ^= V[y(op)]; } else
	if ((op & 0xf00f) == 0x8004) {
		alu = V[x(op)] + V[y(op)];
		V[0x0f] = (alu > 0xff); // carry
		V[x(op)] = alu & 0xff;
	} else
	if ((op & 0xf00f) == 0x8005) {
		alu = V[x(op)] - V[y(op)];
		V[0x0f] = (alu < 0x00); // borrow
		V[x(op)] = alu & 0xff;
	} else
	if ((op & 0xf00f) == 0x8006) {
		V[0x0f] = V[x(op)] & 0x01; // shift right
		V[x(op)] = V[x(op)] >> 1;
	} else 
	if ((op & 0xf00f) == 0x8007) {
		alu = V[y(op)] - V[x(op)];
		V[0x0f] = !(alu < 0x00); // borrow
		V[x(op)] = alu & 0xff;
	} else 
	if ((op & 0xf00f) == 0x800e) {
		V[0x0f] = V[x(op)] & 0x80; // shift left
		V[x(op)] = V[x(op)] << 1;
	} else 
	if ((op & 0xf00f) == 0x9000) {
		if (V[x(op)] != V[y(op)]) read();
	} else 
	if ((op & 0xf000) == 0xa000) {
		I = nnn(op);
	} else 
	if ((op & 0xf000) == 0xb000) {
		PC = nnn(op) + V[0];
	} else 
	if ((op & 0xf000) == 0xc000) {
		V[x(op)] = (rand() % 256) & kk(op);
	} else 
	if ((op & 0xf000) == 0xd000) {
		byte size = n(op);
		byte x_pos = V[x(op)];
		byte y_pos = V[y(op)];
		
		for (byte i = 0; i < size; i++) {
			V[0xf] = m_screen.drawLine(x_pos, (y_pos + i) % 32, m_mapper.peek((I + i) & 0xfff));
		}
	} else 
	if ((op & 0xf0ff) == 0xe09e) {
		if (m_keyboard.getKeyState(V[x(op)])) read();
	} else 
	if ((op & 0xf0ff) == 0xe0a1) {
		if (!m_keyboard.getKeyState(V[x(op)])) read();
	} else 
	if ((op & 0xf0ff) == 0xf007) {
		V[x(op)] = DT;
	} else 
	if ((op & 0xf0ff) == 0xf00a) {
		int pressed = -1;
		
		for (byte i = 0; i < 0x0f; i++) {
			if (m_keyboard.getKeyState(i)) pressed = i;
		}
		
		if (pressed < 0) { PC = (PC - 2) & 0xfff; }
		else {
			V[x(op)] = pressed & 0x0f;
			m_keyboard.release(pressed);
		}
	} else 
	if ((op & 0xf0ff) == 0xf015) {
		DT = V[x(op)];
	} else 
	if ((op & 0xf0ff) == 0xf018) {
		ST = V[x(op)];
	} else 
	if ((op & 0xf0ff) == 0xf01e) {
		I += V[x(op)];
	} else 
	if ((op & 0xf0ff) == 0xf029) {
		I = (V[x(op)] & 0xf) * 5; // for 8x5 font
	} else 
	if ((op & 0xf0ff) == 0xf033) {
		/* BCD representation */
		m_mapper.put(I, V[x(op)] / 100);
		m_mapper.put(I + 1, (V[x(op)] / 10) % 10);
		m_mapper.put(I + 2, V[x(op)] % 10);
	} else 
	if ((op & 0xf0ff) == 0xf055) {
		for (int i = 0; i < 0x10; i++) m_mapper.put((I+i) & 0xfff, V[i]);
	} else 
	if ((op & 0xf0ff) == 0xf065) {
		for (int i = 0; i < 0x10; i++) V[i] = m_mapper.peek((I+i) & 0xfff);
	}
}

void Processor::decDT() {
	if (DT == 0) return; DT--;
}

void Processor::decST() {
	if (ST == 0) return; ST--;
}

bool Processor::beep() {
	return (ST != 0);
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