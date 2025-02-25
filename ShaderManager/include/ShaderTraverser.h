#pragma once

#include "utils/Symbol.h"
#include <glslang/Include/intermediate.h>

namespace shadermanager {
	namespace parsing {
		class ProgramParser;
	}
	class ShaderProgram;
	class ShaderTraverser : public ::glslang::TIntermTraverser {
		friend class ShaderProgram;
	private:
		const std::reference_wrapper<parsing::ProgramParser> m_programParser;

		std::unordered_map<::glslang::TStorageQualifier, std::vector<Symbol>> m_symbols;

	public:
		explicit ShaderTraverser(parsing::ProgramParser& programParser) noexcept;

		virtual void visitSymbol(::glslang::TIntermSymbol* symbol) override;
	};
}
namespace sm = shadermanager;