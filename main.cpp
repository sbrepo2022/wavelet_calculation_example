#include <functional>
#include <iostream>
#include <format>

#include "wavelet.h"
#include "graphics.h"

#define MEASURE_TIME
#define USE_MULTITHREAD


int main()
{
    // Set signal function, which will be process with wavelet decomposition
    auto signal_func = [](double t) { return sin(2 * M_PI * t / 10) + sin(2 * M_PI * t / 50); };

    // Get values of signal function
    constexpr double signal_duration = 250;
    constexpr double signal_values_step = 0.25;
    std::vector<double> x_sig, y_sig;
    x_sig.reserve(signal_duration / signal_values_step);
    y_sig.reserve(signal_duration / signal_values_step);
    for (double t = 0; t < signal_duration; t += signal_values_step)
    {
        x_sig.push_back(t);
        y_sig.push_back(signal_func(t));
    }

    // Plot signal function
    GnuplotProcess2D gp_signal_func;
    PlotStyle2D plot_style_2d_signal_func;
    PlotRect plot_rect_signal_func(0, -2.5, signal_duration, 2.5);

    gp_signal_func.open("out_data/harmonic_signal.png", plot_rect_signal_func);

    plot_style_2d_signal_func.title = "The sum of two harmonic oscillations";
    plot_style_2d_signal_func.line_title = "y = sin(2 * π * t / 10) + sin(2 * π * t / 50)";
    gp_signal_func.setPlot2DStyle(plot_style_2d_signal_func);
    gp_signal_func.plotGraph2D(x_sig, y_sig);

    gp_signal_func.close();


    // Wavelet decomposition
    constexpr double x_min = 1.0, x_max = 50.0, y_min = 1.0, y_max = 50.0;
    constexpr double x_step_size = 1.0, y_step_size = 1.0;
    int x_steps = (x_max - x_min) / x_step_size;
    int y_steps = (y_max - y_min) / y_step_size;
    std::vector<double> x_surf, y_surf, z_surf;
    x_surf.reserve(x_steps);
    y_surf.reserve(y_steps);
    z_surf.resize(x_steps * y_steps);

    double _x = x_min;
    for (int x_step = 0; x_step < x_steps; x_step++)
    {
        x_surf.push_back(_x);
        _x += x_step_size;
    }

    double _y = y_min;
    for (int y_step = 0; y_step < y_steps; y_step++)
    {
        y_surf.push_back(_y);
        _y += y_step_size;
    }

    // Calculation of wavelet
    double wavelet_start_time = omp_get_wtime();
#ifdef USE_MULTITHREAD
    #pragma omp parallel for
#endif
    for (int x_step = 0; x_step < x_steps; x_step++)
    {
        for (int y_step = 0; y_step < y_steps; y_step++)
        {
            auto w = wavelet(signal_func, x_min + x_step * x_step_size, y_min + y_step * y_step_size, 256, 256, DEFAULT_EPS);
            if (!w)
            {
                std::cerr << "wavelet decomposition error: can not integrate signal with DEFAULT_MAX_INTEGRATE_ITERATIONS. "
                          << "Recompile with higher DEFAULT_MAX_INTEGRATE_ITERATIONS defined value.\n";
                exit(1);
            }
            #ifdef USE_MULTITHREAD
                #pragma omp critical
                {
                    z_surf[x_steps * x_step + y_step] = *w;
                }
            #else
                z_surf[x_steps * x_step + y_step] = *w;
            #endif
        }
    }
    double wavelet_end_time = omp_get_wtime();
    #ifdef MEASURE_TIME
    std::cout << "wavelet decomposition time for " << x_steps << "x" << y_steps << " field: " << wavelet_end_time - wavelet_start_time << " sec." << std::endl;
    #endif


    // Plot 3D surface of wavelet function
    GnuplotProcess3D gp_wavelet_surface;
    PlotStyle3D plot_style_3d_wavelet_surface;

    gp_wavelet_surface.open("out_data/wavelet_surface.png");
    plot_style_3d_wavelet_surface.title = "Wavelet decomposition surface";
    plot_style_3d_wavelet_surface.surf_title = "sin(2πt/10) + sin(2πt/50)";
    plot_style_3d_wavelet_surface.x_label = "a: scale";
    plot_style_3d_wavelet_surface.y_label = "b: delay";
    plot_style_3d_wavelet_surface.z_label = "";
    gp_wavelet_surface.setPlot3DStyle(plot_style_3d_wavelet_surface);
    gp_wavelet_surface.plotGraph3D(x_surf, y_surf, z_surf);
    gp_wavelet_surface.close();


    // Plot slice
    std::vector<double> w_2_b, w_15_b, w_30_b;
    w_2_b.reserve(y_steps);
    w_15_b.reserve(y_steps);
    w_30_b.reserve(y_steps);
    int x_step;

    x_step = 2 * x_step_size;
    for (int y_step = 0; y_step < y_steps; y_step++)
    {
        w_2_b.push_back(z_surf[x_step * x_steps + y_step]);
    }

    x_step = 15 * x_step_size;
    for (int y_step = 0; y_step < y_steps; y_step++)
    {
        w_15_b.push_back(z_surf[x_step * x_steps + y_step]);
    }

    x_step = 30 * x_step_size;
    for (int y_step = 0; y_step < y_steps; y_step++)
    {
        w_30_b.push_back(z_surf[x_step * x_steps + y_step]);
    }

    // Plot wavelet slice
    GnuplotProcess2D gp_wavelet_slice;
    PlotStyle2D plot_style_2d_wavelet_slice;
    PlotRect plot_rect_wavelet_slice(0, -6, 50, 6);

    gp_wavelet_slice.open("out_data/wavelet_a_slice.png", plot_rect_wavelet_slice);
    plot_style_2d_wavelet_slice.title = "Wavelet \"a\" slice";

    // Plot w(2, b)
    plot_style_2d_wavelet_slice.line_title = "w(2, b)";
    plot_style_2d_wavelet_slice.line_color = "blue";
    gp_wavelet_slice.setPlot2DStyle(plot_style_2d_wavelet_slice);
    gp_wavelet_slice.plotGraph2D(y_surf, w_2_b);

    // Plot w(15, b)
    plot_style_2d_wavelet_slice.line_title = "w(15, b)";
    plot_style_2d_wavelet_slice.line_color = "orange";
    gp_wavelet_slice.setPlot2DStyle(plot_style_2d_wavelet_slice);
    gp_wavelet_slice.plotGraph2D(y_surf, w_15_b);

    // Plot w(30, b)
    plot_style_2d_wavelet_slice.line_title = "w(30, b)";
    plot_style_2d_wavelet_slice.line_color = "green";
    gp_wavelet_slice.setPlot2DStyle(plot_style_2d_wavelet_slice);
    gp_wavelet_slice.plotGraph2D(y_surf, w_30_b);

    gp_wavelet_slice.close();

    return 0;
}