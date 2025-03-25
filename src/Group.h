#pragma once
#include "Figure.h"
class Group : public Figure
{
public:
	Group(const std::vector<Figure*>& figures);
	~Group();
	bool inArea(int x1, int y1, int x2, int y2) const override;
	void print() const override;
	void translate(int vertical, int horizontal) override;
	void scale(double width, double height) override;
	void writeInFile(std::ofstream& of) const override;


	void addFigure(Figure* elem);
	void removeFigure(size_t elemId);
private:
	std::vector<Figure*> data;

};

class GroupUserCreator : public FigureUserCreator
{
public:
	//GroupUserCreator();
	//Figure* createFigure(std::istream& is) const override;
};


class GroupFileCreator : public FigureFileCreator
{
public:
	GroupFileCreator();
	Figure* createFigure(std::ifstream& ifs) const override;
};

