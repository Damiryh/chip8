#include <SFML/Graphics.hpp>
#include "mapper.h"
#include "screen.h"

int main(int argc, char **argv) {
	sf::RenderWindow window(sf::VideoMode(640, 320), "chip-8");
	sf::Event event;
	sf::Clock clk;
	float fps = 0;
	
	ch8::Screen screen(window);
	
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}
		
		float dt = clk.restart().asSeconds();
		fps += dt; if (fps < 1/60) continue; fps = 0;
		
		window.clear(sf::Color(0, 0, 0));
		screen.draw();
		window.display();
	}
	
	return 0;
}