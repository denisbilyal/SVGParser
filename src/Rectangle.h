#pragma once
#include "Figure.h"

class Rectangle : public Figure
{
public:
	Rectangle(int x1, int y1, unsigned width, unsigned height, const std::string& stroke, const std::string& fill);
	bool inArea(int x1, int y1, int x2, int y2) const override;
	void print() const override;
	void translate(int vertical, int horizontal) override;
	void scale(double width, double height) override;
	void writeInFile(std::ofstream& of) const override;

private:
	unsigned width;
	unsigned height;

};

class RectangleUserCreator : public FigureUserCreator
{
public:
	RectangleUserCreator();
	Figure* createFigure(std::istream& is) const override;
};

class RectangleFileCreator : public FigureFileCreator
{
public:
	RectangleFileCreator();
	Figure* createFigure(std::ifstream& ifs) const override;
};