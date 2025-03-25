#include <iostream>
#include "SvgGraphics.h"

void cli();
bool isNumber(const char* str);
int main()
{
	cli();

}

void cli()
{
	while (true)
	{
		try
		{
			std::string command;
			std::cin >> command;
			SvgGraphics& svg = SvgGraphics::getInstance();
			if (command == "open")
			{
				std::string filename;
				std::cin >> filename;
				svg.open(filename);
			}
			else if (command == "save")
			{
				svg.save();
			}
			else if (command == "saveas")
			{
				std::string filename;
				std::cin >> filename;
				svg.saveAs(filename);
			}
			else if (command == "exit")
			{
				break;
			}
			else if (command == "print")
			{
				svg.print();
			}
			else if (command == "create")
			{
				
				svg.create(std::cin);
			}
			else if (command == "group")
			{
				int x1, y1, x2, y2;
				std::cin >> x1 >> y1 >> x2 >> y2;
				svg.group(x1, y1, x2, y2);
			}
			else if (command == "ungroup")
			{
				size_t id;
				std::cin >> id;
				svg.ungroup(id);
			}
			else if (command == "insert")
			{
				size_t groupId;
				size_t elemId;
				std::cin >> groupId >> elemId;
				svg.insert(groupId, elemId);
			}
			else if (command == "extract")
			{
				size_t groupId;
				size_t elemId;
				std::cin >> groupId >> elemId;
				svg.extract(groupId, elemId);
			}
			else if (command == "hide")
			{
				size_t id;
				std::cin >> id;
				svg.hide(id);
			}
			else if (command == "show")
			{
				size_t id;
				std::cin >> id;
				svg.show(id);
			}
			else if (command == "translate")
			{
				std::cin.ignore();
				char buffer[64];
				std::cin.getline(buffer, 64);
				std::vector<std::string> words;
				stringFuncs::splitValues(words, buffer, ' ');

				if (words.size() == 2)
				{
					size_t vert = words[0].find("vertical=");
					size_t hori = words[1].find("horizontal=");
					if (vert == std::string::npos || hori == std::string::npos)
					{
						throw std::invalid_argument("Invalid command parameters!");
					}

					std::string verticalValue(words[0].substr(9));
					std::string horizontalValue(words[1].substr(11));

					if (!isNumber(verticalValue.c_str()) || !isNumber(horizontalValue.c_str()))
					{
						throw std::invalid_argument("Invalid command parameters!");
					}

					svg.translate(stoi(verticalValue), stoi(horizontalValue));
				}
				else if (words.size() == 3)
				{
					

					size_t vert = words[1].find("vertical=");
					size_t hori = words[2].find("horizontal=");
					if (vert == std::string::npos || hori == std::string::npos || !isNumber(words[0].c_str()))
					{
						throw std::invalid_argument("Invalid command parameters!");
					}

					std::string verticalValue(words[1].substr(9));
					std::string horizontalValue(words[2].substr(11));

					if (!isNumber(verticalValue.c_str()) || !isNumber(horizontalValue.c_str()))
					{
						throw std::invalid_argument("Invalid command parameters!");
					}

					svg.translate(stoi(words[0]), stoi(verticalValue), stoi(horizontalValue));
				}
				else
				{
					throw std::invalid_argument("Invalid command parameters!");
				}
			}
			else if (command == "scale")
			{
				std::cin.ignore();
				char buffer[64];
				std::cin.getline(buffer, 64);
				std::vector<std::string> words;
				stringFuncs::splitValues(words, buffer, ' ');

					if (words.size() == 2)
					{
						if (!isNumber(words[0].c_str()) || !isNumber(words[1].c_str()))
						{
							throw std::invalid_argument("Invalid command parameters!");
						}
						svg.scale(stoi(words[0]), stoi(words[1]));
					}
					else if (words.size() == 3)
					{
						if(!isNumber(words[0].c_str()) || !isNumber(words[1].c_str()) || !isNumber(words[2].c_str()))
						{
							throw std::invalid_argument("Invalid command parameters!");
						}
						svg.scale(stoi(words[0]), stod(words[1]), stod(words[2]));
					}
					else
					{
						throw std::invalid_argument("Invalid command parameters!");
					}
			}
			else if (command == "help")
			{
				svg.help();
			}
			else
			{
				throw std::invalid_argument("Invalid command!");
			}
		}
		catch (const std::exception& e)
		{
			std::cin.ignore(LLONG_MAX, '\n');
			std::cout << e.what() << std::endl;
		}
		
	}
}

bool isNumber(const char* str)
{
	if (*str < '0' || *str > '9')
	{
		if (*str != '-')
		{
			return false;
		}
		str++;
	}

	size_t dotCount = 0;

	while (*str)
	{
		if (*str == '.')
		{
			dotCount++;
		}
		else if (*str < '0' || *str > '9')
		{
			return false;
		}
		
		str++;
	}
	return dotCount <= 1 && *(str - 1) != '.';
}
