#include "mapper.h"
#include <fstream>

using namespace ch8;

Mapper::Mapper() {}

bool Mapper::load(std::string filename) {
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file.is_open()) return false;
	file.read((char*)m_space, 0x1000);
	file.close();
	return true;
}

bool Mapper::loadRom(std::string filename) {
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file.is_open()) return false;
	file.read((char*)m_space, 0x200);
	file.close();
	return true;
}

bool Mapper::loadProg(std::string filename) {
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file.is_open()) return false;
	file.read((char*)m_space + 0x200, 0x1000 - 0x200);
	file.close();
	return true;
}

byte Mapper::peek(word address) {
	return m_space[address];
}

void Mapper::put(word address, byte value) {
	if (address < 0x200) return;
	m_space[address] = value;
}