#ifndef TYPES_H
#define TYPES_H

#define INLINE static inline

typedef long long int64;
typedef int int32;
typedef short int int16;
typedef char int8;

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short int uint16;
typedef unsigned char uint8;

typedef int32 bool;

typedef int32 (*COMPARATOR_F)(void *, void *);

#endif