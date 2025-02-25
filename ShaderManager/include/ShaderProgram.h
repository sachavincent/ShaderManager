#pragma once

#include "Shader.h"
#include "utils/ParserUtils.h"
#include "utils/OpenGLConstants.h"
#include "ShadersDescription.h"
#include "UniformsHandler.h"
#include "ShaderParser.h"
#include "utils/Symbol.h"
#pragma warning (push)
#pragma warning (disable: 4244)
#include <glaze/json.hpp>
#include <bitset>

namespace shadermanager {
	class Shader;
	namespace utils {
		class ErrorHandler;
		class FileWatcher;
	}
	using ShaderProgramID = GLuint;
	class ShaderProgram {
		friend class utils::ErrorHandler;
		friend class utils::FileWatcher;
	private:
		struct private_ctor_t {};
	private:
		const std::reference_wrapper<ShaderManager> m_manager;
		const ShaderProgramID m_id;
		std::unordered_map<ShaderType, Shader*> m_shaders;
		const std::unordered_map<::glslang::TStorageQualifier, std::vector<Symbol>> m_symbols;
		magic_enum::containers::bitset<ShaderType> m_types;

		UniformsHandler m_handler;

		mutable Error error{};

		inline static std::vector<Symbol> s_emptyVector{};

		mutable std::bitset<4> m_status;

		[[nodiscard]] Error AssignUniforms();

	public:
		explicit ShaderProgram(private_ctor_t, ShaderProgramID id, ShaderManager& manager,
							   std::unordered_map<ShaderType, Shader*>&& shaders, 
							   std::unordered_map<::glslang::TStorageQualifier, std::vector<Symbol>>&& symbols);

		[[nodiscard]] static std::expected<ShaderProgram*, Error> CreateShaderProgram(ShaderManager& manager, const std::string& shaderProgramName, parsing::Parser& parser, const ShaderDescription& shaderDescription);
		
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&&) = delete;

		~ShaderProgram() = default;

		[[nodiscard]] friend bool operator==(const ShaderProgram&, const ShaderProgram&) noexcept;
		[[nodiscard]] friend bool operator!=(const ShaderProgram&, const ShaderProgram&) noexcept;

		[[nodiscard]] bool Link();

		[[nodiscard]] bool AttachToShaders();

		[[nodiscard]] bool DetachFromShaders();

		[[nodiscard]] bool Start() const;

		[[nodiscard]] bool Stop() const;

		[[nodiscard]] bool CleanUp() const { return false; }

		[[nodiscard]] ShaderProgramID GetId() const;

		[[nodiscard]] const UniformsHandler& Handler() const;

		[[nodiscard]] const std::vector<Symbol>& GetSymbols(::glslang::TStorageQualifier qualifier) const;
	};
}
namespace sm = shadermanager;
#pragma warning (pop)
