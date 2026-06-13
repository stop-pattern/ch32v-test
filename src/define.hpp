#pragma once

/// @brief Version number in the format 0xMMmmpp (Major, minor, patch)
/// @note v1.0.0
constexpr int VERSION = 0x100;

/// @brief Major version
constexpr int VERSION_MAJOR = (VERSION >> 16) & 0xFF;
/// @brief Minor version
constexpr int VERSION_MINOR = (VERSION >> 8) & 0xFF;
/// @brief Patch version
constexpr int VERSION_PATCH = VERSION & 0xFF;

/// @brief CPUクロック周波数
/// @note 144MHz
constexpr uint32_t F_CPU = 144000000;
