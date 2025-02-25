#include "stdafx.h"
#include "utils/ErrorHandler.h"

#include "ShaderProgram.h"

namespace shadermanager {
	namespace utils {
		bool ErrorHandler::HasErrors(const ShaderProgram& program) {
			return program.error.hasError();
		}
		const Error& ErrorHandler::Errors(const ShaderProgram& program) {
			return program.error;
		}
	}
}