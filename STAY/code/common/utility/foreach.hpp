#pragma once
/* // old version
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
*/
/* new version */
#define stay_FE_CALLITn01(a,b)  a b
#define stay_FE_CALLITn02(a,b)  a b
#define stay_FE_CALLITn03(a,b)  a b 
#define stay_FE_CALLITn04(a,b)  a b
#define stay_FE_CALLITn04(a,b)  a b
#define stay_FE_CALLITn05(a,b)  a b
#define stay_FE_CALLITn06(a,b)  a b
#define stay_FE_CALLITn07(a,b)  a b
#define stay_FE_CALLITn08(a,b)  a b
#define stay_FE_CALLITn09(a,b)  a b
#define stay_FE_CALLITn10(a,b)  a b
#define stay_FE_CALLITn11(a,b)  a b
#define stay_FE_CALLITn12(a,b)  a b
#define stay_FE_CALLITn13(a,b)  a b
#define stay_FE_CALLITn14(a,b)  a b
#define stay_FE_CALLITn15(a,b)  a b
#define stay_FE_CALLITn16(a,b)  a b
#define stay_FE_CALLITn17(a,b)  a b
#define stay_FE_CALLITn18(a,b)  a b
#define stay_FE_CALLITn19(a,b)  a b
#define stay_FE_CALLITn20(a,b)  a b
#define stay_FE_CALLITn21(a,b)  a b

/* the MSVC preprocessor expands __VA_ARGS__ as a single argument, so it needs
 * to be expanded indirectly through the CALLIT macros.
 * http://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
 * http://stackoverflow.com/questions/21869917/visual-studio-va-args-issue
 */
#define stay_FE_n00()
#define stay_FE_n01(what, a, ...)  what(a)
#define stay_FE_n02(what, a, ...)  what(a) stay_FE_CALLITn02(stay_FE_n01,(what, ##__VA_ARGS__))
#define stay_FE_n03(what, a, ...)  what(a) stay_FE_CALLITn03(stay_FE_n02,(what, ##__VA_ARGS__))
#define stay_FE_n04(what, a, ...)  what(a) stay_FE_CALLITn04(stay_FE_n03,(what, ##__VA_ARGS__))
#define stay_FE_n05(what, a, ...)  what(a) stay_FE_CALLITn05(stay_FE_n04,(what, ##__VA_ARGS__))
#define stay_FE_n06(what, a, ...)  what(a) stay_FE_CALLITn06(stay_FE_n05,(what, ##__VA_ARGS__))
#define stay_FE_n07(what, a, ...)  what(a) stay_FE_CALLITn07(stay_FE_n06,(what, ##__VA_ARGS__))
#define stay_FE_n08(what, a, ...)  what(a) stay_FE_CALLITn08(stay_FE_n07,(what, ##__VA_ARGS__))
#define stay_FE_n09(what, a, ...)  what(a) stay_FE_CALLITn09(stay_FE_n08,(what, ##__VA_ARGS__))
#define stay_FE_n10(what, a, ...)  what(a) stay_FE_CALLITn10(stay_FE_n09,(what, ##__VA_ARGS__))
#define stay_FE_n11(what, a, ...)  what(a) stay_FE_CALLITn11(stay_FE_n10,(what, ##__VA_ARGS__))
#define stay_FE_n12(what, a, ...)  what(a) stay_FE_CALLITn12(stay_FE_n11,(what, ##__VA_ARGS__))
#define stay_FE_n13(what, a, ...)  what(a) stay_FE_CALLITn13(stay_FE_n12,(what, ##__VA_ARGS__))
#define stay_FE_n14(what, a, ...)  what(a) stay_FE_CALLITn14(stay_FE_n13,(what, ##__VA_ARGS__))
#define stay_FE_n15(what, a, ...)  what(a) stay_FE_CALLITn15(stay_FE_n14,(what, ##__VA_ARGS__))
#define stay_FE_n16(what, a, ...)  what(a) stay_FE_CALLITn16(stay_FE_n15,(what, ##__VA_ARGS__))
#define stay_FE_n17(what, a, ...)  what(a) stay_FE_CALLITn17(stay_FE_n16,(what, ##__VA_ARGS__))
#define stay_FE_n18(what, a, ...)  what(a) stay_FE_CALLITn18(stay_FE_n17,(what, ##__VA_ARGS__))
#define stay_FE_n19(what, a, ...)  what(a) stay_FE_CALLITn19(stay_FE_n18,(what, ##__VA_ARGS__))
#define stay_FE_n20(what, a, ...)  what(a) stay_FE_CALLITn20(stay_FE_n19,(what, ##__VA_ARGS__))
#define stay_FE_n21(what, a, ...)  what(a) stay_FE_CALLITn21(stay_FE_n20,(what, ##__VA_ARGS__))
#define stay_FE_n22(...)           ERROR: FOR_EACH only supports up to 21 arguments

#define stay_FE_GET_MACRO(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,NAME,...) NAME
#define stay_FOR_EACH(what, ...) stay_FE_CALLITn01(stay_FE_GET_MACRO(_0, ##__VA_ARGS__,stay_FE_n22,stay_FE_n21,stay_FE_n20,stay_FE_n19,\
                            stay_FE_n18,stay_FE_n17,stay_FE_n16,stay_FE_n15,stay_FE_n14,stay_FE_n13,stay_FE_n12,stay_FE_n11,stay_FE_n10,stay_FE_n09,\
                            stay_FE_n08,stay_FE_n07,stay_FE_n06,stay_FE_n05,stay_FE_n04,stay_FE_n03,stay_FE_n02,stay_FE_n01,stay_FE_n00), (what, ##__VA_ARGS__))
