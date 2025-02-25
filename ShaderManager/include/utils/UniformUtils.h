#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace shadermanager {
	namespace internal {
		template <typename...> inline constexpr bool always_false = false;

		template<glm::length_t N, typename T>
		struct FunctionSelector {
			static constexpr auto get() {
				static_assert(always_false<T>);
			}
		};
		template<glm::length_t C, glm::length_t R, typename T>
		struct FunctionSelectorMatrix {
			static constexpr auto get() {
				static_assert(always_false<T>);
			}
		};

		template<glm::length_t N>
		struct FunctionSelector<N, GLint> {
			static constexpr auto get() {
				if constexpr (N == 1)
					return &glUniform1i;
				else if constexpr (N == 2)
					return &glUniform2i;
				else if constexpr (N == 3)
					return &glUniform3i;
				else if constexpr (N == 4)
					return &glUniform4i;
				else
					static_assert(always_false<N>);
			}
		};

		template<glm::length_t N>
		struct FunctionSelector<N, bool> {
			static constexpr auto get() {
				if constexpr (N == 1)
					return &glUniform1i;
				else if constexpr (N == 2)
					return &glUniform2i;
				else if constexpr (N == 3)
					return &glUniform3i;
				else if constexpr (N == 4)
					return &glUniform4i;
				else
					static_assert(always_false<N>);
			}
		};

		template<glm::length_t N>
		struct FunctionSelector<N, GLint64> {
			static constexpr auto get() {
				if constexpr (N == 1)
					return &glUniform1i64ARB;
				else if constexpr (N == 2)
					return &glUniform2i64ARB;
				else if constexpr (N == 3)
					return &glUniform3i64ARB;
				else if constexpr (N == 4)
					return &glUniform4i64ARB;
				else
					static_assert(always_false<N>);
			}
		};

		template<glm::length_t N>
		struct FunctionSelector<N, GLfloat> {
			static constexpr auto get() {
				if constexpr (N == 1)
					return &glUniform1f;
				else if constexpr (N == 2)
					return &glUniform2f;
				else if constexpr (N == 3)
					return &glUniform3f;
				else if constexpr (N == 4)
					return &glUniform4f;
				else
					static_assert(always_false<N>);
			}
		};

		template<glm::length_t N>
		struct FunctionSelector<N, GLdouble> {
			static constexpr auto get() {
				if constexpr (N == 1)
					return &glUniform1d;
				else if constexpr (N == 2)
					return &glUniform2d;
				else if constexpr (N == 3)
					return &glUniform3d;
				else if constexpr (N == 4)
					return &glUniform4d;
				else
					static_assert(always_false<N>);
			}
		};

		template<glm::length_t N>
		struct FunctionSelector<N, GLuint> {
			static constexpr auto get() {
				if constexpr (N == 1)
					return &glUniform1ui;
				else if constexpr (N == 2)
					return &glUniform2ui;
				else if constexpr (N == 3)
					return &glUniform3ui;
				else if constexpr (N == 4)
					return &glUniform4ui;
				else
					static_assert(always_false<N>);
			}
		};

		template<glm::length_t N>
		struct FunctionSelector<N, GLuint64> {
			static constexpr auto get() {
				if constexpr (N == 1)
					return &glUniform1ui64ARB;
				else if constexpr (N == 2)
					return &glUniform2ui64ARB;
				else if constexpr (N == 3)
					return &glUniform3ui64ARB;
				else if constexpr (N == 4)
					return &glUniform4ui64ARB;
				else
					static_assert(always_false<N>);
			}
		};

		template<glm::length_t C, glm::length_t R>
		struct FunctionSelectorMatrix<C, R, GLfloat> {
			static constexpr auto get() {
				if constexpr (C == 2 && R == 2)
					return &glUniformMatrix2fv;
				else if constexpr (C == 2 && R == 3)
					return &glUniformMatrix2x3fv;
				else if constexpr (C == 2 && R == 4)
					return &glUniformMatrix2x4fv;
				else if constexpr (C == 3 && R == 2)
					return &glUniformMatrix3x2fv;
				else if constexpr (C == 3 && R == 3)
					return &glUniformMatrix3fv;
				else if constexpr (C == 3 && R == 4)
					return &glUniformMatrix3x4fv;
				else if constexpr (C == 4 && R == 2)
					return &glUniformMatrix4x2fv;
				else if constexpr (C == 4 && R == 3)
					return &glUniformMatrix4x3fv;
				else if constexpr (C == 4 && R == 4)
					return &glUniformMatrix4fv;
				else
					static_assert(always_false<C>);
			}
		};

		template<glm::length_t C, glm::length_t R>
		struct FunctionSelectorMatrix<C, R, GLdouble> {
			static constexpr auto get() {
				if constexpr (C == 2 && R == 2)
					return &glUniformMatrix2dv;
				else if constexpr (C == 2 && R == 3)
					return &glUniformMatrix2x3dv;
				else if constexpr (C == 2 && R == 4)
					return &glUniformMatrix2x4dv;
				else if constexpr (C == 3 && R == 2)
					return &glUniformMatrix3x2dv;
				else if constexpr (C == 3 && R == 3)
					return &glUniformMatrix3dv;
				else if constexpr (C == 3 && R == 4)
					return &glUniformMatrix3x4dv;
				else if constexpr (C == 4 && R == 2)
					return &glUniformMatrix4x2dv;
				else if constexpr (C == 4 && R == 3)
					return &glUniformMatrix4x3dv;
				else if constexpr (C == 4 && R == 4)
					return &glUniformMatrix4dv;
				else
					static_assert(always_false<C>);
			}
		};



		template<typename T, glm::length_t L>
		struct ParamUnpacker {
			static constexpr auto unpack(const glm::vec<L, T>& value) {
				static_assert(always_false<L>);
			}
		};

		template<typename T>
		struct ParamUnpacker<T, 1> {
			static constexpr auto unpack(const glm::vec<1, T>& value) {
				return std::make_tuple(value.x);
			}
		};

		template<typename T>
		struct ParamUnpacker<T, 2> {
			static constexpr auto unpack(const glm::vec<2, T>& value) {
				return std::make_tuple(value.x, value.y);
			}
		};

		template<typename T>
		struct ParamUnpacker<T, 3> {
			static constexpr auto unpack(const glm::vec<3, T>& value) {
				return std::make_tuple(value.x, value.y, value.z);
			}
		};

		template<typename T>
		struct ParamUnpacker<T, 4> {
			static constexpr auto unpack(const glm::vec<4, T>& value) {
				return std::make_tuple(value.x, value.y, value.z, value.w);
			}
		};
	}

	template<glm::length_t N, typename T>
	constexpr auto GetFunctionForType() {
		return internal::FunctionSelector<N, T>::get();
	}
	template<glm::length_t C, glm::length_t R, typename T>
	constexpr auto GetFunctionForTypeMatrix() {
		return internal::FunctionSelectorMatrix<C, R, T>::get();
	}
	template<typename T, glm::length_t L>
	constexpr auto Unpack(const glm::vec<L, T>& value) {
		return internal::ParamUnpacker<T, L>::unpack(value);
	}
}