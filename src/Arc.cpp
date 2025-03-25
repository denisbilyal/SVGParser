#define _USE_MATH_DEFINES

#include "Arc.h"
#include <cmath>
#include <iostream>

Arc::Arc(int x, int y, double r, unsigned angleStart, unsigned angleLength, const std::string& stroke, const std::string& fill) : Figure(stroke, fill)
{

    points.push_back(Point(x, y));
    
    setRadius(r);
    setAngleStart(angleStart);
    setAngleLength(angleLength);

    std::cout << "Successfully created Arc! ID: " << id << std::endl;
}

bool Arc::inArea(int x1, int y1, int x2, int y2) const
{
    //check for every angle which coordinate is the farthest
    double leftX = DBL_MAX;
    double upperY = DBL_MAX;
    double rightX = 0;
    double downY = 0;

    for (size_t i = 0; i <= angleLength; i++)
    {
        double currX = r * cos((-angleStart - i) * M_PI / 180) + points[0].x;
        double currY = r * sin((-angleStart - i) * M_PI / 180) + points[0].y;

        leftX = currX < leftX ? currX : leftX;
        upperY = currY < upperY ? currY : upperY;

        rightX = currX > rightX ? currX : rightX;
        downY = currY > downY ? currY : downY;
    }

    return x1 <= leftX && y1 <= upperY && x2 >= rightX && y2 >= downY;
}

void Arc::print() const
{
    std::cout << id << ". arc " << points[0].x << ' ' << points[0].y << ' ' << r << ' ' << angleStart << ' ' <<  angleLength << ' ' << stroke << ' ' << fill << std::endl;
}

void Arc::translate(int vertical, int horizontal)
{
    points[0].x += vertical;
    points[0].y += horizontal;
}

void Arc::scale(double width, double height)
{
    //we don't support ellipses
    if (width < 0)
    {
        throw std::invalid_argument("Invalid scale parameters!");
    }
    r *= width;
}

void Arc::writeInFile(std::ofstream& of) const
{
    if (!of.is_open())
    {
        throw std::logic_error("File not opened for writing!");
    }

    //calculating the start of the arc
    double startX = r * cos(-angleStart * M_PI / 180) + points[0].x;
    double startY = r * sin(-angleStart * M_PI / 180) + points[0].y;

    //calculating the end of the arc
    double endX = r * cos((-angleLength - angleStart) * M_PI / 180) + points[0].x;
    double endY = r * sin((-angleLength - angleStart) * M_PI / 180) + points[0].y;

    //determine from which half we want to connect the points
    bool fa = angleLength > 180;

    of << "<path d=\"M" << startX << " " << startY << " A" << r << " " << r << " " << 0 << " " << fa << " " << 0 << " " << endX << " " << endY <<
         "\" fill = \"" << fill << "\" stroke =\"" << stroke << "\"/>" << std::endl;

}

void Arc::setRadius(double radius)
{
    if (radius < 0)
    {
        throw std::invalid_argument("Invalid radius!");
    }
    r = radius;
}

void Arc::setAngleStart(unsigned angle)
{
    if (angle >= 360)
    {
        angle -= 360;
    }
    angleStart = angle;
}

void Arc::setAngleLength(unsigned angle)
{
   
    angleLength = angle;
}








Circle::Circle(int x, int y, double r, const std::string& stroke, const std::string& fill) : Figure(stroke, fill)
{
    points.push_back(Point(x, y));
    setRadius(r);
    std::cout << "Successfully created Arc! ID: " << id << std::endl;
}

bool Circle::inArea(int x1, int y1, int x2, int y2) const
{
    return x1 <= points[0].x - r && y1 <= points[0].y - r
        && x2 >= points[0].x + r && y2 >= points[0].y + r;
}

void Circle::print() const
{
    std::cout << id << ". arc " << points[0].x << ' ' << points[0].y << ' ' << r << ' ' << 0 << ' ' <<  360  << ' ' << stroke << ' ' << fill << std::endl;
}

void Circle::translate(int vertical, int horizontal)
{
    points[0].x += vertical;
    points[0].y += horizontal;
}

void Circle::scale(double width, double height)
{
    //we don't support ellipses
    if (width < 0)
    {
        throw std::invalid_argument("Invalid scale parameters!");
    }
    r *= width;
}

void Circle::writeInFile(std::ofstream& of) const
{
    of << "<circle cx=\"" << points[0].x << "\" cy=\"" << points[0].y << "\" r=\"" << r << "\" stroke=\"" << stroke << "\" fill=\"" << fill << "\"/>" << std::endl;
}

