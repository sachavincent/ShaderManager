#include "stdafx.h"
#include "Shader.h"
#include "ShaderParser.h"
#include "ShaderManager.h"
#include "ShaderProgram.h"
#include "utils/OpenGLConstants.h"
#include <fstream>

namespace {
	bool CheckCompileErrors(sm::Error& error, GLuint shaderId) {
		GLint success;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (success == GL_TRUE)
			return true;

		GLint infoLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);
		if (infoLength <= 0) {
			error.set(sm::ErrorCode::COMPILE_SHADER);
			return false;
		}

		std::string desc;
		desc.resize(infoLength);
		glGetShaderInfoLog(shaderId, infoLength, nullptr, desc.data());

		error.set(sm::ErrorCode::COMPILE_SHADER, desc);
		return false;
	}
}

namespace shadermanager {
	[[nodiscard]] std::expected<Shader*, Error> Shader::CreateShader(ShaderManager& manager, const ShaderPath& shaderPath, parsing::ProgramParser& programParser, ShaderType shaderType) {
		if (manager.m_shaders.contains(shaderPath)) {
			// Shader already parsed
			return &manager.m_shaders.at(shaderPath);
		}

		const auto shaderContentExp = programParser.ParseShader(ShaderFile(manager.ShaderRoot() / shaderPath, shaderType));
		if (!shaderContentExp) {
			return std::unexpected(shaderContentExp.error());
		}

		Error error;
		ShaderID id = glCreateShader(opengl::GetOpenGLType(shaderType));
		if (utils::CheckOpenGLError(error) || id == 0) {
			return std::unexpected(error);
		}

		const char* source[1] = { shaderContentExp->c_str() };
		glShaderSource(id, 1, source, nullptr);
		if (utils::CheckOpenGLError(error)) {
			glDeleteShader(id);
			return std::unexpected(error);
		}

		glCompileShader(id);
		if (utils::CheckOpenGLError(error)) {
			glDeleteShader(id);
			return std::unexpected(error);
		}

		if (!CheckCompileErrors(error, id)) {
			glDeleteShader(id);
			return std::unexpected(error);
		}
		auto res = manager.m_shaders.try_emplace(shaderPath, private_ctor_t{}, id, manager, shaderType);
		return &res.first->second;
	}

	Shader::Shader(private_ctor_t, ShaderID id, ShaderManager& manager, ShaderType shaderType) : m_id(id), m_manager(manager), m_shaderType(shaderType) {}

	[[nodiscard]] bool Shader::operator==(const Shader& shader) const noexcept { return shader.m_id == m_id; }
	[[nodiscard]] bool Shader::operator!=(const Shader& shader) const noexcept { return shader.m_id != m_id; }

	[[nodiscard]] bool Shader::Clean() {
		bool success = true;
		for (auto& program : m_programs) {
			success |= DetachFrom(*program);
		}

		glDeleteShader(m_id);
		success |= utils::HasNoOpenGLError();

		return success;
	}

	[[nodiscard]] bool Shader::Compile() const {
		glCompileShader(m_id);
		return utils::HasNoOpenGLError();
	}

	[[nodiscard]] bool Shader::AttachTo(ShaderProgram& program) {
		const auto res = m_programs.insert(&program);
		if (!res.second)
			return false;

		glAttachShader(program.GetId(), m_id);

		return utils::HasNoOpenGLError();
	}

	[[nodiscard]] bool Shader::DetachFrom(ShaderProgram& program) {
		const auto res = m_programs.erase(&program);
		if (res == 0)
			return false;

		glDetachShader(program.GetId(), m_id);
		return utils::HasNoOpenGLError();
	}
}