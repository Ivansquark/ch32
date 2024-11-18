#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include "write.h"

#include "stdlib.h"

#include <charconv>

void* operator new(size_t size);
void operator delete( void *p);
void* operator new[](size_t size);
void operator delete[]( void *p);
void operator delete(void*, unsigned int);
void operator delete[](void*, unsigned int);

//extern "C" void __cxa_pure_virtual() { while (1); }


//namespace std
//{
//	void __throw_length_error(const char *);
//	void __throw_bad_alloc();
//}


#endif //MY_MALLOC_H