void Circle::setRadius(double radius)
{
    if (radius < 0)
    {
        throw std::invalid_argument("Invalid radius!");
    }
    r = radius;
}




ArcUserCreator::ArcUserCreator() : FigureUserCreator("arc")
{
}

Figure* ArcUserCreator::createFigure(std::istream& is) const
{
    int x, y;
    double r;
    unsigned startingAngle, lengthAngle;
    std::string stroke, fill;
    is >> x >> y >> r >> startingAngle >> lengthAngle >> stroke >> fill;

    if (lengthAngle >= 360)
        return new Circle(x, y, r, stroke, fill);
    
    return new Arc(x, y, r, startingAngle, lengthAngle, stroke, fill);;
}

static ArcUserCreator _;


ArcFileCreator::ArcFileCreator() : FigureFileCreator("path")
{

}

Figure* ArcFileCreator::createFigure(std::ifstream& ifs) const
{
    
    if (!ifs.is_open())
    {
        throw std::logic_error("File not opened for reading!");
    }

    char buffer[1024];
    ifs.getline(buffer, 1024, '>');
    std::string values(buffer);
    

    try
    {
        std::string measurements = stringFuncs::getValue(values, "d");
        std::string stroke = stringFuncs::getValue(values, "stroke");
        std::string fill = stringFuncs::getValue(values, "fill");

        std::vector<std::string> split;
        stringFuncs::splitValues(split, measurements, ' ');
        //get the values

        double sx = std::stod(split[0].c_str() + 1);
        double sy = std::stod(split[1]);
        //					  split[2] is radius
        double r = std::stod(split[3]);
        //		 we dont need split[4]
        bool fa = std::stoi(split[5]);
        bool fs = std::stoi(split[6]);
        double ex = std::stod(split[7]);
        double ey = std::stod(split[8]);


        // distance between the 2 points and diameter
        double distanceSqr = (sx - ex) * (sx - ex) + (sy - ey) * (sy - ey);
        double diameterSqr = 4 * r * r;

        //get middle point
        double mx = (sx + ex) / 2;
        double my = (sy + ey) / 2;

        double cx;
        double cy;

        //if they are not the same, then calculate...
        if ((float)distanceSqr != (float)diameterSqr)
        {
            double d = sqrt(r * r - distanceSqr / 4);
            double dist = sqrt(distanceSqr);

            double ox = d * (sx - ex) / dist;
            double oy = d * (sy - ey) / dist;

            if (!fa)
            {
                cx = mx - oy;
                cy = my + ox;
            }
            else
            {
                cx = mx + oy;
                cy = my - ox;
            }

        }
        else
        {
            //if distance and diameter are equal their middle point is center
            cx = mx;
            cy = my;
        }

        //calculate the angles
        double startingAngle = atan2(sy - cy, sx - cx) * (180 / M_PI);
        startingAngle = startingAngle < 0 ? -startingAngle : 360 - startingAngle;

        double endingAngle = atan2(ey - cx, ex - cy) * (180 / M_PI);
        endingAngle = endingAngle < 0 ? -endingAngle : 360 - endingAngle;

        double lengthAngle = endingAngle - startingAngle;

        if (startingAngle > endingAngle)
            lengthAngle += 360;


        return new Arc(round(cx), round(cy), r, round(startingAngle), round(lengthAngle), stroke, fill);
    }
    catch (const std::exception&)
    {
        throw std::logic_error("The svg element is missing arguments!");
    }
    
}

static ArcFileCreator __;



CircleFileCreator::CircleFileCreator() : FigureFileCreator("circle")
{

}

Figure* CircleFileCreator::createFigure(std::ifstream& ifs) const
{
    if (!ifs.is_open())
    {
        throw std::logic_error("File not opened for reading!");
    }

    char buffer[1024];
    ifs.getline(buffer, 1024, '>');

    std::string values(buffer);
    
    try
    {
        //get values
        int x = stoi(stringFuncs::getValue(values, "cx"));
        int y = stoi(stringFuncs::getValue(values, "cy"));
        double r = stod(stringFuncs::getValue(values, "r"));
        std::string stroke = stringFuncs::getValue(values, "stroke");
        std::string fill = stringFuncs::getValue(values, "fill");
        

        return new Circle(x, y, r, stroke, fill);
    }
    catch (const std::exception&)
    {
        throw std::logic_error("The svg element is missing arguments!");
    }
}
static CircleFileCreator ___;
