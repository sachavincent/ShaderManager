#pragma once

#include <string>
#include <unordered_map>

#undef NO_ERROR
namespace shadermanager {
	enum class ErrorCode {
		NO_ERROR, //= 0x0,
		INVALID_PROGRAM, //= 0x01,
		UNKNOWN_PROGRAM, //= 0x02,
		DUPLICATE_PROGRAM, //= 0x04,
		UNKNOWN_SHADER, //= 0x08,
		INVALID_SHADER_CONTENT, //= 0x10,
		PREPROCESSING_SHADER, //= 0x20,
		PARSING_SHADER, //= 0x40,
		WRONG_PROGRAM_STATUS, //= 0x80,
		COMPILE_SHADER, //= 0x100,
		LINK_SHADER, //= 0x200,
		OPENGL_ERROR, //= 0x400,
		CLASHING_UNIFORM_TYPES, //= 0x400,
	};

	class Error {
	private:
		magic_enum::containers::bitset<ErrorCode> codes;
		std::unordered_map<ErrorCode, std::string> descriptions;

	public:
		Error() { codes.set(ErrorCode::NO_ERROR); }
		Error(ErrorCode code, std::string_view description = "") { set(code, description); }

		// Clear all errors and set to no-error state
		void clear() {
			codes.reset();
			descriptions.clear();
			codes.set(ErrorCode::NO_ERROR);
		}

		// Add an error with optional description
		void set(ErrorCode code, std::string_view description = "") {
			if (code != ErrorCode::NO_ERROR) {
				codes.reset(ErrorCode::NO_ERROR);  // Clear NO_ERROR when adding an error
				codes.set(code);
				if (!description.empty()) {
					descriptions[code] = std::string(description);
				}
			}
		}

		// Check if any errors are present
		bool hasError() const {
			return !codes.test(ErrorCode::NO_ERROR) && !codes.none();
		}

		// Get all error descriptions concatenated
		std::string getFullDescription() const {
			if (!hasError()) return "No errors";

			std::string result;
			// Iterate through all possible error codes
			for (auto code : magic_enum::enum_values<ErrorCode>()) {
				if (code != ErrorCode::NO_ERROR && codes.test(code)) {
					result += std::string(magic_enum::enum_name(code));
					auto it = descriptions.find(code);
					if (it != descriptions.end()) {
						result += ": " + it->second;
					}
					result += "\n";
				}
			}
			return result;
		}

		// Get specific error description
		std::optional<std::string> getDescription(ErrorCode code) const {
			auto it = descriptions.find(code);
			return it != descriptions.end() ? std::optional{ it->second } : std::nullopt;
		}

		// Check for specific error
		bool has(ErrorCode code) const {
			return codes.test(code);
		}

		// Get all error codes
		const auto& getCodes() const { return codes; }
	};

	class ShaderProgram;
	namespace utils {
		class ErrorHandler {
		public:
			static bool HasErrors(const ShaderProgram&);

			static const Error& Errors(const ShaderProgram&);
		};
	}
}
