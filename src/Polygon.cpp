#include "Polygon.h"
#include <iostream>

Polygon::Polygon(const std::vector<Point>& points, const std::string& stroke, const std::string& fill) : Figure(stroke, fill)
{
	//add points
	if (points.empty())
	{
		throw std::invalid_argument("Invalid polygon points!");
	}
    this->points = points;
	std::cout << "Successfully created Polygon! ID: " << id << std::endl;
}

bool Polygon::inArea(int x1, int y1, int x2, int y2) const
{
	// get the farthest points
	double leftX = DBL_MAX;
	double upperY = DBL_MAX;
	double rightX = 0;
	double downY = 0;

	for (const Point& p : points)
	{
		leftX = p.x < leftX ? p.x : leftX;
		upperY = p.y < upperY ? p.y : upperY;

		rightX = p.x > rightX ? p.x : rightX;
		downY = p.y > downY ? p.y : downY;
	}
	return x1 <= leftX && y1 <= upperY && x2 >= rightX && y2 >= downY;
}

void Polygon::print() const
{
	std::cout << id << ". polygon ";
	for (const Point& p : points)
	{
		std::cout << p.x << ',' << p.y << ' ';
	}

	std::cout << stroke << ' ' << fill << std::endl;
}

void Polygon::translate(int vertical, int horizontal)
{
	for (Point& p : points)
	{
		p.x += vertical;
		p.y += horizontal;
	}
}

void Polygon::scale(double width, double height)
{

	if (width < 0 || height < 0)
	{
		throw std::logic_error("Invalid scale parameters!");
	}
	//scale by moving its points
	Point midp(0, 0);

	for (const Point& p : points)
	{
		midp.x += p.x;
		midp.y += p.y;
	}

	midp.x /= points.size();
	midp.y /= points.size();

	for (Point& p : points)
	{
		p.x = width * (p.x - midp.x) + midp.x;
		p.y = width * (p.y - midp.y) + midp.y;
	}

}

void Polygon::writeInFile(std::ofstream& of) const
{
	if (!of.is_open())
	{
		throw std::logic_error("File not opened for writing!");
	}
	//<rect x="50" y="20" width="150" height="150"/>
	of << "<polygon points=\"";
	for (const Point& p : points)
	{
		of << p.x << ',' << p.y << ' ';
	}
	of << "\" stroke =\"" << stroke << "\" fill=\"" << fill << "\"/>" << std::endl;
}

PolygonUserCreator::PolygonUserCreator() : FigureUserCreator("polygon")
{
}

Figure* PolygonUserCreator::createFigure(std::istream& is) const
{
	// get values from user
	unsigned pointsCount;
	int x, y;
	std::string stroke, fill;
	std::vector<Point> points;

	is >> pointsCount;

	for (size_t i = 0; i < pointsCount; i++)
	{
		is >> x >> y;
		points.push_back(Point(x, y));
	}
	is >> stroke >> fill;


	return new Polygon(points, stroke, fill);
}

static PolygonUserCreator _;


PolygonFileCreator::PolygonFileCreator() : FigureFileCreator("polygon")
{
}

Figure* PolygonFileCreator::createFigure(std::ifstream& ifs) const
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
		std::string measurements = stringFuncs::getValue(values, "points");

		std::vector<std::string> split;
		stringFuncs::splitValues(split, measurements, ' ');

		std::vector<Point> points;
		for (const std::string& point : split)
		{
			size_t commaInd = point.find(',');
			points.push_back(Point(stoi(point.substr(0, commaInd)), stoi(point.substr(commaInd + 1))));
		}

		std::string stroke = stringFuncs::getValue(values, "stroke");
		std::string fill = stringFuncs::getValue(values, "fill");

		return new Polygon(points, stroke, fill);
	}
	catch (const std::exception&)
	{
		throw std::logic_error("The svg element is missing arguments!");
	}
	
}

static PolygonFileCreator __;