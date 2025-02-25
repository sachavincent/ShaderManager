#include "stdafx.h"
#include "ShaderIncluder.h"

#include <fstream>
#include <fmt/format.h>

//GlslShaderIncluder::IncludeResult* GlslShaderIncluder::includeSystem(
//	const char* headerName, const char* includerName, size_t inclusionDepth) {
//	headerName;
//	includerName;
//	inclusionDepth;
//	// TODO: This should be used if a shader file says "#include <source>",
//	// in which case it includes a "system" file instead of a local file.
//   // log_error("GlslShaderIncluder::includeSystem() is not implemented!");
//   // log_error("includeSystem({}, {}, {})", headerName, includerName, inclusionDepth);
//	return nullptr;
//}

GlslShaderIncluder::IncludeResult* GlslShaderIncluder::includeLocal(
	const char* headerName, const char* includerName, size_t inclusionDepth) {
	headerName;
	includerName;
	inclusionDepth;

	auto includerItr = m_includeResults.find(headerName);
	if (includerItr != m_includeResults.end()) {
		return includerItr->second;
	}

	auto path = m_Shaderdir / headerName;

	std::ifstream filestream(path);

	if (!filestream) {
		auto err = fmt::format("Failed to load shader {}. File error.", path.string().c_str());
		m_errorStrings.insert({ headerName, err });
		return new IncludeResult(s_empty, err.data(), 0, nullptr);
	}
	if (filestream.peek() == std::ifstream::traits_type::eof()) {
		auto err = fmt::format("Failed to load shader {}. Empty string.", path.string().c_str());
		m_errorStrings.insert({ headerName, err });
		return new IncludeResult(s_empty, err.data(), 0, nullptr);
	}

	std::stringstream strStream;
	strStream << filestream.rdbuf();
	std::string str = strStream.str();

	m_sources[headerName] = str;

	IncludeResult* result = new IncludeResult(headerName, m_sources[headerName].data(), str.size(), m_sources[headerName].data());

	m_includeResults[headerName] = result;
	return result;
}

void GlslShaderIncluder::releaseInclude(IncludeResult* result) {
	if (!result)
		return;

	if (auto it = m_sources.find(result->headerName); it != m_sources.end()) {
		m_sources.erase(it);
	}

	if (auto it = m_includeResults.find(result->headerName); it != m_includeResults.end()) {
		m_includeResults.erase(it);
	}
	if (auto it = m_errorStrings.find(result->headerName); it != m_errorStrings.end()) {
		m_errorStrings.erase(it);
	}
	delete result;
}