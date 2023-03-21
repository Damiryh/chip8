#include "keyboard.h"

using namespace ch8;

Keyboard::Keyboard(std::map<sf::Keyboard::Key, byte> &key_map):
	m_key_map(key_map) {
	for (int i = 0; i < 0x10; i++) m_keys[i] = false;
}

void Keyboard::press(sf::Keyboard::Key key) {
	if (auto k = m_key_map.find(key); k != m_key_map.end() ) {
		m_keys[k->second] = true;
	}
}

void Keyboard::release(sf::Keyboard::Key key) {
	if (auto k = m_key_map.find(key); k != m_key_map.end() ) {
		m_keys[k->second] = false;
	}
}

bool Keyboard::getKeyState(byte code) {
	return (code < 0x10) && m_keys[code];
}