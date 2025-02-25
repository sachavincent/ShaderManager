#pragma once

#define NOMINMAX
#define _HAS_EXCEPTIONS 0
#define ENABLE_OPENGL_ERROR_CHECKS // TMP

#include <Windows.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <fmt/format.h>
#pragma warning (push)
#pragma warning (disable: 4244)
#include <glaze/json.hpp>
#include <glaze/core/wrappers.hpp>
#include <magic_enum/magic_enum_flags.hpp>
#pragma warning (pop)
#include <string_view>
#include <regex>
#include <expected>
#include <bitset>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <cstdint>
#include <print>
#include <array>
#include <filesystem>
