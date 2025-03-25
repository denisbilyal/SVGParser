#pragma once
#include "Figure.h"
class FigureFactory
{
public:
	static FigureFactory& getInstance();
	void addUserCreator(const FigureUserCreator* creator);
	void addFileCreator(const FigureFileCreator* creator);

	Figure* createUserFigure(std::istream& is);
	Figure* createFileFigure(std::ifstream& ifs);

private:

	const FigureUserCreator* getUserCreator(const std::string& cmd);
	const FigureFileCreator* getFileCreator(const std::string& cmd);

	FigureFactory() = default;
	~FigureFactory() = default;

	FigureFactory(const FigureFactory& other) = delete;
	FigureFactory& operator=(const FigureFactory& other) = delete;

	std::vector<const FigureUserCreator*> userCreators;
	std::vector<const FigureFileCreator*> fileCreators;
};

