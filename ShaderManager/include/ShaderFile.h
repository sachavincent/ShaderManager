#pragma once

#include "ShaderType.h"
#include <filesystem>
#include <xhash>

namespace shadermanager {
	using ShaderFilePath = std::filesystem::path;
	using ShaderFileContent = std::string;
	//using Error = std::string;

	class ShaderFile {
	private:
		ShaderFilePath m_path;
		ShaderType m_type;

	public:
		ShaderFile(const ShaderFilePath& filePath, ShaderType type) : m_path(filePath), m_type(type) {}

		const ShaderFilePath& GetPath() const { return m_path; }

		ShaderType GetType() const { return m_type; }

		bool operator==(const ShaderFile& rhs) const noexcept {
			return m_path == rhs.m_path;
		}
	};
}
namespace sm = shadermanager;

namespace std {
	template<>
	struct hash<sm::ShaderFile> {
		std::size_t operator()(const sm::ShaderFile& s) const noexcept {
			return std::hash<sm::ShaderFilePath>{}(s.GetPath());
		}
	};
}