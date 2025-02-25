#include "stdafx.h"

#include "UniformsHandler.h"

#include "ShaderProgram.h"

namespace shadermanager {
	UniformsHandler::UniformsHandler(const ShaderProgram& program, std::unordered_map<std::string_view, GLint>&& locations) noexcept
		: m_program(&program), m_locations(std::move(locations)) {}

	[[nodiscard]] std::expected<UniformsHandler, Error> UniformsHandler::CreateHandler(const ShaderProgram& program, const std::vector<Symbol>& symbols) {
		Error error;

		std::unordered_map<std::string_view, GLint> locations;
		for (const Symbol& symbol : symbols) {
			if (locations.contains(symbol.name)) {
				// Duplicate symbol means the same symbol was defined with different types
				error.set(ErrorCode::CLASHING_UNIFORM_TYPES);
			}
			GLint loc = glGetUniformLocation(program.GetId(), symbol.name.c_str());
			if (loc != -1) {
				// Variable was not optimized away
				locations[symbol.name] = loc;
			}
			if (utils::CheckOpenGLError(error))
				return std::unexpected(error);
		}

		return UniformsHandler(program, std::move(locations));
	}

	GLint UniformsHandler::GetUniformLocation(std::string_view name) const {
		auto it = m_locations.find(name);
		if (it != m_locations.end())
			return it->second;

		return m_locations[name] = glGetUniformLocation(m_program->GetId(), name.data());
	}
}