#include "stdafx.h"
#include "utils/SetupOpenGL.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <stdexcept>

namespace utils {
	void OpenGLWrapper::PrepareOpenGL() {
		if (!glfwInit())
			throw std::runtime_error("Error in glfwInit()");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#else
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
#endif
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

		s_window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
		if (!s_window)
			throw std::runtime_error("Failed to create window!");

		glfwMakeContextCurrent(s_window);
		if (!gladLoadGL())
			throw std::runtime_error("Error in gladLoadGL()");
	}
}