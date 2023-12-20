#pragma once

#include <iostream>
#include <optional>
#include <functional>
#include <cmath>

#include "integrate.h"


class WaveletBasis
{
public:
    virtual double basis(double t, double a, double b) const = 0;
    virtual ~WaveletBasis() {}
};


class WaveletHat : public WaveletBasis
{
public:
    virtual double basis(double t, double a, double b) const;
};

/**
 * @brief Method for calculating wavelet for signal.
 * @param func          Function with one double parameter, returns double, that will be integrated;
 * @param a             Wavelet parameter (scale);
 * @param b             Wavelet parameter (offset);
 * @param N             Lower and upper bound of integral [-N:N]
 * @param n             The number of integration sub-sections (if `eps` specified, used as start number of subsections);
 * @param eps           Accuracy of integration. If not specified, only one integration step will be completed;
 * @param integral      Integral function, that will be use for integration;
 * @param wavelet_basis Object, that provides overrided wavelet basis function (default wavelet hat);
 * @return              Return std::optional<double>. Return value is integration result. Returning std::nullopt means that DEFAULT_MAX_INTEGRATE_ITERATIONS are reached.
 */
std::optional<double> wavelet(
    std::function<double(double)> signal_func,
    double a,
    double b,
    double N,
    int n = DEFAULT_START_INTEGRATE_N,
    std::optional<double> eps = DEFAULT_EPS,
    std::function<std::optional<double>(std::function<double(double)>, double, double, int, std::optional<double>)> integral = single::integral,
    const WaveletBasis &wavelet_basis = WaveletHat()
);