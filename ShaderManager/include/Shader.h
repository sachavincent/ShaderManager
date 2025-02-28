#pragma once

#include <glad/glad.h>
#include <memory>
#include <filesystem>
#include <expected>
#include <glm/glm.hpp>
#include "ShaderFile.h"
#include "utils/ErrorHandler.h"
#include "utils/Constants.h"

namespace shadermanager {
	namespace parsing {
		class ProgramParser;
	}
	namespace utils {
		class FileWatcher;
	}

	class ShaderManager;
	class ShaderProgram;

	using ShaderID = GLuint;
	using Slot = GLuint;

	class Shader {
		friend class utils::FileWatcher;
	private:
		struct private_ctor_t {};

	private:
		const ShaderID m_id;
		const ShaderType m_shaderType;
		const std::reference_wrapper<ShaderManager> m_manager;

		mutable std::unordered_map<const char*, GLuint> m_locations;
		std::unordered_set<ShaderProgram*> m_programs;

	public:
		explicit Shader(private_ctor_t, ShaderID id, ShaderManager& manager, ShaderType shaderType);

		[[nodiscard]] static std::expected<Shader*, Error> CreateShader(ShaderManager& manager, const ShaderPath& shaderPath, parsing::ProgramParser& programParser, ShaderType shaderType);

		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;

		~Shader() = default;

		[[nodiscard]] bool operator==(const Shader& shader) const noexcept;
		[[nodiscard]] bool operator!=(const Shader& shader) const noexcept;

		[[nodiscard]] bool Clean();

		[[nodiscard]] bool Start() const;

		[[nodiscard]] bool Stop() const;

		[[nodiscard]] bool Compile() const;

		[[nodiscard]] bool AttachTo(ShaderProgram& program);

		[[nodiscard]] bool DetachFrom(ShaderProgram& program);
	};
}

namespace sm = shadermanager;