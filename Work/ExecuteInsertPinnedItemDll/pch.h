// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include "./detours.h"
#include <windows.h>
#include <Objidl.h>
#include <string>
#include <cstdio>
#include <ShlObj.h>
#include <objidl.h>
#include <sdkddkver.h>
#include <vector>


void HookInsertPin();

#define WIN_VERSION_UNSUPPORTED    (-1)
#define WIN_VERSION_7              0
#define WIN_VERSION_8              1
#define WIN_VERSION_81             2
#define WIN_VERSION_811            3
#define WIN_VERSION_10_T1          4  // 1507
#define WIN_VERSION_10_T2          5  // 1511
#define WIN_VERSION_10_R1          6  // 1607
#define WIN_VERSION_10_R2          7  // 1703
#define WIN_VERSION_10_R3          8  // 1709
#define WIN_VERSION_10_R4          9  // 1803
#define WIN_VERSION_10_R5          10 // 1809
#define WIN_VERSION_10_19H1        11 // 1903, 1909
#define WIN_VERSION_10_20H1        12 // 2004, 20H2, 21H1, 21H2, 22H2
#define WIN_VERSION_SERVER_2022    13 // Server 2022
#define WIN_VERSION_11_21H2        14
#define WIN_VERSION_11_22H2        15
// helper macros
#define FIRST_NONEMPTY_ARG_2(a, b) \
                                   ( (sizeof(#a) > sizeof("")) ? (a+0) : (b) )
#define FIRST_NONEMPTY_ARG_3(a, b, c) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_2(b, c))
#define FIRST_NONEMPTY_ARG_4(a, b, c, d) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_3(b, c, d))
#define FIRST_NONEMPTY_ARG_5(a, b, c, d, e) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_4(b, c, d, e))
#define FIRST_NONEMPTY_ARG_6(a, b, c, d, e, f) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_5(b, c, d, e, f))
#define FIRST_NONEMPTY_ARG_7(a, b, c, d, e, f, g) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_6(b, c, d, e, f, g))
#define FIRST_NONEMPTY_ARG_8(a, b, c, d, e, f, g, h) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_7(b, c, d, e, f, g, h))
#define FIRST_NONEMPTY_ARG_9(a, b, c, d, e, f, g, h, i) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_8(b, c, d, e, f, g, h, i))
#define FIRST_NONEMPTY_ARG_10(a, b, c, d, e, f, g, h, i, j) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_9(b, c, d, e, f, g, h, i, j))
#define FIRST_NONEMPTY_ARG_11(a, b, c, d, e, f, g, h, i, j, k) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_10(b, c, d, e, f, g,  h, i, j, k))
#define FIRST_NONEMPTY_ARG_12(a, b, c, d, e, f, g, h, i, j, k, l) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_11(b, c, d, e, f, g, h, i, j, k, l))
#define FIRST_NONEMPTY_ARG_13(a, b, c, d, e, f, g, h, i, j, k, l, m) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_12(b, c, d, e, f, g, h, i, j, k, l, m))
#define FIRST_NONEMPTY_ARG_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_13(b, c, d, e, f, g, h, i, j, k, l, m, n))
#define FIRST_NONEMPTY_ARG_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_14(b, c, d, e, f, g, h, i, j, k, l, m, n, o))
#define FIRST_NONEMPTY_ARG_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_15(b, c, d, e, f, g, h, i, j, k, l, m, n, o, p))
#define FIRST_NONEMPTY_ARG_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) \
                                   FIRST_NONEMPTY_ARG_2(a, FIRST_NONEMPTY_ARG_16(b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q))

#define DO2(d7, dx)                ( (nWinVersion > WIN_VERSION_7) ? FIRST_NONEMPTY_ARG_2(dx, d7) : (d7) )
#define DO3(d7, d8, dx)            ( (nWinVersion > WIN_VERSION_8) ? FIRST_NONEMPTY_ARG_3(dx, d8, d7) : DO2(d7, d8) )
#define DO4(d7, d8, d81, dx)       ( (nWinVersion > WIN_VERSION_81) ? FIRST_NONEMPTY_ARG_4(dx, d81, d8, d7) : DO3(d7, d8, d81) )
#define DO5(d7, d8, d81, d811, dx) ( (nWinVersion > WIN_VERSION_811) ? FIRST_NONEMPTY_ARG_5(dx, d811, d81, d8, d7) : DO4(d7, d8, d81, d811) )
#define DO6(d7, d8, d81, d811, d10_t1, dx) \
                                   ( (nWinVersion > WIN_VERSION_10_T1) ? \
                                     FIRST_NONEMPTY_ARG_6(dx, d10_t1, d811, d81, d8, d7) : \
                                     DO5(d7, d8, d81, d811, d10_t1) )
#define DO7(d7, d8, d81, d811, d10_t1, d10_t2, dx) \
                                   ( (nWinVersion > WIN_VERSION_10_T2) ? \
                                     FIRST_NONEMPTY_ARG_7(dx, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO6(d7, d8, d81, d811, d10_t1, d10_t2) )
#define DO8(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, dx) \
                                   ( (nWinVersion > WIN_VERSION_10_R1) ? \
                                     FIRST_NONEMPTY_ARG_8(dx, d10_r1, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO7(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1) )
#define DO9(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, dx) \
                                   ( (nWinVersion > WIN_VERSION_10_R2) ? \
                                     FIRST_NONEMPTY_ARG_9(dx, d10_r2, d10_r1, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO8(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2) )
#define DO10(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, dx) \
                                   ( (nWinVersion > WIN_VERSION_10_R3) ? \
                                     FIRST_NONEMPTY_ARG_10(dx, d10_r3, d10_r2, d10_r1, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO9(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3) )
#define DO11(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, dx) \
                                   ( (nWinVersion > WIN_VERSION_10_R4) ? \
                                     FIRST_NONEMPTY_ARG_11(dx, d10_r4, d10_r3, d10_r2, d10_r1, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO10(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4) )
#define DO12(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, dx) \
                                   ( (nWinVersion > WIN_VERSION_10_R5) ? \
                                     FIRST_NONEMPTY_ARG_12(dx, d10_r5, d10_r4, d10_r3, d10_r2, d10_r1, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO11(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5) )
#define DO13(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1, dx) \
                                   ( (nWinVersion > WIN_VERSION_10_19H1) ? \
                                     FIRST_NONEMPTY_ARG_13(dx, d10_19h1, d10_r5, d10_r4, d10_r3, d10_r2, d10_r1, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO12(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1) )
#define DO14(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1, d10_20h1, dx) \
                                   ( (nWinVersion > WIN_VERSION_10_20H1) ? \
                                     FIRST_NONEMPTY_ARG_14(dx, d10_20h1, d10_19h1, d10_r5, d10_r4, d10_r3, d10_r2, d10_r1, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO13(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1, d10_20h1) )
#define DO15(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1, d10_20h1, ds_2022, dx) \
                                   ( (nWinVersion > WIN_VERSION_SERVER_2022) ? \
                                     FIRST_NONEMPTY_ARG_15(dx, ds_2022, d10_20h1, d10_19h1, d10_r5, d10_r4, d10_r3, d10_r2, d10_r1, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO14(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1, d10_20h1, ds_2022) )
#define DO16(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1, d10_20h1, ds_2022, d11_21h2, dx) \
                                   ( (nWinVersion > WIN_VERSION_11_21H2) ? \
                                     FIRST_NONEMPTY_ARG_16(dx, d11_21h2, ds_2022, d10_20h1, d10_19h1, d10_r5, d10_r4, d10_r3, d10_r2, d10_r1, d10_t2, d10_t1, d811, d81, d8, d7) : \
                                     DO15(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1, d10_20h1, ds_2022, d11_21h2) )
#define DO17(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1, d10_20h1, ds_2022, d11_21h2, d11_22h2, dx) \
                                   DO16(d7, d8, d81, d811, d10_t1, d10_t2, d10_r1, d10_r2, d10_r3, d10_r4, d10_r5, d10_19h1, d10_20h1, ds_2022, d11_21h2, d11_22h2)

#ifdef _WIN64
#define DEF3264(d32, d64)          (d64)
#else
#define DEF3264(d32, d64)          (d32)
#endif

#define DO2_3264(d7_32, d7_64, dx_32, dx_64) \
                                   DEF3264(DO2(d7_32, dx_32), \
                                           DO2(d7_64, dx_64))

#define DO3_3264(d7_32, d7_64, d8_32, d8_64, dx_32, dx_64) \
                                   DEF3264(DO3(d7_32, d8_32, dx_32), \
                                           DO3(d7_64, d8_64, dx_64))

#define DO4_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, dx_32, dx_64) \
                                   DEF3264(DO4(d7_32, d8_32, d81_32, dx_32), \
                                           DO4(d7_64, d8_64, d81_64, dx_64))

#define DO5_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, dx_32, dx_64) \
                                   DEF3264(DO5(d7_32, d8_32, d81_32, d811_32, dx_32), \
                                           DO5(d7_64, d8_64, d81_64, d811_64, dx_64))

#define DO6_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, dx_32, dx_64) \
                                   DEF3264(DO6(d7_32, d8_32, d81_32, d811_32, d10_t1_32, dx_32), \
                                           DO6(d7_64, d8_64, d81_64, d811_64, d10_t1_64, dx_64))

#define DO7_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, dx_32, dx_64) \
                                   DEF3264(DO7(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, dx_32), \
                                           DO7(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, dx_64))

#define DO8_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, dx_32, dx_64) \
                                   DEF3264(DO8(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, dx_32), \
                                           DO8(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, dx_64))

#define DO9_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, d10_r2_32, d10_r2_64, dx_32, dx_64) \
                                   DEF3264(DO9(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, d10_r2_32, dx_32), \
                                           DO9(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, d10_r2_64, dx_64))

#define DO10_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, d10_r2_32, d10_r2_64, d10_r3_32, d10_r3_64, dx_32, dx_64) \
                                   DEF3264(DO10(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, d10_r2_32, d10_r3_32, dx_32), \
                                           DO10(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, d10_r2_64, d10_r3_64, dx_64))

#define DO11_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, d10_r2_32, d10_r2_64, d10_r3_32, d10_r3_64, d10_r4_32, d10_r4_64, dx_32, dx_64) \
                                   DEF3264(DO11(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, d10_r2_32, d10_r3_32, d10_r4_32, dx_32), \
                                           DO11(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, d10_r2_64, d10_r3_64, d10_r4_64, dx_64))

#define DO12_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, d10_r2_32, d10_r2_64, d10_r3_32, d10_r3_64, d10_r4_32, d10_r4_64, d10_r5_32, d10_r5_64, dx_32, dx_64) \
                                   DEF3264(DO12(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, d10_r2_32, d10_r3_32, d10_r4_32, d10_r5_32, dx_32), \
                                           DO12(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, d10_r2_64, d10_r3_64, d10_r4_64, d10_r5_64, dx_64))

#define DO13_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, d10_r2_32, d10_r2_64, d10_r3_32, d10_r3_64, d10_r4_32, d10_r4_64, d10_r5_32, d10_r5_64, d10_19h1_32, d10_19h1_64, dx_32, dx_64) \
                                   DEF3264(DO13(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, d10_r2_32, d10_r3_32, d10_r4_32, d10_r5_32, d10_19h1_32, dx_32), \
                                           DO13(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, d10_r2_64, d10_r3_64, d10_r4_64, d10_r5_64, d10_19h1_64, dx_64))

#define DO14_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, d10_r2_32, d10_r2_64, d10_r3_32, d10_r3_64, d10_r4_32, d10_r4_64, d10_r5_32, d10_r5_64, d10_19h1_32, d10_19h1_64, d10_20h1_32, d10_20h1_64, dx_64) \
                                   DEF3264(DO13(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, d10_r2_32, d10_r3_32, d10_r4_32, d10_r5_32, d10_19h1_32, d10_20h1_32), \
                                           DO14(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, d10_r2_64, d10_r3_64, d10_r4_64, d10_r5_64, d10_19h1_64, d10_20h1_64, dx_64))

#define DO15_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, d10_r2_32, d10_r2_64, d10_r3_32, d10_r3_64, d10_r4_32, d10_r4_64, d10_r5_32, d10_r5_64, d10_19h1_32, d10_19h1_64, d10_20h1_32, d10_20h1_64, ds_2022_64, dx_64) \
                                   DEF3264(DO13(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, d10_r2_32, d10_r3_32, d10_r4_32, d10_r5_32, d10_19h1_32, d10_20h1_32), \
                                           DO15(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, d10_r2_64, d10_r3_64, d10_r4_64, d10_r5_64, d10_19h1_64, d10_20h1_64, ds_2022_64, dx_64))

#define DO16_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, d10_r2_32, d10_r2_64, d10_r3_32, d10_r3_64, d10_r4_32, d10_r4_64, d10_r5_32, d10_r5_64, d10_19h1_32, d10_19h1_64, d10_20h1_32, d10_20h1_64, ds_2022_64, d11_21h2_64, dx_64) \
                                   DEF3264(DO13(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, d10_r2_32, d10_r3_32, d10_r4_32, d10_r5_32, d10_19h1_32, d10_20h1_32), \
                                           DO16(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, d10_r2_64, d10_r3_64, d10_r4_64, d10_r5_64, d10_19h1_64, d10_20h1_64, ds_2022_64, d11_21h2_64, dx_64))

#define DO17_3264(d7_32, d7_64, d8_32, d8_64, d81_32, d81_64, d811_32, d811_64, d10_t1_32, d10_t1_64, d10_t2_32, d10_t2_64, d10_r1_32, d10_r1_64, d10_r2_32, d10_r2_64, d10_r3_32, d10_r3_64, d10_r4_32, d10_r4_64, d10_r5_32, d10_r5_64, d10_19h1_32, d10_19h1_64, d10_20h1_32, d10_20h1_64, ds_2022_64, d11_21h2_64, d11_22h2_64, dx_64) \
                                   DEF3264(DO13(d7_32, d8_32, d81_32, d811_32, d10_t1_32, d10_t2_32, d10_r1_32, d10_r2_32, d10_r3_32, d10_r4_32, d10_r5_32, d10_19h1_32, d10_20h1_32), \
                                           DO17(d7_64, d8_64, d81_64, d811_64, d10_t1_64, d10_t2_64, d10_r1_64, d10_r2_64, d10_r3_64, d10_r4_64, d10_r5_64, d10_19h1_64, d10_20h1_64, ds_2022_64, d11_21h2_64, d11_22h2_64, dx_64))

#endif //PCH_H
