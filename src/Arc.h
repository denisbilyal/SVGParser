#pragma once

#include "Figure.h"

class Arc : public Figure
{
public:
	Arc(int x, int y, double r, unsigned angleStart, unsigned angleLength, const std::string& stroke, const std::string& fill);

	bool inArea(int x1, int y1, int x2, int y2) const override;
	void print() const override;
	void translate(int vertical, int horizontal) override;
	void scale(double width, double height) override;
	void writeInFile(std::ofstream& of) const override;

	void setRadius(double radius);
	void setAngleStart(unsigned angle);
	void setAngleLength(unsigned angle);

private:
	double r;
	double angleStart; // in degrees
	double angleLength; // in degrees
};

class Circle : public Figure
{
public:
	Circle(int x, int y, double r, const std::string& stroke, const std::string& fill);
	bool inArea(int x1, int y1, int x2, int y2) const override;
	void print() const override;
	void translate(int vertical, int horizontal) override;
	void scale(double width, double height) override;
	void writeInFile(std::ofstream& of) const override;

	void setRadius(double radius);

private:
	double r;
};


class ArcUserCreator : public FigureUserCreator
{
public:
	ArcUserCreator();
	Figure* createFigure(std::istream& is) const override;
};

class ArcFileCreator : public FigureFileCreator
{
public:
	ArcFileCreator();
	Figure* createFigure(std::ifstream& ifs) const override;
};

class CircleFileCreator : public FigureFileCreator
{
public:
	CircleFileCreator();
	Figure* createFigure(std::ifstream& ifs) const override;
};

