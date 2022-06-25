#pragma once

#define __a 96
#define __z 123
#define __A 64
#define __Z 91
#define __DEBUG_MATH
//typedef unsigned short s16;
using s16 = unsigned short;
#define __START_TIME\
	const clock_t start = clock();
#define __FINISHED_AT\
	printf("\nfinished at %f secs\n",\
	static_cast<double>\
	(clock() - start) / CLOCKS_PER_SEC);
