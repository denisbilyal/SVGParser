#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include "Point.h"

//functions for strings
namespace stringFuncs 
{
	std::string getValue(const std::string& text, const std::string& word);

	std::string getValue(const std::string& text, char sym);

	void splitValues(std::vector<std::string>& res, const std::string& text, char delim);
	
}

//Base Class
class Figure
{
public:
	Figure(const std::string& stroke, const std::string& fill);
	Figure(const Figure& other);
	virtual ~Figure() = default;


	virtual bool inArea(int x1, int y1, int x2, int y2) const = 0;
	virtual void print() const = 0;
	virtual void translate(int vertical, int horizontal) = 0;
	virtual void scale(double width, double height) = 0;
	virtual void writeInFile(std::ofstream& of) const = 0;
	size_t getId();


	void show();
	void hide();

	void group();
	void ungroup();

	bool getState() const;
	bool getIsInGroup() const;


protected:
	


	std::vector<Point> points;

	const size_t id;
	std::string stroke;
	std::string fill;
	bool isHidden;
	bool isInGroup;

	static size_t createdCount;
	static std::vector<std::string> colors;

private:

	void setStroke(const std::string& stroke);
	void setFill(const std::string& fill);
};

static std::vector<std::string> getColors();


//Base class of creator that reads input from a user
class FigureUserCreator
{
public:
	FigureUserCreator(const std::string& cmd);
	virtual ~FigureUserCreator() = default;
	bool isValid(const std::string& cmd) const;

	virtual Figure* createFigure(std::istream& is) const = 0;

protected:
	std::string cmd;
};

//Base class of creator that reads input from a file
class FigureFileCreator
{
public:
	FigureFileCreator(const std::string& cmd);
	virtual ~FigureFileCreator() = default;
	bool isValid(const std::string& cmd) const;

	virtual Figure* createFigure(std::ifstream& ifs) const = 0;

protected:
	std::string cmd;
};