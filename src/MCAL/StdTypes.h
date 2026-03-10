#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/* ============================
   Standard Boolean Definitions
   ============================ */
#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef OK
#define OK   1
#endif

#ifndef NOK
#define NOK   0
#endif

#ifndef NULL
#define NULL   0
#endif

#ifndef NULL_PTR
#define NULL_PTR ((void*)0)
#endif


/* ============================
   Standard Integer Types
   ============================ */

/* Unsigned types */
typedef unsigned char      u8;      // 8-bit unsigned
typedef unsigned short     u16;     // 16-bit unsigned
typedef unsigned long      u32;     // 32-bit unsigned
typedef unsigned long long u64;     // 64-bit unsigned

/* Signed types */
typedef signed char        s8;      // 8-bit signed
typedef signed short       s16;     // 16-bit signed
typedef signed long        s32;     // 32-bit signed
typedef signed long long   s64;     // 64-bit signed

/* Character type */
typedef char               c8;      // 8-bit char

#endif /* STD_TYPES_H_ */