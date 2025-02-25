#include "stdafx.h"
#include "ShaderTraverser.h"
#include "ShaderProgram.h"

namespace shadermanager {
	ShaderTraverser::ShaderTraverser(parsing::ProgramParser& programParser) noexcept : m_programParser(programParser),
		::glslang::TIntermTraverser(true, true, true) {}

	/*ShaderTraverser::~ShaderTraverser() noexcept {
		m_programParser.get().m_symbols = std::move(m_symbols);
	}*/

	void ShaderTraverser::visitSymbol(::glslang::TIntermSymbol* symbol) {
		if (!symbol) return;

		m_symbols[symbol->getQualifier().storage].emplace_back(symbol);
	}
}