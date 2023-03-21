#pragma once
#include "base.h"
#include <SFML/Graphics.hpp>
#include <map>

namespace ch8 {
	class Keyboard {
	public:
		Keyboard(std::map<sf::Keyboard::Key, byte> &key_map);
		
		void press(sf::Keyboard::Key key);
		void release(sf::Keyboard::Key key);
		
		bool getKeyState(byte code);
		
	private:
		std::map<sf::Keyboard::Key, byte> &m_key_map;
		bool m_keys[0x10];
	};
}