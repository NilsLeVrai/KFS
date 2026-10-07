#ifndef _PORTABLE_STDINT_H_
#define _PORTABLE_STDINT_H_

/* ========================================================================= */
/* 1. Types entiers à largeur exacte                                         */
/* ========================================================================= */

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
typedef long long          int64_t;
#else
__extension__ typedef long long int64_t; /* Support des compilateurs C89 anciens */
#endif

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
typedef unsigned long long uint64_t;
#else
__extension__ typedef unsigned long long uint64_t;
#endif

/* ========================================================================= */
/* 2. Types entiers de capacité minimale (Least)                             */
/* ========================================================================= */

typedef int8_t             int_least8_t;
typedef int16_t            int_least16_t;
typedef int32_t            int_least32_t;
typedef int64_t            int_least64_t;

typedef uint8_t            uint_least8_t;
typedef uint16_t           uint_least16_t;
typedef uint32_t           uint_least32_t;
typedef uint64_t           uint_least64_t;

/* ========================================================================= */
/* 3. Types entiers rapides (Fast)                                           */
/* ========================================================================= */

typedef int                int_fast8_t;
typedef int                int_fast16_t;
typedef int                int_fast32_t;
typedef int64_t            int_fast64_t;

typedef unsigned int       uint_fast8_t;
typedef unsigned int       uint_fast16_t;
typedef unsigned int       uint_fast32_t;
typedef uint64_t           uint_fast64_t;

/* ========================================================================= */
/* 4. Types pour pointeurs et capacité maximale                              */
/* ========================================================================= */

/* Détection automatique de la taille des pointeurs (32-bit vs 64-bit) */
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(__LP64__) || defined(_WIN64)
typedef int64_t            intptr_t;
typedef uint64_t           uintptr_t;
#else
typedef int32_t            intptr_t;
typedef uint32_t           uintptr_t;
#endif

typedef int64_t            intmax_t;
typedef uint64_t           uintmax_t;

/* ========================================================================= */
/* 5. Limites des valeurs (Macros MIN / MAX)                                 */
/* ========================================================================= */

#define INT8_MIN         (-128)
#define INT16_MIN        (-32768)
#define INT32_MIN        (-2147483647 - 1)
#define INT64_MIN        (-9223372036854775807LL - 1)

#define INT8_MAX         (127)
#define INT16_MAX        (32767)
#define INT32_MAX        (2147483647)
#define INT64_MAX        (9223372036854775807LL)

#define UINT8_MAX        (255)
#define UINT16_MAX       (65535)
#define UINT32_MAX       (4294967295U)
#define UINT64_MAX       (18446744073709551615ULL)

#define INTMAX_MIN       INT64_MIN
#define INTMAX_MAX       INT64_MAX
#define UINTMAX_MAX      UINT64_MAX

#endif /* _PORTABLE_STDINT_H_ */
