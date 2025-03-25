#include "Group.h"
#include "FigureFactory.h"
#include "SvgGraphics.h"
#include <iostream>

Group::Group(const std::vector<Figure*>& figures) : Figure("none", "none")
{
	//get access to the figures
    data = figures;
	std::cout << "Successfully created Group! ID: " << id << std::endl << "  Grouped elements ID: ";
	for (Figure* figure : figures)
	{
		std::cout << figure->getId() << " | ";
		figure->group();
	}
	std::cout << std::endl;

}

Group::~Group()
{
	for (Figure* figure : data)
	{
		if (figure)
		{
			figure->ungroup();
		}
		
	}
}

bool Group::inArea(int x1, int y1, int x2, int y2) const
{
	//check if every figure of the group is in area
	for (const Figure* figure : data)
	{
		if (figure->inArea(x1, y1, x2, y2) == false)
			return false;
	}
	return true;
}

void Group::print() const
{
	std::cout << id << ". group:" << std::endl;
	for (const Figure* figure : data)
	{
		std::cout << "  ";
		figure->print();
	}
}

void Group::translate(int vertical, int horizontal)
{
	for (Figure* x : data)
	{
		x->translate(vertical, horizontal);
	}
}

void Group::scale(double width, double height)
{
	for (Figure* x : data)
	{
		x->scale(width, height);
	}
}

void Group::addFigure(Figure* elem)
{
	elem->group();
	data.push_back(elem);
}

void Group::removeFigure(size_t elemId)
{
	size_t size = data.size();
	for (size_t i = 0; i < size; i++)
	{
		if (data[i]->getId() == elemId)
		{
			data[i]->ungroup();
			std::swap(data[i], data[size - 1]);
			data.pop_back();
			return;
		}
	}
	throw std::logic_error("There is no element with such id in this group!");
}

void Group::writeInFile(std::ofstream& of) const
{
	if (!of.is_open())
	{
		throw std::logic_error("File not opened for writing!");
	}

	of << "<g>" << std::endl;
	for (const Figure* figure : data)
	{
		figure->writeInFile(of);
	}
	of << "</g>" << std::endl;
}

//GroupUserCreator::GroupUserCreator() : FigureUserCreator("group")
//{
//}
//
//Figure* GroupUserCreator::createFigure(std::istream& is) const
//{
//	int x1, y1, x2, y2;
//	is >> x1 >> y1 >> x2 >> y2;
//
//	// why not in svg?
//	return nullptr;
//}
//static GroupUserCreator _;

GroupFileCreator::GroupFileCreator() : FigureFileCreator("g>")
{

}

Figure* GroupFileCreator::createFigure(std::ifstream& ifs) const
{
	if (!ifs.is_open())
	{
		throw std::logic_error("File not opened for reading!");
	}

	std::vector<Figure*> res;
	while (true)
	{
		char sym;
		ifs.get(sym);
		if (sym == '<')
		{
			char check;
			ifs.get(check);
			//check if it reached its ending
			if (check == '/')
			{
				ifs.get(check);
				if (check == 'g')
					break;
				else
					ifs.seekg(-2, std::ios::cur);
			}
			else
				ifs.seekg(-1, std::ios::cur);

			
			//create figure
			try
			{
				Figure* elem = FigureFactory::getInstance().createFileFigure(ifs);
				SvgGraphics::getInstance().addFigure(elem);
				res.push_back(elem);
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
			catch (const std::string& str)
			{
				std::cout << "The system doesn't support this element - " << str << std::endl;
			}
		}
	}
	return new Group(res);
}

static GroupFileCreator ___;