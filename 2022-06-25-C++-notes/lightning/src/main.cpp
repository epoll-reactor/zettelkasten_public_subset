#include "src/Lightning/Line/line.hpp"
#include "src/Lightning/Regex/regex.hpp"
#include "src/Lightning/Hash/crc8.hpp"
#include "src/Lightning/Hash/crc16.hpp"
#include "src/Lightning/Hash/crc32.hpp"
#include "src/Lightning/Hash/sha256.hpp"
#include "src/Lightning/Math/math.hpp"
#include "src/Lightning/Complex/complex.hpp"
#include "src/Lightning/Array/array.hpp"
#include <iostream>
#include <string>

//#define __DEBUG_LINE
//#define __DEBUG_REGEX
#define __DEBUG_HASH
//#define __DEBUG_MATH
//#define __DEBUG_LOG
//#define __DEBUG_ARRAY

using lightning::Line;
using lightning::Regex;
using lightning::SHA256;
using lightning::CRC8;
using lightning::CRC16;
using lightning::CRC32;
using lightning::Math;
using lightning::Complex;
using lightning::Array;
using std::cout;
using std::endl;

int main(void)
{
const clock_t start = clock();
#ifdef	__START_TIME
		__START_TIME
#endif
#ifdef	__DEBUG_LINE
	Line a = "std::ios::sync_with_stdio(false))))))123";
	Line b = a;
	std::cout << "first string: \t\t\t\t" << a << std::endl;
	std::cout << "length of first string: \t\t" << a.length() << std::endl;
	std::cout << "reverse first string: \t\t\t" << a.reverse() << std::endl;
//	std::cout << "cut \" \" from first string: \t\t" << a.cut(" ") << std::endl;		// [!!!] BUG
//	std::cout << "replace \":\" with \".\": \t\t\t" << a.replace(":", ".") << std::endl;
	std::cout << "convert first string to uppercase: \t" << a.to_upper() << std::endl;
	std::cout << "convert first string to lowercase: \t" << a.to_lower() << std::endl;
	std::cout << "concatenate first string with \"[0-9]+\":\t" << a + " [0-9]+" << std::endl;
	std::cout << "concatenate first string with itself: \t" << a + a << std::endl;
	std::cout << "clear first string: \t" << a.clear()	<< std::endl;
	std::cout << "first string == itself: \t\t" << (a == a) << std::endl;
	std::cout << "first string != itself: \t\t" << (a != a) << std::endl;
	std::cout << "first string == \"123\": \t\t\t" << (a == "123") << std::endl;
	std::cout << "first string != \"123\": \t\t\t" << (a != "123") << std::endl;
//	std::cout << "match text (regex): \t\t\t" << Regex::match_exists(".*false", a.to_char_array()) << std::endl;
#endif//__DEBUG_LINE
#ifdef	__DEBUG_REGEX
#define SIZE 8
    const char* patterns[SIZE][2] = {
        {
            "\\d+",
            "123"
        }, {
            "\\w+\\s\\w+\\s\\w+",
            "first second third"
        }, {
            "\\D+\\s\\D+\\s\\D+",
            "first second third"
        }, {
            "first",
            "first"
        }, {
            "\\W+\\s\\W+",
            "? !"
        }, {
            "\\d.*",
            "text 123 abc"
        }, {
            "\\d+$",
            "123"
        }, {
            "{\"\\w+\":\\s\"\\w+\".*}",
            "{\"abc\": \"123\"}"
        }
    };
    for (uint i = 0; i < SIZE; i++) {
        cout << "pattern: " << patterns[i][0] << endl;
        cout << "text:    " << patterns[i][1] << endl;
        cout << "founded: " << Regex::match_exists(patterns[i][0],
                                            (char*)patterns[i][1])
                            << endl << endl;
    }
#endif//__DEBUG_REGEX
#ifdef  __DEBUG_HASH
//#define ARR_SIZE 10
//#define TEXT 0
//#define HASH 1

//    const char* sha256_values[ARR_SIZE][2] = {
//        {
//            "C++",
//            "f1deb75fa21d3c47a6e7a45967b79fad2d053947066dc95322c5bcee1e9fe21c"
//        }, {
//            "sha256",
//            "5d5b09f6dcb2d53a5fffc60c4ac0d55fabdf556069d6631545f42aa6e3500f2e"
//        }, {
//            "test",
//            "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"
//        }, {
//            "This tool allows you to generate the SHA256 hash of any string.",
//            "d6ec3b0f68b9a5127b9b3fe4b130c5d446dfd82c819d068f76834d146270b1fc"
//        }, {
//            "PHP",
//            "0a6a15345ad0c7e36ed9dc3ec9c8ce843af4bc765d1019eb6d563e0836b962f3"
//        }, {
//            "Python",
//            "18885f27b5af9012df19e496460f9294d5ab76128824c6f993787004f6d9a7db"
//        }, {
//            "Lua",
//            "b897ce727082286d99b851bcc54df589cf3704f56d3cc76e8b389e7f336e5735"
//        }, {
//            "CoffeeScript",
//            "99500c7189ab3d7565bacd906cb0c46607b898096cfdef814b3ccce269c21a20"
//        }, {
//            "C#",
//            "040228846ead4a4195145fe089343cb0894d00a9380176a41a8f6c5ee70b4824"
//        }, {
//            "Perl",
//            "eca37636c0711d96f605f252e43a4f3d0811a2ce7495c004b43eea2a9f7216e8"
//        }
//    };

//    for (int i = 0; i < ARR_SIZE; i++) {
//        SHA256 sha(sha256_values[i][0]);
//        std::cout << "generated hash: " << sha.hash() << std::endl;
//        std::cout << "original hash:  " << sha256_values[i][1] << std::endl << std::endl;
//    }

//////////////////////
//CHECK CRC{8..32} OF STRING
//////////////////////
    const char* string =
    "123456789";
//    printf("CRC-8: \t\t%x\n",		CRC8::crc8gen(string));
//    printf("CRC-16: \t%x\n",		CRC16::crc16gen(string));
    printf("CRC-32: \t%lx\n",		CRC32::crc32gen(string));
//    printf("CRC-8 compare: \t%d\n",	CRC8::crc8cmp(string, "crc is a very simple algorithm"));
//    printf("CRC-16 compare: %d\n",	CRC16::crc16cmp(string, "crc is a very simple algorithm"));
//    printf("CRC-32 compare: %d\n",	CRC32::crc32cmp(string, "crc is a very simple algorithm"));
#endif//__DEBUG_HASH
#ifdef __DEBUG_ARRAY
//	Array<int, 100> array;
//	for (int i = 0; i < 100; i++) {
//		array[i] = i;
//		cout << array[i] << endl;
//	}
    Array<int, 10> array  = { 1, 2, 3, 4, 5 };
    Array<int, 10> array2 = { 5, 4, 3, 2, 1 };
    cout << array.size() << endl;
    cout << array2.size() << endl;
    cout << array.first() << endl;
    cout << array.last() << endl;
    cout << array.size() << endl;
    array.swap(array2);
    array.fill('a');
    array.reverse();
    cout << "array  < array2: " << (array < array2) << endl;
    cout << "array  > array2: " << (array > array2) << endl;
    cout << "array == array2: " << (array == array2) << endl;
    cout << "array != array2: " << (array != array2) << endl;
    array.fill(10);
    cout << "\n";
    cout << "{ ";
    for (uint i = 0; i < array.size() - 1; i++) {
        cout << array[i] << ", ";
    }
    cout << array[array.size() - 1];
    cout << " }\n";
    array.swap(array2);
    cout << "{ ";
    for (uint i = 0; i < array.size() - 1; i++) {
        cout << array[i] << ", ";
    }
    cout << array[array.size() - 1];
    cout << " }\n";
    cout << "array.first: " << array.first() << endl;
    cout << "array.last:  " << array.last()  << endl;
    cout << "array.size:  " << array.size() << endl;
    array.~Array();

#endif//__DEBUG_ARRAY
#ifdef __DEBUG_MATH

	cout <<  Math::max<int>(Math::fabs<unsigned int>(Math::cos<int>(Math::min<int>(999, 888))), \
							Math::max<int>(Math::tg<int>(Math::sin<int>(Math::squareRoot<int>
						   (Math::sin<int>(30) == Math::cos<int>(Math::max<int>(Math::roundUp<float>(59.1), \
							Math::squareRoot<int>(Math::power<int>(Math::power<int>(2, Math::sin(120)), \
							Math::cos<float>(Math::tg<float>(Math::min<int>(Math::roundUp(1.1), \
							Math::sin<float>(Math::fabs<int>(-100)))))))))))), \
							Math::roundUp<float>(Math::roundDown<float>(11.3))));



	cout <<"Math::squareRoot<float>(8192):       ";
	cout << Math::squareRoot<float>(8192) << endl;
	cout << Math::root<float>(81, 3) << endl;
	cout <<"Math::power(2, 10):         ";
	cout << Math::power(2, 2) << endl;
	cout << Math::root<double>(81, 2) << endl;
	cout <<"Math::max((Math::power<int>(2, 10)), Math::squareRoot<int>(81)): ";
	cout << Math::max((Math::power<int>(2, 10)), Math::squareRoot<int>(81)) << endl;
	cout <<"Math::min((Math::power<int>(2, 10)), Math::squareRoot<int>(81)): ";
	cout << Math::min((Math::power<int>(2, 10)), Math::squareRoot<int>(81)) << endl;
	cout <<"Math::roundUp<float>(2.3):           ";
	cout << Math::roundUp<float>(2.3) << endl;
	cout <<"Math::roundDown<float>(2.923):       ";
	cout << Math::roundDown<float>(2.923) << endl;
	cout <<"Math::fabs<float> (1234.5):          ";
	cout << Math::fabs<float> (1234.5) << endl;
	cout <<"Math::fabs<float>(-1234.5):          ";
	cout << Math::fabs<float>(-1234.5) << endl;
	cout <<"Math::sin<float>(30):                ";
	cout << Math::sin<float>(30) << endl;
	cout <<"Math::cos<float>(29.3):              ";
	cout << Math::cos<float>(29.3) << endl;
	cout <<"Math::tg<float>(3):                  ";
	cout << Math::tg<float>(3) << endl;
	cout <<"Math::ctg<float>(3):                 ";
	cout << Math::ctg<float>(3) << endl;
	cout << Math::log<float>(1.4) << endl;
	cout << Math::ln<float>(1.4) << endl;
	cout << (130 >> 1) << endl;
	Complex<float> complex(7, 9);
	Complex<float> complex2(4, 2);
	cout << "complex (7 + 9) + (4 + 2) =         ";
	cout << complex + complex2 << endl;
	cout << "complex (7 + 9) - (4 + 2) =         ";
	cout << complex - complex2 << endl;
	cout << "complex (7 + 9) / (4 + 2) =         ";
	cout << complex / complex2 << endl;
	cout << "complex (7 + 9) * (4 + 2) =         ";
	cout << complex * complex2 << endl;
	cout << "complex1 == complex2:               ";
	cout << (complex != complex2) << endl;
#endif//__DEBUG_MATH
#ifdef __DEBUG_LOG
	double logs[10][2] {
		{
			1, 0
		},
		{
			2, 0.301030f
		},
		{
			3, 0.477121f
		},
		{
			4, 0.602060f
		},
		{
			5, 0.698970f
		},
		{
			6, 0.778151f
		},
		{
			7, 0.845098f
		},
		{
			8, 0.903090f
		},
		{
			9, 0.954243f
		},
		{
			10, 1.0f
		}
	};
	for (int i = 0; i < 10; i++) {
		cout << "log(" << logs[i][0] << ") = "
			 <<  Math::log<float>(logs[i][0])
			 << ", original = " << logs[i][1] << endl;
	}
#endif//__DEBUG_LOG
#ifdef	__FINISHED_AT
		__FINISHED_AT
#endif
	printf("\nfinished at %f secs\n",\
	static_cast<double>\
	(clock() - start) / CLOCKS_PER_SEC);
	return EXIT_SUCCESS;
}
