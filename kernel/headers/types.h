#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned char 	    byte;
typedef unsigned short 	    word;
typedef unsigned int 	    dword;
typedef unsigned char 	    uint8_t;
typedef signed char 	    int8_t;
typedef signed short int 	int16_t;
typedef unsigned short int 	uint16_t;
typedef signed int   	    int32_t;
typedef unsigned int 	    uint32_t;
typedef unsigned long long 	uint64_t;
typedef long long 	        int64_t;

typedef int8_t 	            int8;
typedef uint8_t 	        uint8;
typedef int16_t 	        int16;
typedef uint16_t 	        uint16;
typedef int32_t 	        int32;
typedef uint32_t 	        uint32;
typedef int64_t 	        int64;
typedef uint64_t 	        uint64;

typedef unsigned char 	    uchar_t;
typedef uint32_t 	        wchar_t;
typedef uint32_t 	        size_t;
typedef uint32_t 	        addr_t;
typedef int32_t 	        pid_t;
typedef unsigned int        bool;

#ifndef UINT8_MAX
#define UINT8_MAX ((uint8_t)(-1))
#endif

#ifndef UINT16_MAX
#define UINT16_MAX ((uint16_t)(-1))
#endif

#ifndef UINT64_MAX
#define UINT64_MAX ((uint64_t)(-1))
#endif

#ifndef VOID
#define VOID void
#endif //!VOID

#ifndef PVOID
#define PVOID VOID*
#endif //!PVOID

#ifndef NULL
#define NULL 0
#endif //! NULL


#ifndef true
#define true 1
#endif //!true

#ifndef false
#define false 0
#endif //!false

#endif //!_TYPES_H_