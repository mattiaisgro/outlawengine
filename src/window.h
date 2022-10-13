#ifndef _WINDOW_H
#define _WINDOW_H
#include <string>
#include "types.h"

namespace outlaw {

	/*
	* Window modes
	* WINDOW_WINDOWED_FULLSCREEN is not supported, it is still in development.
	*/
	enum class WindowMode {
		WINDOW_WINDOWED = 0,
		WINDOW_FULLSCREEN = 1,
		WINDOW_WINDOWED_FULLSCREEN = 2
	};

	class Window {
		private:
			static std::string title;
			static void* ID;
			static WindowMode mode;
			static uint width;
			static uint height;
			static uint samples;
			static bool sRGB;
			static bool vsync;
			static bool visible;
			static bool resizable;
			static bool initialized;

			struct WindowNext {
				WindowMode mode;
				uint samples;
				bool sRGB;
				bool vsync;
				bool resizable;
			};

			static WindowNext next;

			Window() = delete;
			~Window() = delete;

		public:

			static int create(std::string title = "Outlaw Rendering Engine", uint width = 1200, uint height = 800);

			static void destroy();

			static void swapBuffers();

			static void pollEvents();

			static void focus();

			static void maximize();

			static void iconify();

			//Setters

			static void setWidth(uint width);

			static void setHeight(uint height);

			static void setTitle(std::string title);

			static void setVisible(bool visible);

			static void setResizable(bool resizable);

			static void setShouldClose(bool shouldClose);

			static void setRatio(int number, int denom);

			static void setPosition(uint x, uint y);

			static void setSamples(uint samples);

			static void setMode(WindowMode mode);

			static void setsRGB(bool sRGB);

			static void setVsync(bool vsync);

			static void setClipboard(std::string s);


			//Getters

			static int getWidth();

			static int getHeight();

			static std::string getTitle();

			static bool getVisible();

			static bool getResizable();

			static bool getShouldClose();

			static void getRatio(int* number, int* denom);

			static float getAspectRatio();

			static void getPosition(int* x, int* y);

			static uint getSamples();

			static WindowMode getMode();

			static bool getsRGB();

			static bool getVsync();

			static std::string getClipboard();

			static bool isFocused();

			static bool isIconified();

			static int getWindowAttrib(int attrib);

			static void setWindowHint(int hint, int value);

			inline static void* getID() {
				return ID;
			}

			static void reshape();

			// static double getTime();

		private:
			static void initCallbacks();

	};

}

#endif
