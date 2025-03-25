#include "FigureFactory.h"
#include "Figure.h"
#include <cmath>
#include <fstream>
#include <stdexcept>


//colors that the system supports
std::vector<std::string> Figure::colors = getColors();

//for id creation
size_t Figure::createdCount = 1;

Figure::Figure(const std::string& stroke, const std::string& fill) : id(createdCount), isHidden(false), isInGroup(false)
{
	//set colors
	setStroke(stroke);
	setFill(fill);

	//go to the next id
	createdCount++;
}
Figure::Figure(const Figure& other) : id(createdCount), isHidden(false), isInGroup(false)
{
	//go to the next id
	createdCount++;
}

size_t Figure::getId()
{
	return id;
}

void Figure::show()
{
	isHidden = false;
}

void Figure::hide()
{
	isHidden = true;
}

void Figure::group()
{
	isInGroup = true;
}

void Figure::ungroup()
{
	isInGroup = false;
}

bool Figure::getState() const
{
	return isHidden;
}

bool Figure::getIsInGroup() const
{
	return isInGroup;
}



void Figure::setStroke(const std::string& stroke)
{
	//see if our system supports the color
	for (const std::string& color : colors)
	{
		if (color == stroke)
		{
			this->stroke = stroke;
			return;
		}
	}
	//if not the default color is transparent(none)
	this->stroke = "none";
}

void Figure::setFill(const std::string& fill)
{
	//see if our system supports the color
	for (const std::string& color : colors)
	{
		if (color == fill)
		{
			this->fill = fill;
			return;
		}
	}
	//if not the default color is black
	this->fill = "black";
}


std::vector<std::string> getColors()
{

	//get the colors that the system supports from colors.txt
	std::vector<std::string> colors;
	std::ifstream colorsFile("colors.txt");
	if (!colorsFile.is_open())
	{
		throw std::logic_error("Colors file not opened!");
	}

	while (!colorsFile.eof())
	{
		char buffer[32];
		colorsFile >> buffer;
		colors.push_back(buffer);
	}

	colorsFile.close();
	return colors;
}



FigureUserCreator::FigureUserCreator(const std::string& cmd) : cmd(cmd)
{
	//add the user creator to the factory
	FigureFactory::getInstance().addUserCreator(this);
}

bool FigureUserCreator::isValid(const std::string& cmd) const
{
	return this->cmd == cmd;
}



FigureFileCreator::FigureFileCreator(const std::string& cmd) : cmd(cmd)
{
	//add the file creator to the factory
	FigureFactory::getInstance().addFileCreator(this);
}

bool FigureFileCreator::isValid(const std::string& cmd) const
{
	return this->cmd == cmd;
}

std::string stringFuncs::getValue(const std::string& text, const std::string& word)
{

	// getting the value of a word, for instance: color="blue", value = blue
	size_t i = text.find(word);
	if (i == std::string::npos)
	{
		throw std::invalid_argument("No such word!");
	}
	size_t first = text.find('\"', i) + 1;
	size_t length = text.find('\"', first) - first;

	if (text.find('=', i) - first <= 0)
		throw std::logic_error("error");



	return text.substr(first, length);
}


std::string stringFuncs::getValue(const std::string& text, char sym)
{
	// getting the value of a word, for instance: x="123", value = 123
	size_t i = text.find(sym);
	if (i == std::string::npos)
	{
		throw std::invalid_argument("No such word!");
	}
	size_t first = text.find('\"', i) + 1;
	size_t length = text.find('\"', first) - first;

	if (text.find('=', i) - first <= 0)
		throw std::logic_error("error");

	return text.substr(first, length);
}

void stringFuncs::splitValues(std::vector<std::string>& res, const std::string& text, char delim)
{
	size_t start, end = 0;
	while ((start = text.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = text.find(delim, start);
		res.push_back(text.substr(start, end - start));
	}
}