#pragma once
#include <string>
#include "base.h"


namespace ch8 {
	class Mapper {
	public:
		Mapper();
		
		bool load(std::string filename);
		bool loadRom(std::string filename);
		bool loadProg(std::string filename);
		
		byte peek(word address);
		void put(word address, byte value);
	
	private:
		byte m_space[0x1000];
	};
}