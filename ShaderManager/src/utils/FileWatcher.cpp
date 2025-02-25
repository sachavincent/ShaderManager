#include "stdafx.h"

#include "utils/FileWatcher.h"
#include "ShaderFile.h"
#include "ShaderManager.h"

namespace shadermanager {
	namespace utils {
		FileWatcher::FileWatcher(ShaderManager& manager) noexcept : m_manager(manager) {}

		void FileWatcher::AddFile(const ShaderFile& shaderFile) {
			m_watchList.emplace_back(std::filesystem::last_write_time(shaderFile.GetPath()), shaderFile.GetPath());
		}

		void FileWatcher::PollChanges() const {
			for (auto& file : m_watchList) {
				auto newTime = std::filesystem::last_write_time(file.path);
				if (newTime != file.time) {
					// File change detected
					UpdateShader(file.path);
				}
			}
		}

		void FileWatcher::UpdateShader(const ShaderPath& shaderPath) const {
			const auto& shaderNode = m_manager.get().m_shaders.extract(shaderPath);
			if (shaderNode.empty())
				return;

			auto& shader = shaderNode.mapped();
			std::unordered_set<ShaderProgram*> programsCache = shader.m_programs;
			
			if (!shader.Clean())
				return;

			auto parser = m_manager.get().GetParser().CreateProgramParser();
			auto exp = Shader::CreateShader(m_manager.get(), shaderPath, parser, shader.m_shaderType);
			if (!exp)
				return;
			auto* newShader = *exp;

			for (auto& program : programsCache) {
				std::ignore = shader.AttachTo(*program);
				program->m_shaders[shader.m_shaderType] = newShader;
				std::ignore = program->Link();
			}
		}
	}
}