#include "stdafx.h"
#include "ShaderProgram.h"

#include "ShaderManager.h"
#include "Shader.h"
#include "UniformsHandler.h"
#include "utils/OpenGLConstants.h"

namespace {
	bool CheckLinkErrors(sm::Error& error, GLuint shaderProgramId) {
		GLint success;
		glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
		if (success == GL_TRUE)
			return true;

		GLint infoLength;
		glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &infoLength);
		if (infoLength <= 0) {
			error.set(sm::ErrorCode::LINK_SHADER);
			return false;
		}

		std::string desc;
		desc.resize(infoLength);
		glGetProgramInfoLog(shaderProgramId, infoLength, nullptr, desc.data());

		error.set(sm::ErrorCode::LINK_SHADER, desc);
		return false;
	}
}

namespace shadermanager {
	ShaderProgram::ShaderProgram(private_ctor_t, ShaderProgramID id, ShaderManager& manager,
								 std::unordered_map<ShaderType, Shader*>&& shaders,
								 std::unordered_map<::glslang::TStorageQualifier, std::vector<Symbol>>&& symbols)
		: m_manager(manager), m_id(id), m_shaders(std::move(shaders)), m_symbols(std::move(symbols)) {
		if (utils::HasNoOpenGLError())
			m_status.set(0);
	}

	[[nodiscard]] std::expected<ShaderProgram*, Error> ShaderProgram::CreateShaderProgram(ShaderManager& manager, const std::string& shaderProgramName, parsing::Parser& parser, const ShaderDescription& shaderDescription) {
		Error error;
		ShaderProgramID id = glCreateProgram();
		if (utils::CheckOpenGLError(error) || id == 0) {
			return std::unexpected(error);
		}

		parsing::ProgramParser programParser = parser.CreateProgramParser();
		std::unordered_map<ShaderType, Shader*> shaders;
		for (const auto& [shaderType, shaderPath] : shaderDescription.files) {
			auto exp = Shader::CreateShader(manager, shaderPath, programParser, shaderType);
			if (exp.has_value())
				shaders[shaderType] = *exp;
			else
				return std::unexpected(exp.error());
		}

		const auto result = manager.m_programs.try_emplace(shaderProgramName, private_ctor_t{}, id, manager, std::move(shaders), std::move(programParser.m_traverser.m_symbols));

		return &result.first->second;
	}

	[[nodiscard]] Error ShaderProgram::AssignUniforms() {
		auto handlerExp = UniformsHandler::CreateHandler(*this, GetSymbols(::glslang::TStorageQualifier::EvqUniform));
		if (!handlerExp) {
			return handlerExp.error();
		}
		m_handler = *handlerExp;
		return {};
	}

	[[nodiscard]] bool ShaderProgram::AttachToShaders() {
		if (error.hasError()) {
			return false;
		}
		if (!m_status.test(0)) {
			error = Error{ ErrorCode::WRONG_PROGRAM_STATUS };
			return false;
		}

		for (const auto& [_, shader] : m_shaders) {
			if (!shader->AttachTo(*this))
				return false;
		}

		m_status.set(1);
		return true;
	}

	[[nodiscard]] bool ShaderProgram::Link() {
		if (error.hasError()) {
			return false;
		}

		if (!m_status.test(1)) {
			error = Error{ ErrorCode::WRONG_PROGRAM_STATUS };
			return false;
		}

		glLinkProgram(m_id);

		if (!CheckLinkErrors(error, m_id)) {
			std::ignore = CleanUp();
			return false;
		}

		m_status.set(2);

		if (error = AssignUniforms(); error.hasError()) {
			std::ignore = CleanUp();
			return false;
		}

		return true;
	}

	[[nodiscard]] bool ShaderProgram::DetachFromShaders() {
		if (error.hasError()) {
			return false;
		}

		if (!m_status.test(1)) {
			error = Error{ ErrorCode::WRONG_PROGRAM_STATUS };
			return false;
		}
		m_status.set(2, false);

		for (const auto& [_, shader] : m_shaders) {
			if (!shader->DetachFrom(*this))
				return false;
		}
		return true;
	}


	[[nodiscard]] bool ShaderProgram::Start() const {
		if (error.hasError()) {
			return false;
		}

		if (!m_status.test(2) || m_status.test(3)) {
			error = Error{ ErrorCode::WRONG_PROGRAM_STATUS };
			return false;
		}

		glUseProgram(m_id);

		m_status.set(3);
		return utils::HasNoOpenGLError();
	}

	[[nodiscard]] bool ShaderProgram::Stop() const {
		if (error.hasError()) {
			return false;
		}

		if (!m_status.test(3)) {
			error = Error{ ErrorCode::WRONG_PROGRAM_STATUS };
			return false;
		}

		glUseProgram(0);

		m_status.set(3, false);
		return utils::HasNoOpenGLError();
	}

	[[nodiscard]] ShaderProgramID ShaderProgram::GetId() const { return m_id; }

	[[nodiscard]] const UniformsHandler& ShaderProgram::Handler() const { return m_handler; }

	[[nodiscard]] const std::vector<Symbol>& ShaderProgram::GetSymbols(::glslang::TStorageQualifier qualifier) const {
		if (!m_symbols.contains(qualifier))
			return s_emptyVector;
		return m_symbols.at(qualifier);
	}

	[[nodiscard]] bool operator==(const ShaderProgram& lhs, const ShaderProgram& rhs) noexcept {
		return lhs.m_id == rhs.m_id;
	}
	[[nodiscard]] bool operator!=(const ShaderProgram& lhs, const ShaderProgram& rhs) noexcept {
		return lhs.m_id != rhs.m_id;
	}
}