#pragma once

#include "../dysk/dysk.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class Shell{
public:
	static void Loop();
private:
	static std::string waitUserInput();
	static std::vector<std::string> parseInput(std::string input);
	static bool executeCommand(std::vector<std::string> parameters);
};