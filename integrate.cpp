#include "integrate.h"


bool inrange(double num, double low, double high)
{
    return num > low && num < high;
}


namespace single
{

std::optional<double> integral(
    std::function<double(double)> func,
    double a,
    double b,
    int n,
    std::optional<double> eps
)
{
    double integ_res = 0.0, integ_res_prev = 0.0;

    bool first_try = true;
    int iterations_counter = 0;
    do {
        integ_res_prev = integ_res;

        double h = (b - a) / n;
        auto x = [a,h](int i) -> double { return a + (i - 1) * h; };

        // -- Simpson`s method --

        double sum_res = 0.0, sum1 = 0.0, sum2 = 0.0;

        for (int i = 1; i <= n / 2 - 1; i++)
        {
            sum1 += func(x(2 * i + 1));
        }

        for (int i = 1; i <= n / 2; i++)
        {
            sum2 += func(x(2 * i));
        }

        sum_res = func(x(1)) + 2 * sum1 + 4 * sum2 + func(x(n + 1));
        integ_res = h / 3 * sum_res;

        // -- end of Simpson`s method --

        #ifdef INTEGRAL_DEBUG
            std::cout << "single::integral() debug - "
                      << "iteration/steps: " << std::setw(10) << std::to_string(iterations_counter) + " / " + std::to_string(n)
                      << std::setw(5) << "|" << std::setw(15) << "integ_res: " << std::setw(10) << std::to_string(integ_res)
                      << std::setw(5) << "|" << std::setw(20) << "integ_res_prev: " << std::setw(10) << std::to_string(integ_res_prev)
                      << std::setw(5) << "|" << std::setw(9) << "acc: " << std::setw(10) << std::to_string(fabs(integ_res - integ_res_prev))
                      << std::setw(5) << "|" << std::setw(9) << "eps: " << std::setw(10) << std::to_string(*eps)
                      << std::endl;
        #endif


        if (!eps) break;  // If only one iteration needed

        if (!first_try)
        {
            if (fabs(integ_res - integ_res_prev) < *eps)  // If require accurasy reached
                break;
        }
        first_try = false;

        iterations_counter++;
        if (iterations_counter >= DEFAULT_MAX_INTEGRATE_ITERATIONS)
        {
            std::cerr << "integration error: DEFAULT_MAX_INTEGRATE_ITERATIONS reached" << std::endl;
            return std::nullopt;
        }

        n *= 2;
    } while(true);

    return integ_res;
}

}


namespace parallel
{

std::optional<double> integral(
    std::function<double(double)> func,
    double a,
    double b,
    int n,
    std::optional<double> eps
)
{
    return 0.0;
}

}