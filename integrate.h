#pragma once

#include <omp.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <functional>
#include <optional>
#include <cmath>

//#define INTEGRAL_DEBUG

#define DEFAULT_START_INTEGRATE_N 8    // Must be even
#define DEFAULT_EPS 1e-3
#define DEFAULT_MAX_INTEGRATE_ITERATIONS 15


bool inrange(double num, double low, double high);


namespace single
{

/**
 * @brief Numeric Simpson`s adaptive integration method.
 * @param func Function with one double parameter, returns double, that will be integrated;
 * @param a    Lower bound of integral;
 * @param b    Upper bound of integral;
 * @param n    The number of integration sub-sections (if `eps` specified, used as start number of subsections);
 * @param eps  Accuracy of integration. If not specified, only one integration step will be completed.
 * @return     Return std::optional<double>. Return value is integration result. Returning std::nullopt means that DEFAULT_MAX_INTEGRATE_ITERATIONS are reached.
 */
std::optional<double> integral(
    std::function<double(double)> func,
    double a,
    double b,
    int n = DEFAULT_START_INTEGRATE_N,
    std::optional<double> eps = std::nullopt
);

}