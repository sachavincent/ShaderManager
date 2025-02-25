#pragma once

#include "../ShaderType.h"

namespace shadermanager {
	namespace utils {
		inline GLint GetMaximumStorageBlocks(ShaderType shaderType) {
			/*if (shaderType == ShaderType::UTILITY)
				return 0;*/

			const GLenum pname = [](ShaderType shaderType) {
				switch (shaderType) {
				case ShaderType::VERTEX:
					return GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS;
				case ShaderType::FRAGMENT:
					return GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS;
				case ShaderType::COMPUTE:
					return GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS;
				case ShaderType::GEOMETRY:
					return GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS;
				case ShaderType::TESSELATION_CONTROL:
					return GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS;
				case ShaderType::TESSELATION_EVALUATION:
					return GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS;
				case ShaderType::TASK:
					return GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS;
				case ShaderType::MESH:
					return GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS;
				default:
					std::unreachable();
				}
			}(shaderType);

			GLint val;
			glGetIntegerv(pname, &val);
			return val;
		}

		inline bool HasNoOpenGLError() {
#ifdef ENABLE_OPENGL_ERROR_CHECKS
			return glGetError() == GL_NO_ERROR;
#else
			return true;
#endif
		}
		inline GLint OpenGLError() {
#ifdef ENABLE_OPENGL_ERROR_CHECKS
			return glGetError();
#else
			return GL_NO_ERROR;
#endif
		}
		inline std::string_view LogOpenGLError(GLint error) {
			static std::unordered_map<int, const char*> ERRORS = {
				{ GL_NO_ERROR, "" },
				{ GL_INVALID_ENUM, "INVALID_ENUM" },
				{ GL_INVALID_VALUE, "INVALID_VALUE" },
				{ GL_INVALID_OPERATION, "INVALID_OPERATION" },
				{ GL_INVALID_FRAMEBUFFER_OPERATION, "INVALID_FRAMEBUFFER_OPERATION" },
				{ GL_OUT_OF_MEMORY, "OUT_OF_MEMORY" },
				{ GL_STACK_UNDERFLOW, "STACK_UNDERFLOW" },
				{ GL_STACK_OVERFLOW, "STACK_OVERFLOW" }
			};
			return ERRORS[error];
		}
		// Check for OpenGL errors and log them if present
		inline bool CheckOpenGLError(sm::Error& error) {
			GLint errorCode = sm::utils::OpenGLError();
			if (errorCode != GL_NO_ERROR) {
				error.set(sm::ErrorCode::OPENGL_ERROR, sm::utils::LogOpenGLError(errorCode));
				return true;
			}
			return false;
		}
	}
}