#include "screen.h"

using namespace ch8;

Screen::Screen(sf::RenderWindow &target):
	m_target(target),
	m_pixel(sf::Vector2f(10, 10)) {
	m_pixel.setFillColor(sf::Color(255,255,255));
	clear();
}

void Screen::clear() {
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 64; j++) {
			m_screen_mem[i*64 + j] = 0;
		}
	}
}

void Screen::put(byte x, byte y, bool state) {
	m_screen_mem[y*64 + x] = state;
}

bool Screen::peek(byte x, byte y) {
	return m_screen_mem[y*64 + x];
}

bool Screen::drawLine(byte x, byte y, byte line) {
	bool intersect = false;
	for (byte i = 0; i < 8; i++) {
		byte mask = 0x80 >> i;
		byte pos = (x + i) % 64;
		if (!(mask & line)) continue;
		bool pixel = peek(pos, y);
		if (pixel) intersect = true;
		put(pos, y, !pixel);
	}
	return intersect;
}

void Screen::draw() {
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 64; j++) {
			if (!m_screen_mem[i*64 + j]) continue;
			m_pixel.setPosition(10*j, 10*i);
			m_target.draw(m_pixel);
		}
	}
}