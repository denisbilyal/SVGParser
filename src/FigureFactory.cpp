#include "FigureFactory.h"
#include <iostream>


FigureFactory& FigureFactory::getInstance()
{
    static FigureFactory obj;
    return obj;
}

void FigureFactory::addUserCreator(const FigureUserCreator* creator)
{
    //add the user creator to factory
    userCreators.push_back(creator);
}

void FigureFactory::addFileCreator(const FigureFileCreator* creator)
{
    //add the file creator to factory
    fileCreators.push_back(creator);
}

Figure* FigureFactory::createUserFigure(std::istream& is)
{
    //get input
    std::string cmd;
    is >> cmd;

    //find the creator
    const FigureUserCreator* creator = getUserCreator(cmd);

    if (creator)
    {
        return creator->createFigure(is);
    }
    else
    {
        throw cmd;
    }
}

Figure* FigureFactory::createFileFigure(std::ifstream& ifs)
{
    //get input
    std::string cmd;
    ifs >> cmd;

    //get the creator
    const FigureFileCreator* creator = getFileCreator(cmd);
    if (creator)
    {
        return creator->createFigure(ifs);
    }
    else
    {
        ifs.ignore(LLONG_MAX, '>');
        throw cmd;
    }
}

const FigureUserCreator* FigureFactory::getUserCreator(const std::string& cmd)
{
    //find the creator
    for (const FigureUserCreator* creator : userCreators)
    {
        if (creator->isValid(cmd))
        {
            return creator;
        }
    }
    return nullptr;
}

const FigureFileCreator* FigureFactory::getFileCreator(const std::string& cmd)
{
    //find the creator
    for (const FigureFileCreator* creator : fileCreators)
    {
        if (creator->isValid(cmd))
        {
            return creator;
        }
    }
    return nullptr;
}
