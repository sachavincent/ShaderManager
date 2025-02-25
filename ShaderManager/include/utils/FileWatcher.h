#pragma once

#include "utils/Constants.h"

namespace shadermanager {
	class ShaderManager;
	class ShaderFile;

	namespace utils {
		class FileWatcher {
		private:
			struct Watch {
				std::filesystem::file_time_type time;
				ShaderPath path;
			};
			std::vector<Watch> m_watchList;
			const std::reference_wrapper<ShaderManager> m_manager;

		private:
			void UpdateShader(const ShaderPath& shaderPath) const;

		public:
			FileWatcher(ShaderManager& manager) noexcept;

			void AddFile(const ShaderFile& shaderFile);

			void PollChanges() const;
		};
	}
}
namespace sm = shadermanager;