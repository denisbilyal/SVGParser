#include "Rectangle.h"
#include <iostream>

Rectangle::Rectangle(int x1, int y1, unsigned width, unsigned height, const std::string& stroke, const std::string& fill) : Figure(stroke, fill), width(width), height(height)
{
    points.push_back(Point(x1, y1));
    std::cout << "Successfully created Rectangle! ID: " << id << std::endl;
}

bool Rectangle::inArea(int x1, int y1, int x2, int y2) const
{
   
    return x1 <= points[0].x && y1 <= points[0].y
        && x2 >= points[0].x + width && y1 >= points[0].y + height;
}

void Rectangle::print() const
{
    std::cout << id << ". rectangle " << points[0].x << ' ' << points[0].y << ' '
              << width << ' ' << height << ' ' << stroke << ' ' << fill << '\n';
}

void Rectangle::translate(int vertical, int horizontal)
{
    points[0].x += vertical;
    points[0].y += horizontal;
}

void Rectangle::scale(double widthScalar, double heightScalar)
{
    if (width < 0 || height < 0)
    {
        throw std::logic_error("Invalid scale parameters!");
    }
    width *= widthScalar;
    height *= heightScalar;
}

void Rectangle::writeInFile(std::ofstream& of) const
{
    if (!of.is_open())
    {
        throw std::logic_error("File not opened for writing!");
    }
    //<rect x="50" y="20" width="150" height="150" stroke="red" fill="red"/>
    of << "<rect x=\"" << points[0].x << "\" y=\"" << points[0].y << "\" width=\"" << width << "\" height=\"" << height << "\" stroke=\"" << stroke << "\" fill=\"" << fill << "\"/>" << std::endl;
}

RectangleUserCreator::RectangleUserCreator() : FigureUserCreator("rectangle")
{
}

Figure* RectangleUserCreator::createFigure(std::istream& is) const
{
    //get values from user
    int x, y;
    unsigned width, height;
    std::string stroke, fill;

    is >> x >> y >> width >> height >> stroke >> fill;
    return new Rectangle(x, y, width, height, stroke, fill);
}
static RectangleUserCreator _;


RectangleFileCreator::RectangleFileCreator() : FigureFileCreator("rect")
{
}

Figure* RectangleFileCreator::createFigure(std::ifstream& ifs) const
{
    if (!ifs.is_open())
    {
        throw std::logic_error("File not opened for reading!");
    }
    char buffer[1024];
    ifs.getline(buffer, 1024, '>');

    std::string values(buffer);
    //get values from file
    try
    {
        int x = std::stoi(stringFuncs::getValue(values, 'x'));
        int y = std::stoi(stringFuncs::getValue(values, 'y'));
        unsigned width = std::stoul(stringFuncs::getValue(values, "width"));
        unsigned height = std::stoul(stringFuncs::getValue(values, "height"));
        std::string stroke = stringFuncs::getValue(values, "stroke");
        std::string fill = stringFuncs::getValue(values, "fill");
        return new Rectangle(x, y, width, height, stroke, fill);
    }
    catch (const std::exception&)
    {
        throw std::logic_error("The svg element is missing arguments!");
    }
}

static RectangleFileCreator __;
