#pragma once

#include <string>
#include <vector>

std::size_t __int_count_of_symboles(std::string_view text, char symbol = ' ') noexcept;
std::size_t __simd128_count_of_symboles(std::string_view text) noexcept;
std::size_t __simd256_count_of_symboles(std::string_view text, char symbol = ' ') noexcept;
std::vector<std::string_view> daz_split(std::string_view text, char delimiter = ' ') noexcept;
std::vector<std::string_view> daz_split_2(std::string_view text, char delimiter = ' ') noexcept;
std::vector<std::string_view> stl_split(std::string_view text);
std::vector<std::string_view> simd_split(std::string_view text, char delimiter = ' ');
std::vector<std::string_view> danya_split(std::string_view str, std::string_view delim) noexcept;
std::vector<std::string_view> danya_split_2(std::string_view str, char delim = ' ') noexcept;
std::vector<std::string_view> eva_split(std::string_view str, char del);
