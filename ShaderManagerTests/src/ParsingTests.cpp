#include "stdafx.h"
#include "utils/SetupOpenGL.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ShaderManager.h>

using namespace ::testing;

class ParsingTests : public ::testing::Test {
protected:
	sm::ShaderManager m_shaderManager;
	sm::parsing::Parser& m_shaderParser;
	std::string m_filePath;

	ParsingTests() : m_shaderParser(m_shaderManager.GetParser()) {
		char* buf = nullptr;
		size_t sz = 0;
		if (_dupenv_s(&buf, &sz, "TESTS_ASSETS_DIR") == 0 && buf != nullptr) {
			m_filePath = std::string(buf) + std::string("\\ParsingTests\\");
			free(buf);
		}

		utils::OpenGLWrapper::PrepareOpenGL();

		m_shaderParser.Initialize();
	}
	virtual ~ParsingTests() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	auto ParseShaderUtils(const std::string& shaderStr) {
		//auto programExp = sm::ShaderProgram::CreateShaderProgram(m_shaderManager, "", m_shaderParser, sm::ShaderDescription{
		//						  .type = {sm::ShaderType::VERTEX},
		//						  .name = "test",
		//						  .files = {{sm::ShaderType::VERTEX, shaderStr}}
		//						  });
		//ASSERT_THAT(programExp.has_value(), IsTrue());
		sm::parsing::ProgramParser parser(m_shaderParser);
		return parser.ParseShader(sm::ShaderFile(std::filesystem::path(m_filePath + shaderStr), sm::ShaderType::VERTEX));
	}
};

TEST_F(ParsingTests, UnknownShaderError) {
	auto result = ParseShaderUtils("#.glsl");
	ASSERT_THAT(result.has_value(), IsFalse());
}

TEST_F(ParsingTests, EmptyShaderError) {
	auto result = ParseShaderUtils("empty.glsl");
	ASSERT_THAT(result.has_value(), IsFalse());
}

TEST_F(ParsingTests, BasicSuccess) {
	auto result = ParseShaderUtils("definitions.glsl");
	ASSERT_THAT(result.has_value(), IsTrue());
}

TEST_F(ParsingTests, IncludePropagationError) {
	auto result = ParseShaderUtils("includePropagationError.glsl");
	ASSERT_THAT(result.has_value(), IsFalse());
}

TEST_F(ParsingTests, IncludeOnlySuccess) {
	auto result = ParseShaderUtils("includeOnlySuccess.glsl");
	ASSERT_THAT(result.has_value(), IsTrue());
}

TEST_F(ParsingTests, IncludeTwoFilesOnlySuccess) {
	auto result = ParseShaderUtils("includeTwoFilesOnlySuccess.glsl");
	ASSERT_THAT(result.has_value(), IsTrue());
}

TEST_F(ParsingTests, IncludeTwoFilesOnlyWithOneCachedSuccess) {
	// Caching definitions.glsl
	std::ignore = ParseShaderUtils("definitions.glsl");

	auto result = ParseShaderUtils("includeTwoFilesOnlySuccess.glsl");
	ASSERT_THAT(result.has_value(), IsTrue());
}

TEST_F(ParsingTests, IncludeWithContent) {
	auto result = ParseShaderUtils("includeWithContent.glsl");
	ASSERT_THAT(result.has_value(), IsTrue());
}

TEST_F(ParsingTests, IncludeTwoWithContent) {
	auto result = ParseShaderUtils("includeTwoWithContent.glsl");
	ASSERT_THAT(result.has_value(), IsTrue());
}

TEST_F(ParsingTests, IncludeWithContentMixed) {
	auto result = ParseShaderUtils("includeWithContentMixed.glsl");
	ASSERT_THAT(result.has_value(), IsTrue());
}

TEST_F(ParsingTests, CustomBufferBinding) {
	auto result = ParseShaderUtils("buffer_only.glsl");
	ASSERT_THAT(result.has_value(), IsTrue());
}
