#pragma once

#include "Arc.h"
#include "Group.h"
#include "Line.h"
#include "Polygon.h"
#include "Rectangle.h"
 
class SvgGraphics
{
public:

	static SvgGraphics& getInstance();
	~SvgGraphics();

	void open(const std::string& fileName); 
	void print() const;
	void save();
	void saveAs(const std::string& fileName);

	void create(std::istream& is);
	void translate(size_t elemId, int vertical, int horizontal);
	void scale(size_t elemId, double width, double height);
	void translate(int vertical, int horizontal);
	void scale(double width, double height);

	void group(int x1, int y1, int x2, int y2);
	void ungroup(size_t groupId);
	void insert(size_t groupId, size_t elemId);
	void extract(size_t groupId, size_t elemId);

	void hide(size_t elemId);
	void show(size_t elemId);

	void help();

	void addFigure(Figure* elem);
	void addGroup(Group* group);

	void checkStart(std::ifstream& file);
	bool checkEnd(std::ifstream& file);

private:

	std::vector<Figure*> figures;
	std::vector<Group*> groups;
	std::string fileName;

	SvgGraphics() = default;
	SvgGraphics(const SvgGraphics& other) = delete;
	SvgGraphics& operator=(const SvgGraphics& other) = delete;

	Figure* findElem(size_t id);
	Group* findGroup(size_t id);
	void removeElem(size_t id);
	void removeGroup(size_t id);
};

