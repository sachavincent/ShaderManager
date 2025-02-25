#pragma once

#include <glslang/Public/ShaderLang.h>
#include <set>

class CustomBufferBindingHandler : public glslang::TShader::BufferBindingHandler {
private:
	CustomBufferBindingHandler() = default;

	int m_nextLocation = 0;
	std::set<int> m_usedLocations;

	std::unordered_map<std::string, int> m_customBindings;

	int GetNextLocationIndex(const std::string& tokenLocation) {
		if (m_customBindings.contains(tokenLocation)) {
			return m_customBindings[tokenLocation];
		}

		while (m_usedLocations.contains(m_nextLocation++));

		const int res = m_nextLocation - 1;
		m_usedLocations.insert(res);
		m_customBindings[tokenLocation] = res;
		return res;
	}

public:
	static CustomBufferBindingHandler& GetInstance() {
		static CustomBufferBindingHandler instance;
		return instance;
	}

	virtual ~CustomBufferBindingHandler() = default;

	virtual int GetLocation(const std::string& tokenLocation) override {
		if (tokenLocation.empty())
			return -1;

		if (tokenLocation[0] == '$') {
			return GetNextLocationIndex(tokenLocation);
		}

		char* end;
		long value = std::strtol(tokenLocation.c_str(), &end, 10);

		// Check if no conversion was performed or if there are leftover characters
		if (end == tokenLocation.c_str() || *end != '\0')
			return -1;

		m_usedLocations.insert(value);
		return static_cast<int>(value);
	}
};