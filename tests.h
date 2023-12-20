// #define ABORT_IF_TEST_FAILED

#include "wavelet.h"
#include "integrate.h"

#include <cassert>

#define __CLEAR "\x1b[0m"
#define __RED "\x1b[31m"
#define __BLUE "\x1b[34m"
#define __GREEN "\x1b[32m"


static int __test_count = 1;


#ifdef ABORT_IF_TEST_FAILED
    #define ASSERT_ON_TEST_FAILED(__assert_expr) __assert_expr
#else
    #define ASSERT_ON_TEST_FAILED(__assert_expr)
#endif


#define MSG_ASSERT(__test_name, __msg) \
    do { \
        std::cerr << __RED << "► Test #" << __test_count << " '" << __test_name << "' failed\n" \
                    << "Reason: " << __BLUE << __msg << __CLEAR << std::endl; \
        ASSERT_ON_TEST_FAILED(assert(false)); \
    } while (false)


#define OPERATOR_ASSERT(__test_name, __result, __oper, __expected) \
    do { \
        if (!(__result __oper __expected)) \
        { \
            std::cerr << __RED << "► Test #" << __test_count << " '" << __test_name << "' failed\n" \
                      << "Expected: " << __BLUE << __expected << __RED \
                      << "; Operator: " << __BLUE << #__oper << __RED \
                      << "; Given: " << __BLUE << __result << __CLEAR << std::endl; \
            ASSERT_ON_TEST_FAILED(assert(__result __oper __expected)); \
        } \
        else \
        { \
            std::cerr << __GREEN << "• Test #" << __test_count << " '" << __test_name << "' done. " \
                      << __expected << " " << #__oper << " " << __result << __CLEAR << std::endl; \
        } \
        __test_count++; \
    } while (false)


#define TEST_FUNC_ASSERT(__test_name, __result, __test_func) \
    do { \
        if (!__test_func(__result)) \
        { \
            std::cerr << __RED << "► Test #" << __test_count << " '" << __test_name << "' failed\n" \
                      << "Test func: " << __BLUE << #__test_func << __RED \
                      << "\nGiven: " << __BLUE << __result << __CLEAR << std::endl; \
            ASSERT_ON_TEST_FAILED(assert(__test_func(__result))); \
        } \
        else \
        { \
            std::cerr << __GREEN << "• Test #" << __test_count << " '" << __test_name << "' done. Test result: " << __result << __CLEAR << std::endl; \
        } \
        __test_count++; \
    } while (false)
