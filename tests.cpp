#include "tests.h"
#include <cmath>

int main(int argc, char** argv)
{
    {
        auto func = [](double x) { return x; };

        auto result = single::integral(func, 0, 5, DEFAULT_START_INTEGRATE_N);

        if (! result)
        {
            MSG_ASSERT("single::integral | y = x", "integral returns std::nullopt");
        }
        else
        {
            TEST_FUNC_ASSERT(
                "single::integral | y = x",
                *result,
                [](double result)
                {
                    return inrange(result, 12.5 - 1e-3, 12.5 + 1e-3);
                }
            );
        }
    }


    {
        auto func = [](double x) { return sin(x); };

        auto result = single::integral(func, 0, M_PI, DEFAULT_START_INTEGRATE_N);

        if (! result)
        {
            MSG_ASSERT("single::integral | y = sin(x)", "integral returns std::nullopt");
        }
        else
        {
            TEST_FUNC_ASSERT(
                "single::integral | y = sin(x)",
                *result,
                [](double result)
                {
                    return inrange(result, 2 - 1e-3, 2 + 1e-3);
                }
            );
        }
    }


    {
        auto signal_func = [](double t) { return sin(2 * M_PI * t / 10) + sin(2 * M_PI * t / 50); };

        auto result = wavelet(signal_func, 15, 30, 256, DEFAULT_START_INTEGRATE_N, DEFAULT_EPS, single::integral);

        if (! result)
        {
            MSG_ASSERT("wavelet | y = sin(2 * π * t / 10) + sin(2 * π * t / 50)", "wavelet returns std::nullopt");
        }
        else
        {
            TEST_FUNC_ASSERT(
                "wavelet | y = sin(2 * π * t / 10) + sin(2 * π * t / 50)",
                *result,
                [](double result)
                {
                    return inrange(result, 3.430993092956012 - 1e-3, 3.430993092956012 + 1e-3);
                }
            );
        }
    }


    return 0;
}