#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <memory_resource>
#include "ShaderProgram.h"
#include "ShaderParser.h"
#include "ShadersDescription.h"

namespace shadermanager {
	namespace utils {
		class FileWatcher;
	}
	class AbstractShaderBuffer {};

	template <typename T>
	concept ShaderBuffer = std::is_base_of_v<AbstractShaderBuffer, T>;

	class ShaderManager {
		friend class Shader;
		friend class ShaderProgram;
		friend class utils::FileWatcher;
	private:
		std::unordered_map<std::string, ShaderProgram> m_programs;
		std::unordered_map<ShaderPath, Shader> m_shaders;

		std::unordered_map<std::type_index, std::unique_ptr<AbstractShaderBuffer>> m_buffers;

		std::array<char, 1024> m_buffer{};
		std::pmr::monotonic_buffer_resource m_mbr{ m_buffer.data(), m_buffer.size() };
		std::pmr::string m_readBuffer{ &m_mbr };

		ShadersDescription m_desc;
		std::filesystem::path m_shaderRoot;

		parsing::Parser m_parser{};

	public:
		ShaderManager() {}

		[[nodiscard]] std::expected<ShaderProgram*, Error> RegisterShaderProgram(const std::string& shaderProgramName);

		[[nodiscard]] parsing::Parser& GetParser();

		void AddShaderDescriptionFile(const std::filesystem::path& path);

		[[nodiscard]] const std::filesystem::path& ShaderRoot() const;

		void Clean();
	};
}

namespace sm = shadermanager;