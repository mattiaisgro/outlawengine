#ifndef OUTLAW_INPUT_H
#define OUTLAW_INPUT_H
#include "window.h"
#include "uroboro/vec.h"
#include "keydef.h"

using namespace uroboro;

namespace outlaw {

	constexpr int ButtonPressed = 0;
	constexpr int ButtonReleased = 1;
	constexpr int ButtonUndefined = 3;

	class Input {
		private:
			static void* window_id;
			static int scroll;
			Input() {}
			~Input() {}
		public:

			static void create();

			static void destroy();

			static bool isPressed(int button);

			static bool isReleased(int button);

			static int getKey(int button);

			static bool isMousePressed(int button);

			static bool isMouseReleased(int button);

			static int getMouseKey(int button);

			static vec2 getMousePos();

			static void setMousePos(double xpos, double ypos);

			static void setMousePos(vec2 pos);

			static void hideCursor(); // TO-DO

			static void showCursor(); // TO-DO

			static int getScroll(); //TO-DO

			// TO-DO
			friend void keyboard_callback(void* window, int key, int scancode, int action, int mods);

			friend void mouse_button_callback(void* window, int button, int action, int mods);

			friend void mouse_pos_callback(void* window, double xpos, double ypos);

	};

}

#endif
