#pragma once

#include <glslang/Public/ShaderLang.h>
#include <unordered_map>
#include <filesystem>

class GlslShaderIncluder : public glslang::TShader::Includer {
public:
	explicit GlslShaderIncluder(const std::filesystem::path& shaderDir) : m_Shaderdir(shaderDir) {}

	// Note "local" vs. "system" is not an "either/or": "local" is an
	// extra thing to do over "system". Both might get called, as per
	// the C++ specification.
	//
	// For the "system" or <>-style includes; search the "system" paths.
	//virtual IncludeResult* includeSystem(const char* headerName, const char* includerName, size_t inclusionDepth) override;

	// For the "local"-only aspect of a "" include. Should not search in the
	// "system" paths, because on returning a failure, the parser will
	// call includeSystem() to look in the "system" locations.n
	virtual IncludeResult* includeLocal(const char* headerName, const char* includerName, size_t inclusionDepth) override;

	virtual void releaseInclude(IncludeResult*) override;

private:
	static constexpr const char* s_empty = "";

	std::filesystem::path m_Shaderdir{};
	std::unordered_map<std::string, IncludeResult*> m_includeResults;
	std::unordered_map<std::string, std::string> m_sources;
	std::unordered_map<std::string, std::string> m_errorStrings;
};