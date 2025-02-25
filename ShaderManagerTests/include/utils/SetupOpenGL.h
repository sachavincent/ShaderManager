#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct GLFWwindow;

namespace utils {
	class OpenGLWrapper {
	private:
		inline static GLFWwindow* s_window{};
	public:
		static void PrepareOpenGL();
	};
}