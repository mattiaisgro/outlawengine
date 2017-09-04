#ifndef OUTLAW_CORE_H
#define OUTLAW_CORE_H
#include <iostream>
#include "types.h"

#ifndef SYSTEM

#ifdef WIN32
#define WINDOWS
#endif
#ifdef _WIN32
#define WINDOWS
#endif
#ifdef __WIN32__
#define WINDOWS
#endif
#ifdef __WINDOWS__
#define WINDOWS
#endif

#ifdef __APPLE__
#define MAC
#endif
#ifdef macintosh
#define MAC
#endif

#ifdef __linux__
#define LINUX
#endif
#ifdef __gnu_linux__
#define LINUX
#endif

#endif

namespace outlaw {

	inline void println() {
		std::cout << '\n';
	}
	template<typename First, typename ... Many>
	inline void println(const First &arg, const Many &... rest) {
		std::cout << arg;
		println(rest...);
	}


	inline void _printerror_rest() {
		std::cerr << '\n';
	}
	template<typename First, typename ... Many>
	inline void _printerror_rest(const First &arg, const Many &... rest) {
		std::cerr << arg;
		_printerror_rest(rest...);
	}
	template<typename First, typename ... Many>
	inline void printerror(const First &arg, const Many &... rest) {
		std::cerr << "ERROR: " << arg;
		_printerror_rest(rest...);
	}

	inline void _printlog_rest() {
		std::cerr << '\n';
	}
	template<typename First, typename ... Many>
	inline void _printlog_rest(const First &arg, const Many &... rest) {
		std::cerr << arg;
		_printlog_rest(rest...);
	}
	template<typename First, typename ... Many>
	inline void printlog(const First &arg, const Many &... rest) {
		std::cerr << "LOG: " << arg;
		_printlog_rest(rest...);
	}

}

#endif
