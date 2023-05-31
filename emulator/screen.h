#pragma once
#include "base.h"
#include <SFML/Graphics.hpp>

// Size: 64x32

namespace ch8 {
	class Screen {
	public:
		Screen(sf::RenderWindow &target); // Экран закрепляется за нужным окном
		
		void clear();  // Полная очистка экрана
		void put(byte x, byte y, bool state); // Установка пикселя
		bool peek(byte x, byte y);			  // Получение значения пикселя
		bool drawLine(byte x, byte y, byte line); // Побитовая отрисовка байта с проверкой коллизии
		void draw(); // отрисовка экрана на заданном окне
		
	private:
		sf::RenderWindow &m_target;
		sf::RectangleShape m_pixel; // Рисуем пиксели с помощью прямоугольника
		byte m_screen_mem[64*32];	// "Видеопамять"
	};
}