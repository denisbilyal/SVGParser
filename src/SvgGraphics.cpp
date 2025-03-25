#define _USE_MATH_DEFINES

#include "FigureFactory.h"
#include "SvgGraphics.h"
#include <cmath>
#include <iostream>

SvgGraphics& SvgGraphics::getInstance()
{
	static SvgGraphics obj;
	return obj;
}

SvgGraphics::~SvgGraphics()
{
	for (Figure* figure : figures)
	{
		delete figure;
		figure = nullptr;
	}
}

void SvgGraphics::open(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw std::logic_error("File not opened for reading!");
	}

	while (!file.eof())
	{
		char sym;
		file.get(sym);

		//see if it is a new command
		if (sym == '<')
		{
			//check for start of the file
			checkStart(file);

			//check if we reached the end of file
			if (checkEnd(file))
				break;
			//add the figure
			try
			{
				addFigure(FigureFactory::getInstance().createFileFigure(file));
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


	file.close();
	this->fileName = fileName;
	std::cout << fileName << " opened successfully." << std::endl;
}

void SvgGraphics::print() const
{
	for (const Figure* figure : figures)
	{
		if (!figure->getIsInGroup() && !figure->getState())
			figure->print();
	}
}

void SvgGraphics::save()
{
	saveAs(fileName);
}

void SvgGraphics::saveAs(const std::string& fileName)
{
	std::ofstream file(fileName);
	if (!file.is_open())
	{
		throw std::logic_error("File not opened for writing!");
	}

	file << "<svg width=\"" << 3000 << "\" height=\"" << 3000 << "\"  xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

	for (Figure* figure : figures)
	{
		if (!figure->getIsInGroup() && !figure->getState())
			figure->writeInFile(file);
	}

	file << "</svg>";
	
	file.close();
	std::cout << fileName << " saved successfully." << std::endl;
}

void SvgGraphics::create(std::istream& is)
{
	figures.push_back(FigureFactory::getInstance().createUserFigure(is));
}

void SvgGraphics::translate(size_t elemId, int vertical, int horizontal)
{
	//translate a figure that is not in group
	for (Figure* figure : figures)
	{
		if (figure->getId() == elemId)
		{
			if (!figure->getIsInGroup())
			{
				figure->translate(vertical, horizontal);
				return;
			}
			else
				throw std::logic_error("This element is in group!");
			
		}
	}
	throw std::invalid_argument("This element doesn't exist!");
}

void SvgGraphics::scale(size_t elemId, double width, double height)
{
	//scale a figure that is not in group
	for (Figure* figure : figures)
	{
		if (figure->getId() == elemId)
		{
			if (!figure->getIsInGroup())
			{
				figure->scale(width, height);
				return;
			}
			else
				throw std::logic_error("This element is in group!");

		}
	}
	throw std::invalid_argument("This element doesn't exist!");
	
}

void SvgGraphics::translate(int vertical, int horizontal)
{
	//translate all, but skip these in group(their group will translate them)
	for (Figure* figure : figures)
	{
		if (!figure->getIsInGroup())
			figure->translate(vertical, horizontal);
		
	}
}

void SvgGraphics::scale(double width, double height)
{
	//translate all, but skip these in group(their group will scale them)
	for (Figure* figure : figures)
	{
		if (!figure->getIsInGroup())
			figure->scale(width, height);

	}
}

void SvgGraphics::group(int x1, int y1, int x2, int y2)
{
	//get all figures in area
	std::vector<Figure*> res;
	for (Figure* figure : figures)
	{
		if (!figure->getIsInGroup() && figure->inArea(x1, y1, x2, y2))
			res.push_back(figure);
		
	}
	
	//create the group
	Group* newGroup = new Group(res);
	figures.push_back(newGroup);
	groups.push_back(newGroup);
}

void SvgGraphics::ungroup(size_t groupId)
{
	Group* group = findGroup(groupId);

	if (!group)
	{
		throw std::invalid_argument("This group doesn't exists!");
	}
	if (group->getIsInGroup())
	{
		throw std::invalid_argument("This group is in another group!");
	}

	//first remove from groups then in figures
	removeGroup(groupId);
	removeElem(groupId);
}

void SvgGraphics::insert(size_t groupId, size_t elemId)
{
	if (groupId == elemId)
	{
		throw std::logic_error("The IDs can't be the same!");
	}
	
	Group* group = findGroup(groupId);
	Figure* figure = findElem(elemId);

	if (!group)
	{
		throw std::invalid_argument("This group doesn't exists!");
	}

	if (!figure)
	{
		throw std::invalid_argument("This element doesn't exists!");
	}

	if (figure->getIsInGroup())
	{
		throw std::invalid_argument("This element is already in a group!");
	}

	group->addFigure(figure);
}

void SvgGraphics::extract(size_t groupId, size_t elemId)
{
	Group* group = findGroup(groupId);
	Figure* figure = findElem(elemId);

	if (!group)
	{
		throw std::invalid_argument("This group doesn't exists!");
	}

	if (!figure)
	{
		throw std::invalid_argument("This element doesn't exists!");
	}

	group->removeFigure(elemId);

}

void SvgGraphics::hide(size_t elemId)
{
	for (Figure* figure : figures)
	{
		if (figure->getId() == elemId)
		{
			if (!figure->getIsInGroup())
			{
				figure->hide();
				return;
			}
			throw std::invalid_argument("This element is in a group!");
		}
	}
	throw std::invalid_argument("This element doesn't exist!");
}

void SvgGraphics::show(size_t elemId)
{
	for (Figure* figure : figures)
	{
		if (figure->getId() == elemId)
		{
			if (!figure->getIsInGroup())
			{
				figure->show();
				return;
			}
			throw std::invalid_argument("This element is in a group!");
		}
	}
	throw std::invalid_argument("This element doesn't exist!");
}

void SvgGraphics::help()
{
	std::cout << "open <file>   				   - opens a file\n"
			  << "print 	      				   - prints figures\n"
			  << "save          				   - saves all files\n"
			  << "saveas <file> 				   - saves all files\n"
			  << "create <figure> <components>    	   - creates a figure\n"
			  << "\nrectangle <x> <y> <width> <height> <stroke> <fill>\n"
			  << "arc 	   <cx> <cy> <r> <startingAngle> <lengthAngle> <stroke> <fill>\n"
			  << "polygon   <countPoints> [<x> <y>] <stroke> <fill>\n"
			  << "line      <x1> <y1> <x2> <y2> <stroke>\n\n"
			  << "translate <id> vertical=<v> horizontal=<h> - translates a figure\n"
			  << "translate vertical=<v> horizontal=<h>      - translates all figures\n"
			  << "scale <id> <width> <height>                - scales a figure\n"
			  << "scale <id> <width> <height>                - scales all figures\n"
			  << "group <x1> <y1> <x2> <y2>		   - groups all figures in this region\n"
			  << "ungroup <groupId>   			   - deletes a group\n"
			  << "insert <groupId> <elemId>		   - add a figure to a group\n"
			  << "extract <groupId> <elemId>		   - remove a figure from a group\n"
			  << "hide <id>				   - figure is not printed or saved\n"
			  << "show <id> 				   - removes the hide command\n"
			  << "help					   - show commands\n";
}

void SvgGraphics::addFigure(Figure* elem)
{
	figures.push_back(elem);
}

void SvgGraphics::addGroup(Group* group)
{
	groups.push_back(group);
}

void SvgGraphics::checkStart(std::ifstream& file)
{
	size_t curr = file.tellg();
	if (file.get() == 's')
	{
		if (file.get() == 'v')
		{
			if (file.get() == 'g')
			{
				char buffer[1024];
				file.getline(buffer, 1024, '<');
				return;
			}
		}
	}
	file.seekg(curr);
}

bool SvgGraphics::checkEnd(std::ifstream& file)
{
	size_t curr = file.tellg();
	if (file.get() == '/')
	{
		if (file.get() == 's')
		{
			if (file.get() == 'v')
			{
				if (file.get() == 'g')
				{
					return true;
				}
			}
		}
	}
	file.seekg(curr);
	return false;
}

Figure* SvgGraphics::findElem(size_t id)
{
	int left = 0, right = figures.size() - 1;
	while (right >= left)
	{
		int mid = (left + right) / 2;

		if (figures[mid]->getId() > id)
		{
			right = mid - 1;
		}
		else if (figures[mid]->getId() < id)
		{
			left = mid + 1;
		}
		else
		{
			return figures[mid];
		}
	}

	return nullptr;
}


Group* SvgGraphics::findGroup(size_t id)
{
	//binary search
	int left = 0, right = groups.size() - 1;
	while (right >= left)
	{
		int mid = (left + right) / 2;

		if (groups[mid]->getId() > id)
		{
			right = mid - 1;
		}
		else if (groups[mid]->getId() < id)
		{
			left = mid + 1;
		}
		else
		{
			return groups[mid];
		}
	}
	return nullptr;
}

void SvgGraphics::removeElem(size_t id)
{
	size_t size = figures.size();
	for (size_t i = 0; i < size; i++)
	{
		if (figures[i]->getId() == id && !figures[i]->getIsInGroup())
		{
			for (size_t j = i; j < size - 1; j++)
			{
				std::swap(figures[j], figures[j + 1]);
			}
		}
	}
	delete figures.back();
	figures.pop_back();
}

void SvgGraphics::removeGroup(size_t id)
{
	size_t size = groups.size();
	for (size_t i = 0; i < size; i++)
	{
		if (groups[i]->getId() == id && !groups[i]->getIsInGroup())
		{
			for (size_t j = i; j < size - 1; j++)
			{
				std::swap(groups[j], groups[j + 1]);
			}
		}
	}
	groups.pop_back();
}



