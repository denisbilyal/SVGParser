#include "Line.h"
#include <iostream>

Line::Line(int x1, int y1, int x2, int y2, const std::string& stroke) : Figure(stroke, "none")
{
	points.push_back(Point(x1, y1));
	points.push_back(Point(x2, y2));
	std::cout << "Successfully created Line! ID: " << id << std::endl;

}

bool Line::inArea(int x1, int y1, int x2, int y2) const
{
	return x1 <= points[0].x && y1 <= points[0].y && 
			x2 >= points[1].x && y2 >= points[1].y;
}

void Line::print() const
{
	std::cout << id << ". line " << points[0].x << ' ' << points[0].y
						 << ' ' << points[1].x << ' ' << points[1].y << ' ' << stroke << std::endl;
}

void Line::translate(int vertical, int horizontal)
{
	points[0].x += vertical;
	points[0].y += horizontal;
	points[1].x += vertical;
	points[1].y += horizontal;
}

void Line::scale(double width, double height)
{
	if (width < 0 || height < 0)
	{
		throw std::logic_error("Invalid scale parameters!");
	}
	//scale by moving its coordinates
	Point midp((points[0].x + points[1].x) / 2, (points[0].y + points[1].y) / 2);
	
	points[0].x = width * (points[0].x -  midp.x) + midp.x;
	points[0].y = width * (points[0].y -  midp.y) + midp.y;

	points[1].x = width * (points[1].x -  midp.x) + midp.x;
	points[1].y = width * (points[1].y -  midp.y) + midp.y;
	
}

void Line::writeInFile(std::ofstream& of) const
{
	if (!of.is_open())
	{
		throw std::logic_error("File not opened for writing!");
	}
	of << "<line x1=\"" << points[0].x << "\" y1=\"" << points[0].y << "\" x2=\"" << points[1].x << "\" y2=\"" << points[1].y << "\" stroke =\"" << stroke << "\"/>" << std::endl;
}

LineUserCreator::LineUserCreator() : FigureUserCreator("line")
{

}

Figure* LineUserCreator::createFigure(std::istream& is) const
{
	//get value from user
	int x1, y1, x2, y2;
	std::string stroke;

	is >> x1 >> y1 >> x2 >> y2 >> stroke;
	return new Line(x1, y1, x2, y2, stroke);
}

static LineUserCreator _;

LineFileCreator::LineFileCreator() : FigureFileCreator("line")
{

}

Figure* LineFileCreator::createFigure(std::ifstream& ifs) const
{
	if (!ifs.is_open())
	{
		throw std::logic_error("File not opened for reading!");
	}
	char buffer[1024];
	ifs.getline(buffer, 1024, '>');

	//get values from file
	std::string values(buffer);
	try
	{
		int x1 = std::stoi(stringFuncs::getValue(values, 'x1'));
		int y1 = std::stoi(stringFuncs::getValue(values, 'y1'));
		int x2 = std::stoi(stringFuncs::getValue(values, 'x2'));
		int y2 = std::stoi(stringFuncs::getValue(values, 'y2'));
		std::string stroke = stringFuncs::getValue(values, "stroke");

		return new Line(x1, y1, x2, y2, stroke);
	}
	catch (const std::exception&)
	{
		throw std::logic_error("The svg element is missing arguments!");
	}
	
}

static LineFileCreator __;
