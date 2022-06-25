#ifndef __CRC16_H
#define __CRC16_H

#include "src/Lightning/namespace/lightning.hpp"

/*!
 * @brief CRC-16/MODBUS
 */
class lightning::CRC16 {
public:
	static unsigned short crc16gen(const char*);

	static bool crc16cmp(const char*, const char*);

private:
	static const unsigned short crc16_lookup_table[256];
};

#endif // __CRC16_H
