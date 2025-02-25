#include "stdafx.h"
#include "utils/SetupOpenGL.h"

#include <UniformsHandler.h>
#include <ShaderManager.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace sm;

class ShaderManagerTests : public ::testing::Test {
protected:
	inline static std::unique_ptr<sm::ShaderManager>	s_shaderManager{};
	inline static sm::parsing::Parser*					s_shaderParser{};
	inline static std::filesystem::path					s_filePath{};

	ShaderManagerTests() = default;

	virtual ~ShaderManagerTests() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	static void SetUpTestSuite() {
		s_shaderManager = std::make_unique<sm::ShaderManager>();

		s_shaderParser = &s_shaderManager->GetParser();
		char* buf = nullptr;
		size_t sz = 0;
		if (_dupenv_s(&buf, &sz, "TESTS_ASSETS_DIR") == 0 && buf != nullptr) {
			s_filePath = std::string(buf) + std::string("\\ShaderManagerTests\\Descriptors\\");
			free(buf);
		}
		s_shaderManager->AddShaderDescriptionFile(s_filePath / "test1.json");

		::utils::OpenGLWrapper::PrepareOpenGL();
		s_shaderParser->Initialize();
	}

	static void TearDownTestSuite() {}

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	const std::unordered_map<std::string_view, GLint>& GetLocations(const UniformsHandler& handler) {
		return handler.m_locations;
	}
};

TEST_F(ShaderManagerTests, Error_InvalidShaderProgram) {
	auto result = s_shaderManager->RegisterShaderProgram("");
	ASSERT_THAT(result.has_value(), IsFalse());
	ASSERT_THAT(result.error().getCodes().test(ErrorCode::INVALID_PROGRAM), IsTrue());
}

TEST_F(ShaderManagerTests, Error_InexistantShaderProgram) {
	auto result = s_shaderManager->RegisterShaderProgram("none");
	ASSERT_THAT(result.has_value(), IsFalse());
	ASSERT_THAT(result.error().getCodes().test(ErrorCode::UNKNOWN_PROGRAM), IsTrue());
}

TEST_F(ShaderManagerTests, Error_UnknownShaders) {
	auto result = s_shaderManager->RegisterShaderProgram("testFileUnknown");
	ASSERT_THAT(result.has_value(), IsFalse());
	ASSERT_THAT(result.error().getCodes().test(ErrorCode::UNKNOWN_SHADER), IsTrue());
}

TEST_F(ShaderManagerTests, Success_NoUniform) {
	auto result = s_shaderManager->RegisterShaderProgram("test");
	ASSERT_THAT(result.has_value(), IsTrue());

	ASSERT_THAT(result.value()->AttachToShaders(), IsTrue());
	ASSERT_THAT(result.value()->Link(), IsTrue());
	const auto& locations = GetLocations(result.value()->Handler());
	ASSERT_THAT(locations, IsEmpty());
	ASSERT_THAT(sm::utils::ErrorHandler::HasErrors(**result), IsFalse());
}


TEST_F(ShaderManagerTests, Error_PrimitiveUniformSameNameDifferentTypesInBothShaders) {
	auto result = s_shaderManager->RegisterShaderProgram("testU2");
	ASSERT_THAT(result.has_value(), IsTrue());

	ASSERT_THAT(result.value()->AttachToShaders(), IsTrue());
	ASSERT_THAT(result.value()->Link(), IsFalse());
	ASSERT_THAT(sm::utils::ErrorHandler::HasErrors(**result), IsTrue());
}

TEST_F(ShaderManagerTests, Success_PrimitiveUnusedSameUniformsInBothShaders) {
	auto result = s_shaderManager->RegisterShaderProgram("testU1");
	ASSERT_THAT(result.has_value(), IsTrue());

	ASSERT_THAT(result.value()->AttachToShaders(), IsTrue());
	ASSERT_THAT(result.value()->Link(), IsTrue());
	ASSERT_THAT(sm::utils::ErrorHandler::HasErrors(**result), IsFalse());
	const auto& locations = GetLocations(result.value()->Handler());
	ASSERT_THAT(locations, IsEmpty());
}

TEST_F(ShaderManagerTests, Success_PrimitiveSameUniformsInBothShaders) {
	auto result = s_shaderManager->RegisterShaderProgram("testU3");
	ASSERT_THAT(result.has_value(), IsTrue());

	ASSERT_THAT(result.value()->AttachToShaders(), IsTrue());
	ASSERT_THAT(result.value()->Link(), IsTrue());
	const auto& locations = GetLocations(result.value()->Handler());
	ASSERT_THAT(locations, SizeIs(1));
	ASSERT_THAT(sm::utils::ErrorHandler::HasErrors(**result), IsFalse());
}

TEST_F(ShaderManagerTests, Success_PrimitiveSameBuffersInBothShadersWithExplicitBinding) {
	auto result = s_shaderManager->RegisterShaderProgram("testU4");
	ASSERT_THAT(result.has_value(), IsTrue());

	ASSERT_THAT(result.value()->AttachToShaders(), IsTrue());
	ASSERT_THAT(result.value()->Link(), IsTrue());
	ASSERT_THAT(sm::utils::ErrorHandler::HasErrors(**result), IsFalse());
}