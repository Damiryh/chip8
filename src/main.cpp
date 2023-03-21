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

int main(int argc, char **argv) {
	sf::RenderWindow window(sf::VideoMode(640, 320), "chip-8");
	sf::Event event;
	sf::Clock clk;
	
	sf::SoundBuffer sound_buffer;
	sf::Sound beeper;
	
	float ft = 0.f, tt = 0.f, pt = 0.f;
	float tps = 60.f, fps = 30.f, pps = 10000.f;
	
	ch8::Mapper mapper;
	ch8::Screen screen(window);
	ch8::Keyboard keyboard(key_map);
	
	if (!sound_buffer.loadFromFile("resources/square_sample.wav")) { return 1; }
	if (!mapper.loadRom("resources/font.rom")) { return 1; }
	if (!mapper.loadProg("resources/morse_demo.ch8")) { return 1; }
	
	beeper.setBuffer(sound_buffer);
	beeper.setLoop(true);
	beeper.setVolume(0);
	beeper.play();
	
	ch8::Processor processor(mapper, screen, keyboard);
	
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close(); else
			if (event.type == sf::Event::KeyPressed) keyboard.press(event.key.code); else
			if (event.type == sf::Event::KeyReleased) keyboard.release(event.key.code);
		}
		
		float dt = clk.restart().asSeconds();
		ft += dt; tt += dt; pt += dt;
		
		if (ft > 1/tps) {
			ft = 0; processor.decST(); processor.decDT();
		
			if (processor.beep()) {
				beeper.setVolume(100);
			} else {	
				beeper.setVolume(0);
			}
		}
		
		if (tt > 1/fps) {
			tt = 0;
			window.clear(sf::Color(0, 0, 0));
			screen.draw();
			window.display();
		}
		
		if (pt > 1/pps) {
			pt = 0;
			processor.tick();
		}
	}
	
	return 0;
}