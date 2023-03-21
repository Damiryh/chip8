#pragma once
#include "base.h"
#include <SFML/Graphics.hpp>

// Size: 64x32

namespace ch8 {
	class Screen {
	public:
		Screen(sf::RenderWindow &target);
		
		void clear();
		
		void put(byte x, byte y, bool state);
		bool peek(byte x, byte y);
		
		bool drawLine(byte x, byte y, byte line);
		
		void draw();
		
	private:
		sf::RenderWindow &m_target;
		sf::RectangleShape m_pixel;
		byte m_screen_mem[64*32];
	};
}