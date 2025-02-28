#include "stdafx.h"
#include "ShaderManager.h"

#include "ShadersDescription.h"
#include "utils/ErrorHandler.h"

namespace shadermanager {
	[[nodiscard]] std::expected<ShaderProgram*, Error> ShaderManager::RegisterShaderProgram(const std::string& shaderProgramName) {
		using enum ErrorCode;
		if (shaderProgramName.empty()) {
			return std::unexpected(Error{ INVALID_PROGRAM });
		}

		if (!m_desc.programs.contains(shaderProgramName)) {
			return std::unexpected(Error{ UNKNOWN_PROGRAM });
		}
		return ShaderProgram::CreateShaderProgram(*this, shaderProgramName, m_parser, m_desc.programs[shaderProgramName]);
	}

	[[nodiscard]] parsing::Parser& ShaderManager::GetParser() { return m_parser; }

	void ShaderManager::AddShaderDescriptionFile(const std::filesystem::path& path) {
		m_shaderRoot = path.parent_path();


		constexpr glz::opts opts{ .internal = uint32_t(glz::opts_internal::disable_padding) };
		ShadersDescription obj{};
		auto error = glz::read_file_json(obj, path.string(), m_readBuffer);
		if (error) {
			std::string descriptive_error = glz::format_error(error, m_readBuffer);
		} else {
			m_desc = obj;
		}
		error;

	}
	[[nodiscard]] const std::filesystem::path& ShaderManager::ShaderRoot() const {
		return m_shaderRoot;
	}

	void ShaderManager::Clean() {
		
	}
}