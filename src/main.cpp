#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "base.h"
#include "mapper.h"
#include "screen.h"
#include "keyboard.h"
#include "processor.h"

#include <map>

using sf::Keyboard;


// TODO: Загрузка раскладки из файла
std::map<sf::Keyboard::Key, ch8::byte> key_map {
	{sf::Keyboard::Num1, 0x1},
	{sf::Keyboard::Num2, 0x2},
	{sf::Keyboard::Num3, 0x3},
	{sf::Keyboard::Num4, 0xc},
	{   sf::Keyboard::Q, 0x4},
	{   sf::Keyboard::W, 0x5},
	{   sf::Keyboard::E, 0x6},
	{   sf::Keyboard::R, 0xd},
	{   sf::Keyboard::A, 0x7},
	{   sf::Keyboard::S, 0x8},
	{   sf::Keyboard::D, 0x9},
	{   sf::Keyboard::F, 0xe},
	{   sf::Keyboard::Z, 0xa},
	{   sf::Keyboard::X, 0x0},
	{   sf::Keyboard::C, 0xb},
	{   sf::Keyboard::V, 0xf}
};

sf::RenderWindow window(sf::VideoMode(640, 320), "chip-8");
sf::Event event;
sf::Clock clk;

sf::SoundBuffer sound_buffer;
sf::Sound beeper;

double ft, tt, pt;
double tps, fps, pps;

ch8::Mapper mapper;
ch8::Screen screen(window);
ch8::Keyboard keyboard(key_map);
ch8::Processor processor(mapper, screen, keyboard);

void reset() {
	ft = 0.f;
	tt = 0.f;
	pt = 0.f;
	tps = 60.f;  	// тиков таймеров в секунду
	fps = 30.f;  	// отрисовок экрана в секунду
	pps = 1000.f; 	// инструкций в секунду
	
	beeper.setVolume(0);
	processor.reset();
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("%s\n", "Usage: chip8 [options] <file>");
		return 1;
	}
	
	// Если не получается загрузить ресы, то руинимся
	if (!sound_buffer.loadFromFile("resources/square_sample.wav")) { exit(1); }
	if (!mapper.loadRom("resources/font.rom")) { exit(1); }
	if (!mapper.loadProg(argv[argc-1])) { exit(1); }
	
	
	// TODO: Перенести логику бипера в отдельный класс
	beeper.setBuffer(sound_buffer);
	beeper.setLoop(true);
	beeper.play();
	
	reset();
	
	while (window.isOpen()) {
		// Ловим события для клавиатуры и окна
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close(); else
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Key::Backspace) { reset(); } else { keyboard.press(event.key.code); }
			} else
			if (event.type == sf::Event::KeyReleased) keyboard.release(event.key.code);
		}
		
		// Измеряем время
		float dt = clk.restart().asSeconds();
		ft += dt; tt += dt; pt += dt;
		
		// Тикаем таймерами
		if (ft > 1/tps) {
			ft = 0; processor.decST(); processor.decDT();
		
			if (processor.beep()) {
				beeper.setVolume(100);
			} else {	
				beeper.setVolume(0);
			}
		}
		
		// Отрисовываем
		if (tt > 1/fps) {
			tt = 0;
			window.clear(sf::Color(0, 0, 0));
			screen.draw();
			window.display();
		}
		
		// Исполняем одну инструкцию
		if (pt > 1/pps) {
			pt = 0;
			processor.tick();
		}
	}
	
	return 0;
}