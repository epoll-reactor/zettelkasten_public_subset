#include <x86intrin.h>

#include <iostream>
#include <numeric>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <array>
#include <bitset>
#include <string.h>

#include "simd.hpp"


std::size_t __int_count_of_symboles(std::string_view text, char symbol) noexcept
{
  std::size_t spaces = 0;
  std::size_t i = 0;
  while (text[i++] != '\0')
  {
    if (text[i] == symbol) spaces++;
  }
  return spaces;
}

std::size_t __simd128_count_of_spaces(std::string_view text) noexcept
{
  std::size_t spaces = 0;
  std::size_t length = text.length();

  if (length < 16) return __int_count_of_symboles(text);

  const __m128i __cmp = _mm_set1_epi8(' ');
  for (std::uint64_t i = 0; i < length; i += 16)
  {
    __m128i __chunk = _mm_loadu_si128((__m128i*)(text.substr(i, 16).data()));
    __m128i __eq = _mm_cmpeq_epi8(__chunk, __cmp);
    spaces += _popcnt32(_mm_movemask_epi8(__eq));
  }
  return spaces;
}

std::size_t __simd256_count_of_symboles(std::string_view text, char symbol) noexcept {
  std::size_t spaces = 0;
  const std::size_t length = text.length();
  if (length < 32)
    return __int_count_of_symboles(text, symbol);
  static const __m256i cmpmask = _mm256_set1_epi8(symbol);
  for (std::uint64_t i = 0; i < length; i += 32) {
    __m256i chunk = _mm256_loadu_si256((__m256i*)(text.substr(i, 32).data()));
    __m256i result = _mm256_cmpeq_epi8(chunk, cmpmask);
    spaces += _popcnt32(_mm256_movemask_epi8(result));
  }

  return spaces;
}

std::vector<std::string_view> stl_split(std::string_view text)
{
  std::istringstream stream(text.data());
  return std::vector<std::string_view>{
    std::istream_iterator<std::string>(stream), { }
  };
}

static std::pair<std::string_view, std::string_view> constexpr danya_split_impl(std::string_view str, std::string_view delim) noexcept
{
    std::string_view token;
    std::size_t pos_end = str.find_first_of(delim);
    if (pos_end == std::string::npos)
        return {str, token};
    else if (pos_end != 0) {
        token = str.substr(0, pos_end);
        str = str.substr(pos_end);
        return {token, str};
    } else {
        if (std::size_t new_start = str.find_first_not_of(delim); new_start != std::string_view::npos) {
            str = str.substr(new_start);
            return danya_split_impl(str, delim);
        } else
            return {token, str};
    }
}

std::vector<std::string_view> danya_split(std::string_view str, std::string_view delim) noexcept
{
  std::vector<std::string_view> res;
  res.reserve(__simd256_count_of_symboles(str));
  auto p = danya_split_impl(str, delim);
  while (p.first.length() != 0) {
      res.emplace_back(p.first);
      p = danya_split_impl(p.second, delim);
  }
  return res;
}

std::vector<std::string_view> eva_split(std::string_view str, char del) {
  std::vector<std::string_view> splited;
  splited.reserve(str.length() / 2);
  size_t fit = 0, sit = 0;
  while (fit!= std::string_view::npos && sit != std::string_view::npos) {
      sit = str.find(del, fit);
      splited.emplace_back(str.substr(fit, sit-fit));
      fit = sit+1;
  }
  splited.shrink_to_fit();
  return splited;
}

std::vector<std::string_view> danya_split_2(std::string_view str, char delim) noexcept
{
  std::vector<std::string_view> res;
  res.reserve(str.length() / 4);
  std::size_t pos = 0;
  while (pos != std::string_view::npos) {
      pos = str.find_first_not_of(delim);
      if (pos == std::string_view::npos)
          return res;
      str = str.substr(pos);
      pos = str.find(delim);
      res.emplace_back(str.substr(0, pos));
      str = str.substr(pos == std::string::npos ? 0 : pos);
  }
  res.shrink_to_fit();
  return res;
}

std::vector<std::string_view> daz_split(std::string_view text, char delimiter) noexcept
{
  std::vector<std::string_view> splitted;
  std::size_t word = 0;
  splitted.reserve(__simd256_count_of_symboles(text) + 1);

  for (std::size_t index = 0; index < text.length(); ++index)
  {
    word = index;
    while (text[word] != delimiter)
    {
      if (text[word] == '\0')
      {
        splitted.emplace_back(text.substr(index, word - index));
        return splitted;
      }
      word++;
    }
    if (index != word) splitted.emplace_back(text.substr(index, word - index));
    index = word;
  }

  return splitted;
}

std::vector<std::string_view> daz_split_2(std::string_view text, char delimiter) noexcept {
  std::vector<std::string_view> splitted;
  splitted.reserve(text.length() / 4);
  std::size_t pos = 0;
  std::size_t len = 0;

  for (std::size_t i = 0; i < text.length(); i++) {
    if (text[i] != delimiter) {
      if (pos == 0) {
        pos = i;
      }
      ++len;
    } else {
      if (pos == 0) continue;
      if (pos == 1) --pos;
      splitted.push_back(text.substr(pos, len));
      pos = 0;
      len = 0;
    }
  }
  splitted.push_back(text.substr(pos, len));
  splitted.shrink_to_fit();
  return splitted;
}

static bool* bitmap(__m256i* __cmpres)
{
  uint8_t tmp_map[32];
  bool*   bit_map = new bool[32];

  _mm256_store_epi32(&tmp_map, *__cmpres);

  memcpy(bit_map, tmp_map, 32);

  return bit_map;
}

std::vector<std::string_view> simd_split(std::string_view text, char delimiter)
{
  std::vector<std::string_view> splitted;
  splitted.reserve(text.length() / 2);
  bool* delim_bitmap = new bool[text.length()];

  const __m256i __cmpdelim = _mm256_set1_epi8(delimiter);
  for (std::size_t index = 0; index < text.length(); index += 32)
  {
    __m256i __chunk = _mm256_loadu_si256((__m256i*)(text.substr(index, 32).data()));
    __m256i __cmpres = _mm256_cmpeq_epi8(__chunk, __cmpdelim);
    memcpy(delim_bitmap + index, bitmap(&__cmpres), 32);
  }

  std::size_t pos = 0;
  std::size_t len = 0;

  for (std::size_t i = 0; i < text.length(); i++)
  {
    if (!delim_bitmap[i])
    {
      if (pos == 0)
      {
        pos = i;
      }
      ++len;
    }
    else
    {
      if (pos == 0) continue;
      if (pos == 1) --pos;
      splitted.push_back(text.substr(pos, len));
      pos = 0;
      len = 0;
    }
  }

  splitted.push_back(text.substr(pos, len));

  return splitted;
}













