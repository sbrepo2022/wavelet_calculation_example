#include "wavelet.h"


double WaveletHat::basis(double t, double a, double b) const
{
    return (1 / sqrt(a)) * exp(-0.5 * pow((t - b) / a, 2)) * (pow((t - b) / a, 2) - 1.0);
}


std::optional<double> wavelet(
    std::function<double(double)> signal_func,
    double a,
    double b,
    double N,
    int n,
    std::optional<double> eps,
    std::function<std::optional<double>(std::function<double(double)>, double, double, int, std::optional<double>)> integral,
    const WaveletBasis &wavelet_basis
)
{
    auto func = [&signal_func, a, b, &wavelet_basis](double t) { return wavelet_basis.basis(t, a, b) * signal_func(t); };
    return integral(func, -N, N, n, eps);
}