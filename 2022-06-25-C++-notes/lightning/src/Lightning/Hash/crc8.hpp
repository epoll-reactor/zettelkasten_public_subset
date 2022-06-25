#ifndef __CRC8_H
#define __CRC8_H

#include "src/Lightning/namespace/lightning.hpp"

/*!
 * @brief CRC-8
 */
class lightning::CRC8 {
public:
	static unsigned short crc8gen(const char*);

	static bool crc8cmp(const char*, const char*);

private:
	static const unsigned short crc8_lookup_table[256];
};

#endif // __CRC8_H
