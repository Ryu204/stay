#pragma once

// This file makes a foreach macro with max 10 arguments
#define stay_FE0(WHAT)
#define stay_FE1(WHAT, x1) WHAT(x1)
#define stay_FE2(WHAT, x1, ...) WHAT(x1) stay_FE1(WHAT, __VA_ARGS__)
#define stay_FE3(WHAT, x1, ...) WHAT(x1) stay_FE2(WHAT, __VA_ARGS__)
#define stay_FE4(WHAT, x1, ...) WHAT(x1) stay_FE3(WHAT, __VA_ARGS__)
#define stay_FE5(WHAT, x1, ...) WHAT(x1) stay_FE4(WHAT, __VA_ARGS__)
#define stay_FE6(WHAT, x1, ...) WHAT(x1) stay_FE5(WHAT, __VA_ARGS__)
#define stay_FE7(WHAT, x1, ...) WHAT(x1) stay_FE6(WHAT, __VA_ARGS__)
#define stay_FE8(WHAT, x1, ...) WHAT(x1) stay_FE7(WHAT, __VA_ARGS__)
#define stay_FE9(WHAT, x1, ...) WHAT(x1) stay_FE8(WHAT, __VA_ARGS__)
#define stay_FE10(WHAT, x1, ...) WHAT(x1) stay_FE9(WHAT, __VA_ARGS__)

#define stay_GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define FOR_EACH(action, ...) stay_GET_MACRO(\
    _0, __VA_ARGS__,\
    stay_FE10, stay_FE9, stay_FE8, stay_FE7,\
    stay_FE6, stay_FE5, stay_FE4, stay_FE3,\
    stay_FE2, stay_FE1, stay_FE0)(action, __VA_ARGS__)
