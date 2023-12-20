#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <format>


class PlotRect
{
public:
    double x_min, y_min, x_max, y_max;

    PlotRect();
    PlotRect(double x_min, double y_min, double x_max, double y_max);
};


class PlotStyle2D
{
public:
    PlotStyle2D();
    static PlotStyle2D defaultStyle();
    void merge(const PlotStyle2D &other_style);

    std::optional<double> line_width;
    std::optional<std::string> line_color;

    std::optional<std::string> title;
    std::optional<std::string> key_title;
    std::optional<std::string> line_title;
};


class GnuplotProcess2D {
private:
    FILE* gnuplotPipe;
    int number_of_outputs;

    PlotStyle2D plot_style_2d;

    bool checkGnuplotOpened();

public:
    GnuplotProcess2D();
    ~GnuplotProcess2D();
    void open(const std::string& outputFilename, const PlotRect &plot_rect);
    void setPlot2DStyle(const PlotStyle2D &plot_style_2d = PlotStyle2D());
    void plotGraph2D(const std::vector<double>& x, const std::vector<double>& y);
    void close();
};




class PlotStyle3D
{
public:
    PlotStyle3D();
    static PlotStyle3D defaultStyle();
    void merge(const PlotStyle3D &other_style);

    std::optional<std::string> title;
    std::optional<std::string> surf_title;
    std::optional<std::string> x_label;
    std::optional<std::string> y_label;
    std::optional<std::string> z_label;
};


class GnuplotProcess3D {
private:
    FILE* gnuplotPipe;

    PlotStyle3D plot_style_3d;

    bool checkGnuplotOpened();

public:
    GnuplotProcess3D();
    ~GnuplotProcess3D();
    void open(const std::string& outputFilename);
    void setPlot3DStyle(const PlotStyle3D &plot_style_3d = PlotStyle3D());
    void plotGraph3D(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z);
    void close();
};
