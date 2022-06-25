#ifndef __CRC32_H
#define __CRC32_H

#include "./src/Lightning/namespace/lightning.hpp"
#include <cstdint>

/*!
 * @brief CRC-32
 */
class lightning::CRC32 {
public:
    static unsigned long crc32gen(const char*);

	static bool crc32cmp(const char*, const char*);

private:
	static const unsigned long crc32_lookup_table[256];
};

#endif // __CRC32_H
