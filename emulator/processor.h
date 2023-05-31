#pragma once
#include "mapper.h"
#include "screen.h"
#include "keyboard.h"

namespace ch8 {
	class Processor {
	public:
		Processor(Mapper &mapper, Screen &screen, Keyboard &keyboard);
		void reset();  // Сброс всех регистров
		void tick();   // Выполнение одной инструкции
		void decDT();  // Декремент таймеров из основного цикла
		void decST();  // Согласно таймингам (60 раз в секунду)
		bool beep();   // Управление генератором звука
		
	private:
		byte V[0x10]; // Основные регистры
		word PC, SP, I; // Регистры инструкции, стека, адреса
		byte ST, DT; // Таймеры звука и задержки
		
		word stk[0x10]; // Стек вызовов
		
		Mapper     &m_mapper; // Доступ к периферии и памяти - по ссылкам:
		Screen     &m_screen; // К экрану 
		Keyboard &m_keyboard; // И к клавиатуре
		
		word read();		  // Чтение слова из памяти (через маппер)
		
		word nnn(word opcode);  // Доступ к операндам инструкции - через
		byte   n(word opcode);  // Вспомогательные функции
		byte   x(word opcode);
		byte   y(word opcode);
		byte  kk(word opcode);
	};
}