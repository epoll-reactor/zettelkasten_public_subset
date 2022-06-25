#include "src/Lightning/Line/line.hpp"
#include "src/Lightning/Hash/crc32.hpp"
#include "src/defines.hpp"
#include "src/Lightning/Allocator/allocator.hpp"

using namespace lightning;

Line::Line(const char* string) {
	#ifdef __ALLOCATOR_H
	this->str = Allocator::allocate <char>();
	#else
	this->str = new char(strlen(string));
	#endif
	this->str = (char*)string;
}

Line::Line(const Line& other) {
	this->str = other.str;
}

Line::Line(const Line&& other)
	: str(other.str) {
	delete[] other.str;
}

void Line::operator=(Line& other) {
	this->str = other.str;
}

void Line::operator=(const char* other) {
	this->str = (char*)other;
}

void Line::move(const Line& other) {
	this->str = other.str;
}

std::ostream& lightning::operator <<
	(std::ostream& out,
	const Line& string) noexcept {
		return out << string.str;
	}

std::istream& lightning::operator >>
	(std::istream& in , Line& string) noexcept {
	string = "";
		return in.getline(string.str, 256);
	}

char* Line::operator + (const Line& other) noexcept {
	#ifdef __ALLOCATOR_H
	char* str = Allocator::allocate<char>();
	#else
	char* str = new char(strlen(this->str) + strlen(other.str));
	#endif
	return strcat(strcpy(str, this->str), other.str);
}

char* Line::operator + (const char* other) noexcept {
	#ifdef __ALLOCATOR_H
	char* str = Allocator::allocate<char>();
	#else
	char* str = new char(strlen(this->str) + strlen(other));
	#endif
	return strcat(strcpy(str, this->str), other);
}

bool Line::operator == (const Line & other) noexcept {
	#ifdef __CRC32_H
	return CRC32::crc32cmp(this->str, other.str);
	#else
	return !strcmp(this->str, other.str);
	#endif
}

bool Line::operator == (const char* other) noexcept {
	#ifdef __CRC32_H
	return CRC32::crc32cmp(this->str, other);
	#else
	return !strcmp(this->str, other);
	#endif
}

bool Line::operator != (const Line & other) noexcept {
	#ifdef __CRC32_H
	return !CRC32::crc32cmp(this->str, other.str);
	#else
	return strcmp(this->str, other.str);
	#endif
}

bool Line::operator != (const char* other) noexcept {
	#ifdef __CRC32_H
	return !CRC32::crc32cmp(this->str, other);
	#else
	return strcmp(this->str, other);
	#endif
}

void Line::operator += (const Line & other) noexcept {
	strcat(this->str, other.str);
}

void Line::operator += (const char* other) noexcept {
	strcat(this->str, other);
}

char* Line::to_upper() noexcept {
	#ifdef __ALLOCATOR_H
	char* str = Allocator::allocate<char>();
	#else
	char* str = new char(strlen(this->str));
	#endif
	s16 i = 0;
	strcpy(str, this->str);
	do {
		if (str[i] > __a and str[i] < __z) {
			str[i] -= 0x20;
		}
	} while (str[i++] != '\0');
	return str;
}

char* Line::to_lower() noexcept {
	#ifdef __ALLOCATOR_H
	char* str = Allocator::allocate<char>();
	#else
	char* str = new char(strlen(this->str));
	#endif
	s16 i = 0;
	strcpy(str, this->str);
	do {
		if (str[i] > __A and str[i] < __Z) {
			str[i] += 0x20;
		}
	} while (str[i++] != '\0');
	return str;
}

char* Line::reverse() noexcept {
	s16 i = 0;
	s16 l = strlen(this->str);
	#ifdef __ALLOCATOR_H
	char* str = Allocator::allocate<char>();
	#else
	char* str = new char(strlen(this->str));
	#endif
	strcpy(str, this->str);
	while (i < l / 2) {
		str[i] ^= str[l - i - 1];
		str[l - i - 1] ^= str[i]; // reverse str[i] with str[l-i-1] using XOR
		str[i] ^= str[l - i - 1];
		++i;
	}
	return str;
}

s16 Line::length() noexcept {
	return strlen(this->str);
}

const char* Line::clear() noexcept {
	return "";
}

char* Line::to_char_array() noexcept {
	return this->str;
}

void lightning::swap(Line & what, Line & with) {
	Line tmp = with;
	with = what;
	what = tmp;
}

int lightning::strlen(const char* string) {
	int length = 0;
	while (*string++ != '\0') {
		++length;
	}
	return length;
}

void* lightning::memset(void* dest, int what, int len) {
	 unsigned char* ptr =
	(unsigned char*)dest;
	 while (len-- > 0) {
		 *ptr++ = what;
	 }
	 return dest;
}

Line::~Line() {
	if (not this->str)
		delete[] this->str;
}
