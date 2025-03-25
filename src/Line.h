#pragma once
#include "Figure.h"

class Line : public Figure
{
public:
	Line(int x1, int y1, int x2, int y2, const std::string& stroke);
	bool inArea(int x1, int y1, int x2, int y2) const override;
	void print() const override;
	void translate(int vertical, int horizontal) override;
	void scale(double width, double height) override;
	void writeInFile(std::ofstream& of) const override;

private:

};

class LineUserCreator : public FigureUserCreator
{
public:
	LineUserCreator();
	Figure* createFigure(std::istream& is) const override;
};

class LineFileCreator : public FigureFileCreator
{
public:
	LineFileCreator();
	Figure* createFigure(std::ifstream& ifs) const override;
};