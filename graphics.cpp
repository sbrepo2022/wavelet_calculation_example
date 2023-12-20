#include "graphics.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////  GnuplotProcess2D  //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


PlotRect::PlotRect(double x_min, double y_min, double x_max, double y_max) :
    x_min(x_min),
    y_min(y_min),
    x_max(x_max),
    y_max(y_max)
{

}


PlotStyle2D::PlotStyle2D() :
    line_width(std::nullopt),
    line_color(std::nullopt),
    title(std::nullopt),
    key_title(std::nullopt),
    line_title(std::nullopt)
{

}


PlotStyle2D PlotStyle2D::defaultStyle()
{
    PlotStyle2D plot_style_2d;

    plot_style_2d.line_width = 3;
    plot_style_2d.line_color = "blue";

    return plot_style_2d;
}


void PlotStyle2D::merge(const PlotStyle2D &other_style)
{
    if (other_style.line_width) this->line_width = *(other_style.line_width);
    if (other_style.line_color) this->line_color = *(other_style.line_color);
    if (other_style.title) this->title = *(other_style.title);
    if (other_style.key_title) this->key_title = *(other_style.key_title);
    if (other_style.line_title) this->line_title = *(other_style.line_title);
}


bool GnuplotProcess2D::checkGnuplotOpened()
{
    if (!gnuplotPipe)
    {
        std::cerr << "Error: cannot create pipe to Gnuplot." << std::endl;
        return false;
    }
    return true;
}


GnuplotProcess2D::GnuplotProcess2D() :
    gnuplotPipe(nullptr),
    number_of_outputs(0)
{

}


GnuplotProcess2D::~GnuplotProcess2D()
{
    if (gnuplotPipe)
    {
        pclose(gnuplotPipe);
    }
}


void GnuplotProcess2D::open(const std::string& outputFilename, const PlotRect &plot_rect)
{
    gnuplotPipe = popen("gnuplot -persist", "w");
    if (! this->checkGnuplotOpened())
        return;

    // Setup Gnuplot
    fprintf(gnuplotPipe, "set term pngcairo rounded size 800,600\n");
    fprintf(gnuplotPipe, "set output '%s'\n", outputFilename.c_str());
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "set multiplot\n");
    fprintf(gnuplotPipe, "set xrange [%lf:%lf];\n", plot_rect.x_min, plot_rect.x_max);
    fprintf(gnuplotPipe, "set yrange [%lf:%lf];\n", plot_rect.y_min, plot_rect.y_max);
    this->setPlot2DStyle(PlotStyle2D::defaultStyle());
}


void GnuplotProcess2D::setPlot2DStyle(const PlotStyle2D &plot_style_2d)
{
    this->plot_style_2d.merge(plot_style_2d);

    if (! this->checkGnuplotOpened())
        return;


    if (plot_style_2d.line_color)
    {
        fprintf(
            gnuplotPipe,
            "set style line 1 linecolor rgb '%s'\n",
            plot_style_2d.line_color->c_str()
        );
    }

    if (plot_style_2d.line_width)
    {
        fprintf(
            gnuplotPipe,
            "set style line 1 linewidth %lf\n",
            *(plot_style_2d.line_width)
        );
    }

    if (plot_style_2d.title)
    {
        fprintf(
            gnuplotPipe,
            "set title '%s'\n",
            plot_style_2d.title->c_str()
        );
    }

    if (plot_style_2d.key_title)
    {
        fprintf(
            gnuplotPipe,
            "set key title '%s'\n",
            plot_style_2d.key_title->c_str()
        );
    }
}


void GnuplotProcess2D::plotGraph2D(const std::vector<double>& x, const std::vector<double>& y)
{
    std::string line_title = this->plot_style_2d.line_title ? *(this->plot_style_2d.line_title) : "-";

    if (!gnuplotPipe)
    {
        std::cerr << "Error: Gnuplot process is not start. Call open() before plotting." << std::endl;
        return;
    }

    if (x.size() != y.size())
    {
        std::cerr << "Error: x and y vectors shapes are not same." << std::endl;
        return;
    }

    fprintf(gnuplotPipe, "set key at screen 0.95, screen %lf right;\n", 0.87 - this->number_of_outputs * 0.05);
    fprintf(gnuplotPipe, "plot '-' with lines linestyle 1 title '%s'\n", line_title.c_str());
    for (size_t i = 0; i < x.size(); ++i)
    {
        fprintf(gnuplotPipe, "%lf %lf\n", x[i], y[i]);
    }
    fprintf(gnuplotPipe, "e\n");
    fflush(gnuplotPipe);

    this->number_of_outputs++;
}


