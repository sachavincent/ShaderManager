#pragma once

#include <type_traits>
#include <glad/glad.h>
#include <utility>
#include <glslang/Public/ShaderLang.h>

namespace shadermanager {
	enum ShaderType {
		VERTEX = 0x01,
		FRAGMENT = 0x02,
		COMPUTE = 0x04,
		GEOMETRY = 0x08,
		TESSELATION_CONTROL = 0x10,
		TESSELATION_EVALUATION = 0x20,
		TASK = 0x40,
		MESH = 0x80
	};

	namespace opengl {
		constexpr GLuint GetOpenGLType(ShaderType type) {
			switch (type) {
			case ShaderType::VERTEX:
				return GL_VERTEX_SHADER;
			case ShaderType::FRAGMENT:
				return GL_FRAGMENT_SHADER;
			case ShaderType::COMPUTE:
				return GL_COMPUTE_SHADER;
			case ShaderType::GEOMETRY:
				return GL_GEOMETRY_SHADER;
			case ShaderType::TESSELATION_CONTROL:
				return GL_TESS_CONTROL_SHADER;
			case ShaderType::TESSELATION_EVALUATION:
				return GL_TESS_EVALUATION_SHADER;
			case ShaderType::TASK:
				return GL_TASK_SHADER_NV;
			case ShaderType::MESH:
				return GL_MESH_SHADER_NV;
			default:
				std::unreachable();
			}
		}
	}
	namespace glslang {
		constexpr EShLanguage GetStage(ShaderType type) {
			switch (type) {
			case ShaderType::VERTEX:
				return EShLanguage::EShLangVertex;
			case ShaderType::FRAGMENT:
				return EShLanguage::EShLangFragment;
			case ShaderType::COMPUTE:
				return EShLanguage::EShLangCompute;
			case ShaderType::GEOMETRY:
				return EShLanguage::EShLangGeometry;
			case ShaderType::TESSELATION_CONTROL:
				return EShLanguage::EShLangTessControl;
			case ShaderType::TESSELATION_EVALUATION:
				return EShLanguage::EShLangTessEvaluation;
			case ShaderType::TASK:
				return EShLanguage::EShLangTask;
			case ShaderType::MESH:
				return EShLanguage::EShLangMesh;
			default:
				std::unreachable();
			}
		}
	}
}

namespace sm = shadermanager;

template <>
struct magic_enum::customize::enum_range<sm::ShaderType> {
	static constexpr bool is_flags = true;
};