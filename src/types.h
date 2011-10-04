#pragma once
#include "config.h"

//! Guaranteed to be 32 bit float
typedef float float32_t;

//! Guaranteed to be 64 bit float
typedef double float64_t;

//! Guaranteed to be 8 bit signed int
typedef char int8_t;
	
//! Guaranteed to be 16 bit signed int
typedef short int16_t;
	
//! Guaranteed to be 32 bit signed int
typedef int int32_t;

//! Guaranteed to be 64 bit signed int
#if ARCH == ARCH_X86_64
typedef long int64_t;
#endif

//! Guaranteed to be 8 bit unsigned int
typedef unsigned char uint8_t;

//! Guaranteed to be 16 bit unsigned int
typedef unsigned short uint16_t;
	
//! Guaranteed to be 32 bit unsigned int
typedef unsigned int uint32_t;

//! Guaranteed to be 64 bit unsigned int
#if ARCH == ARCH_X86_64
typedef unsigned long uint64_t;
#endif

typedef unsigned int uint;
