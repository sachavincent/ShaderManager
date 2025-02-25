#pragma once

#include "utils/Symbol.h"
#include "utils/UniformUtils.h"
#include "utils/ErrorHandler.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <expected>

class ShaderManagerTests;
namespace shadermanager {
	class ShaderProgram;
	class UniformsHandler {
		friend class ::ShaderManagerTests;
	private:
		const ShaderProgram* m_program{};

		mutable std::unordered_map<std::string_view, GLint> m_locations;

	private:
		[[nodiscard]] GLint GetUniformLocation(std::string_view name) const;

		UniformsHandler(const ShaderProgram& program, std::unordered_map<std::string_view, GLint>&& locations) noexcept;

	public:
		UniformsHandler() noexcept = default;

		[[nodiscard]] static std::expected<UniformsHandler, Error> CreateHandler(const ShaderProgram& program, const std::vector<Symbol>& symbols);

		template<typename T> requires(std::is_arithmetic_v<T>)
		void LoadValue(std::string_view name, T value) const {
			auto f = GetFunctionForType<1, T>();
			(*f)(GetUniformLocation(name), value);
		}

		template<typename T, glm::length_t L>
		void LoadVector(std::string_view name, const glm::vec<L, T>& value) const {
			auto f = GetFunctionForType<L, T>();
			std::apply(*f, std::tuple_cat(std::make_tuple(GetUniformLocation(name)), Unpack<T, L>(value)));
		}

		template<typename T, glm::length_t C, glm::length_t R>
		void LoadMatrix(std::string_view name, glm::mat<C, R, T> value) const {
			auto f = GetFunctionForTypeMatrix<C, R, T>();
			(*f)(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
		}
	};
}