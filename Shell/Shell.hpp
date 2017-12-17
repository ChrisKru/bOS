#pragma once

#include "../dysk/dysk.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <Windows.h>

class Shell{
public:
	Shell();
	void Loop();
private:
	std::string waitUserInput();
	std::vector<std::string> parseInput(std::string input);
	bool executeCommand(std::vector<std::string> parameters);
	void ErrorIC();
	void ErrorPM();
	void printHelp();
	Disc disc;
};