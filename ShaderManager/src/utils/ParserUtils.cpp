#include "stdafx.h"
#include "utils/ParserUtils.h"

#include <ranges>
#include <locale>

namespace shadermanager {
	namespace parsing {
		namespace utils {
			/**
			 * @brief This function is used to find the starting indices of each instance of substring in str
			 * @param str the string/haystack
			 * @param substring the substring/needle
			 * @return start indices of each instance of substring in str, empty if none found
			 */
			[[nodiscard]] std::vector<std::size_t> FindPositionsOfSubstring(std::string_view str, std::string_view substring) {
				if (substring.empty() || str.empty() || str.size() < substring.size())
					return {}; // Early return

				std::vector<std::size_t> result;
				std::string_view string = str;
				std::size_t offset = 0;
				while (!string.empty()) {
					// std::ranges::search returns the first match of substring in the string
					const auto firstMatch = std::ranges::search(string, substring);
					if (firstMatch.empty()) {
						// No more matches
						break;
					}

					// Start index of the first match
					const auto start = std::distance(string.begin(), firstMatch.begin());

					// At start index to the result
					result.emplace_back(start + offset);

					// Update the string by removing all the characters up until the first match plus one character
					string = string.substr(start + 1);
					// Add the number of deleted characters to the offset
					offset += start + 1;
				}

				return result;
			}

			void TrimString(std::string& str) {
				constexpr auto not_space = [](unsigned char c) { return !std::isspace(c); };

				// erase the the spaces at the back first
				// so we don't have to do extra work
				str.erase(std::ranges::find_if(str | std::views::reverse, not_space).base(), str.end());

				// erase the spaces at the front
				str.erase(str.begin(), std::ranges::find_if(str, not_space));
			}

			bool CheckIncludeFormat(std::string_view str) {
				return std::regex_match(str.data(), include_regex);
			}
		}
	}
}