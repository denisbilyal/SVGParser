#pragma once
#include "Figure.h"

class Polygon : public Figure
{
public:
	Polygon(const std::vector<Point>& points, const std::string& stroke, const std::string& fill);


	bool inArea(int x1, int y1, int x2, int y2) const override;
	void print() const override;
	void translate(int vertical, int horizontal) override;
	void scale(double width, double height) override;
	void writeInFile(std::ofstream& of) const override;

private:

	
};

class PolygonUserCreator : public FigureUserCreator
{
public:
	PolygonUserCreator();
	Figure* createFigure(std::istream& is) const override;
};

class PolygonFileCreator : public FigureFileCreator
{
public:
	PolygonFileCreator();
	Figure* createFigure(std::ifstream& ifs) const override;
};
