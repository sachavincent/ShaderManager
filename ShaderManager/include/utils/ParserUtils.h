#pragma once

#include <vector>
#include <string_view>
#include <regex>

namespace shadermanager {
	namespace parsing {
		namespace utils {
			namespace structs {
				struct ShaderJSON {

				};
			}
			constexpr const std::string_view include_str = "#include";
			const std::regex include_regex("^\\s*#include\\s+\"[^ \\s\"]+\"\\s*\\n$");
			/**
			 * @brief This function is used to find the starting indices of each instance of substring in str
			 * @param str the string/haystack
			 * @param substring the substring/needle
			 * @return start indices of each instance of substring in str, empty if none found
			 */
			[[nodiscard]] std::vector<std::size_t> FindPositionsOfSubstring(std::string_view str, std::string_view substring);

			void TrimString(std::string& str);

			bool CheckIncludeFormat(std::string_view str);
		}
	}
}
namespace sm = shadermanager;