#include "stdafx.h"
#include "ShaderParser.h"

#include "utils/ParserUtils.h"
#include "utils/OpenGLConstants.h"
#include "ShaderIncluder.h"
#include "ShaderTraverser.h"
#include "CustomBufferBindingHandler.h"

#include <glad/glad.h>
#include <fmt/format.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/resource_limits_c.h>
#include <glslang/Include/glslang_c_interface.h>

#include <fstream>
#include <ranges>
#include <regex>
#include <magic_enum/magic_enum.hpp>

namespace shadermanager {
	namespace parsing {
		void Parser::Initialize() {
			constexpr auto types = magic_enum::enum_values<ShaderType>();
			for (ShaderType type : types) {
				m_maximumBlocksAllowed[type] = shadermanager::utils::GetMaximumStorageBlocks(type);
			}
		}
		[[nodiscard]] ProgramParser Parser::CreateProgramParser() {
			return ProgramParser(*this);
		}

		[[nodiscard]] std::expected<ShaderBindingIndex, Error> Parser::GetNextBufferBinding() {
			auto index = m_nextBindingIndex;


			++m_nextBindingIndex;

			return index;
		}

		ProgramParser::ProgramParser(Parser& parser) noexcept : m_traverser(*this), m_parser(parser) {}

		[[nodiscard]] std::expected<ShaderFileContent, Error> ProgramParser::ParseShader(const ShaderFile& shaderFile) {
			std::ifstream filestream(shaderFile.GetPath());

			if (!filestream) {
				return std::unexpected(Error{ ErrorCode::UNKNOWN_SHADER });
			}
			if (filestream.peek() == std::ifstream::traits_type::eof()) {
				return std::unexpected(Error{ ErrorCode::INVALID_SHADER_CONTENT });
			}

			std::stringstream strStream;
			strStream << filestream.rdbuf();
			std::string str = strStream.str();

			auto path = shaderFile.GetPath();

			// Or GL_ARB_shading_language_include

			std::string src = std::string("#version 460\n") + std::string("#extension GL_ARB_shading_language_include : require\n") + str;
			const char* sources[1] = { src.data() };

			EShMessages result = EShMsgDefault;
			result = static_cast<EShMessages>(result | EShMsgSpvRules);
			result = static_cast<EShMessages>(result | EShMsgKeepUncalled);
			result = static_cast<EShMessages>(result | EShMsgHlslLegalization);
			result = static_cast<EShMessages>(result | EShMsgDebugInfo);

			auto stage = glslang::GetStage(shaderFile.GetType());
			::glslang::TShader shader(stage);
			shader.setStrings(sources, 1);
			shader.setAutoMapLocations(true);
			shader.setDebugInfo(true);
			shader.setAutoMapBindings(true);
			shader.setTextureSamplerTransformMode(EShTextureSamplerTransformMode::EShTexSampTransKeep);
			shader.setEnvInput(::glslang::EShSourceGlsl, stage, ::glslang::EShClientOpenGL, 450);
			shader.setEnvClient(::glslang::EShClientOpenGL, ::glslang::EShTargetOpenGL_450);
			shader.setEnvTarget(::glslang::EShTargetSpv, ::glslang::EShTargetSpv_1_0);

			const TBuiltInResource* resources = GetDefaultResources();
			const int defaultVersion = 460;
			const bool forwardCompatible = false;
			EProfile defaultProfile = ECoreProfile;

			GlslShaderIncluder includer(path.parent_path());
			auto& handler = CustomBufferBindingHandler::GetInstance();
			std::string preprocessedStr;
			if (!shader.preprocess(resources, defaultVersion, defaultProfile, false, forwardCompatible, result, &preprocessedStr, includer, &handler)) {
				return std::unexpected(Error{ ErrorCode::PREPROCESSING_SHADER, shader.getInfoLog() });
			}
			const char* preprocessedSources[1] = { preprocessedStr.c_str() };
			shader.setStrings(preprocessedSources, 1);

			if (!shader.parse(resources, defaultVersion, forwardCompatible, result, includer)) {
				return std::unexpected(Error{ ErrorCode::PARSING_SHADER, shader.getInfoLog() });
			}

			auto root = shader.getIntermediate()->getTreeRoot();
			root->traverse(&m_traverser);

			return preprocessedStr;
		}
	}
}