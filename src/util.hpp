#include <iostream>

typedef unsigned char byte;
typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

#define safe_free_s(ptr) if ((ptr) != nullptr) delete (ptr); \
	(ptr) = nullptr;

#define safe_free_m(ptr) if ((ptr) != nullptr) delete[] (ptr); \
	(ptr) = nullptr;