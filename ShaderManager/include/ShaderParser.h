#pragma once

#include "Shader.h"
#include "ShaderTraverser.h"
#include "utils/Symbol.h"
#include "utils/ErrorHandler.h"
#include <glslang/Include/BaseTypes.h>

#include <unordered_map>
#include <expected>

class ParsingTests;
namespace shadermanager {
	class ShaderManager;
	namespace parsing {
		using ShaderBindingIndex = uint16_t;

		class Parser;
		class ProgramParser {
			friend class ShaderProgram;
			friend class Shader;
			friend class Parser;
			friend class ::ParsingTests;
		private:
			std::reference_wrapper<Parser> m_parser;
			//std::reference_wrapper<ShaderProgram> m_program;

			::sm::ShaderTraverser m_traverser;

		protected:
			explicit ProgramParser(Parser& parser) noexcept;

			[[nodiscard]] std::expected<ShaderFileContent, Error> ParseShader(const ShaderFile& shaderFile);
		};


		class Parser {
			friend class ::shadermanager::ShaderManager;
			friend class ProgramParser;

		private:
			//std::unordered_map<ShaderFile, ShaderFileContent> m_shaders;
			std::unordered_map<std::string_view, ShaderBindingIndex> m_shaderBufferBindings;
			std::unordered_map<ShaderType, int> m_maximumBlocksAllowed;

			ShaderBindingIndex m_nextBindingIndex = 0;

			[[nodiscard]] std::expected<ShaderBindingIndex, Error> GetNextBufferBinding();

			Parser() = default;

		public:
			void Initialize();

			[[nodiscard]] ProgramParser CreateProgramParser();
		};
	}
}
namespace sm = shadermanager;