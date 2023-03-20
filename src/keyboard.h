#pragma once

namespace ch8 {
	class Keyboard {
	public:
		Keyboard();
		void press();
		
	private:
		bool keys[0x10];
	};
}