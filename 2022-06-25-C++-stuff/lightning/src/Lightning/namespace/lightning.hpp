/*!
 * @namespace lightning -
 * top of the classes and
 * methods(in future) hierarchy
 */

namespace lightning {

    class Line;

    class Regex;

    class Allocator;

    class CRC8;

    class CRC16;

    class CRC32;

    class SHA256;

    class Math;

    template<typename T>
    class Complex;

    template<class T, unsigned int Size>
    class Array;

    void  swap(Line&, Line&);
    int   strlen(const char*);
    void* memset(void*, int, int);
}
