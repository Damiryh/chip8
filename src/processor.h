#pragma once
#include "mapper.h"
#include "screen.h"

namespace ch8 {
	class Processor {
	public:
		Processor(Mapper &mapper, Screen &screen);
		void tick();
		
	private:
		byte V[0x10]; // General purpose registers
		word PC, SP, I; // Program, stack, address registers
		byte ST, DT; // Sound & Delay timer registers
		
		word stk[0x10]; // Subroutine stack
		
		Mapper &m_mapper;
		Screen &m_screen;
		
		word read();
		
		word nnn(word opcode);
		byte   n(word opcode);
		byte   x(word opcode);
		byte   y(word opcode);
		byte  kk(word opcode);
	};
}