void GnuplotProcess2D::close() {
    if (gnuplotPipe) {
        pclose(gnuplotPipe);
        gnuplotPipe = nullptr;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////  GnuplotProcess3D  //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

PlotStyle3D::PlotStyle3D() :
    title(std::nullopt),
    surf_title(std::nullopt)
{

}


PlotStyle3D PlotStyle3D::defaultStyle()
{
    PlotStyle3D plot_style_3d;

    plot_style_3d.x_label = "X axis";
    plot_style_3d.y_label = "Y axis";
    plot_style_3d.z_label = "Z axis";

    return plot_style_3d;
}


void PlotStyle3D::merge(const PlotStyle3D &other_style)
{
    if (other_style.title) this->title = *(other_style.title);
    if (other_style.surf_title) this->surf_title = *(other_style.surf_title);
    if (other_style.x_label) this->x_label = *(other_style.x_label);
    if (other_style.y_label) this->y_label = *(other_style.y_label);
    if (other_style.z_label) this->z_label = *(other_style.z_label);
}


bool GnuplotProcess3D::checkGnuplotOpened()
{
    if (!gnuplotPipe)
    {
        std::cerr << "Error: cannot create pipe to Gnuplot." << std::endl;
        return false;
    }
    return true;
}


GnuplotProcess3D::GnuplotProcess3D() :
    gnuplotPipe(nullptr)
{

}


GnuplotProcess3D::~GnuplotProcess3D()
{
    if (gnuplotPipe)
    {
        pclose(gnuplotPipe);
    }
}


void GnuplotProcess3D::open(const std::string& outputFilename)
{
    gnuplotPipe = popen("gnuplot -persist", "w");
    if (! this->checkGnuplotOpened())
        return;

    // Setup Gnuplot
    fprintf(gnuplotPipe, "set term pngcairo rounded size 800,600\n");
    fprintf(gnuplotPipe, "set output '%s'\n", outputFilename.c_str());
    fprintf(gnuplotPipe, "set palette rgbformulae 33,13,10;\n");
    fprintf(gnuplotPipe, "set grid;\n");
    this->setPlot3DStyle(PlotStyle3D::defaultStyle());
}


void GnuplotProcess3D::setPlot3DStyle(const PlotStyle3D &plot_style_3d)
{
    this->plot_style_3d.merge(plot_style_3d);

    if (! this->checkGnuplotOpened())
        return;

    if (plot_style_3d.title)
    {
        fprintf(
            gnuplotPipe,
            "set title '%s'\n",
            plot_style_3d.title->c_str()
        );
    }

    if (plot_style_3d.x_label)
    {
        fprintf(
            gnuplotPipe,
            "set xlabel '%s'\n",
            plot_style_3d.x_label->c_str()
        );
    }

    if (plot_style_3d.y_label)
    {
        fprintf(
            gnuplotPipe,
            "set ylabel '%s'\n",
            plot_style_3d.y_label->c_str()
        );
    }

    if (plot_style_3d.z_label)
    {
        fprintf(
            gnuplotPipe,
            "set zlabel '%s'\n",
            plot_style_3d.z_label->c_str()
        );
    }
}


void GnuplotProcess3D::plotGraph3D(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z)
{
    std::string surf_title = this->plot_style_3d.surf_title ? *(this->plot_style_3d.surf_title) : "-";

    if (!gnuplotPipe)
    {
        std::cerr << "Error: Gnuplot process is not start. Call open() before plotting." << std::endl;
        return;
    }

    if (x.size() != y.size() || x.size() * y.size() != z.size())
    {
        std::cerr << "Error: x, y vectors shapes are not same or z vector shape is not x.size() * y.size()." << std::endl;
        return;
    }

    fprintf(gnuplotPipe, "set dgrid3d %ld,%ld;\n", x.size(), y.size());
    fprintf(gnuplotPipe, "splot '-' using 1:2:3 with pm3d title '%s'\n", surf_title.c_str());
    for (size_t i = 0; i < x.size(); ++i)
    {
        for (size_t j = 0; j < y.size(); ++j)
        {
            fprintf(gnuplotPipe, "%lf %lf %lf\n", x[i], y[j], z[i * x.size() + j]);
        }
    }
    fprintf(gnuplotPipe, "e\n");
    fflush(gnuplotPipe);
}


void GnuplotProcess3D::close() {
    if (gnuplotPipe) {
        pclose(gnuplotPipe);
        gnuplotPipe = nullptr;
    }
}