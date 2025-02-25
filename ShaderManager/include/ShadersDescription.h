#pragma once

#include "ShaderType.h"
#include <vector>
#include <filesystem>
#include <unordered_map>
#pragma warning (push)
#pragma warning (disable: 4244)
#include <glaze/json.hpp>
#include <glaze/core/wrappers.hpp>
#include <magic_enum/magic_enum_flags.hpp>

namespace shadermanager {
	struct ShaderDescription {
		magic_enum::containers::bitset<sm::ShaderType> type{};
		std::string name{};
		std::unordered_map<ShaderType, std::filesystem::path> files;

		void readFiles(std::unordered_map<std::string, std::filesystem::path>& f) {
			for (auto& [key, value] : f) {
				std::string upperCase;
				upperCase.resize(key.size());
				std::transform(key.begin(), key.end(), upperCase.begin(),
							   [](auto c) { return std::toupper(c); });
				auto t = magic_enum::enum_cast<sm::ShaderType>(upperCase);
				if (!t)
					return;
				files.insert({ *t, value });
				type.set(*t, true);
			}
		}
		std::unordered_map<std::string, std::filesystem::path> writeFiles() {
			return {};
		}
	};
	struct ShadersDescription {
		std::unordered_map<std::string, ShaderDescription> programs;

		void readFiles(std::vector<ShaderDescription>& f) {
			programs.reserve(f.size());
			for (auto&& desc : f) {
				programs.insert({ desc.name, std::move(desc) });
			}
		}
		std::vector<ShaderDescription> writeFiles() {
			return {};
		}
	};
}

namespace sm = shadermanager;

template <>
struct glz::meta<sm::ShaderDescription> {
	static constexpr auto value = object(
		"type", hide{ &sm::ShaderDescription::type },
		"name", &sm::ShaderDescription::name,
		"files", custom<&sm::ShaderDescription::readFiles, &sm::ShaderDescription::writeFiles>
	);
};

template <>
struct glz::meta<sm::ShadersDescription> {
	static constexpr auto value = object(
		"programs", custom<&sm::ShadersDescription::readFiles, &sm::ShadersDescription::writeFiles>
	);
};

#pragma warning (pop)