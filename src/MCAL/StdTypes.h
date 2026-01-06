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

#ifndef NULL_PTR
#define NULL_PTR ((void*)0)
#endif

/* ============================
   Standard Integer Types
   ============================ */

/* Unsigned types */
typedef unsigned char      unit_8;      // 8-bit unsigned
typedef unsigned short     unit_16;     // 16-bit unsigned
typedef unsigned long      unit_32;     // 32-bit unsigned
typedef unsigned long long unit_64;     // 64-bit unsigned

/* Signed types */
typedef signed char        sint_8;      // 8-bit signed
typedef signed short       sint_16;     // 16-bit signed
typedef signed long        sint_32;     // 32-bit signed
typedef signed long long   sint_64;     // 64-bit signed

/* Character type */
typedef char               char_8;      // 8-bit char

#endif /* STD_TYPES_H_ */