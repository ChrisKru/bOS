#pragma once

#include "../dysk/dysk.h"
#include "../Memory/Memory.h"
#include "../Interpreter/Interpreter.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <Windows.h>

class Shell{
public:
	Shell();
	void systemInit(Disc disc, Memory memory, Scheduler scheduler, Interpreter interpreter);
	void Loop();
private:
	std::string waitUserInput();
	std::vector<std::string> parseInput(std::string input);
	bool executeCommand(std::vector<std::string> parameters);
	void ErrorIC();
	void ErrorPM();
	void ErrorIP();
	void printHelp();
	Disc _disc;
	Memory _memory;
	Scheduler _scheduler;
	Kolejka _kolejka;
	Interpreter _interpreter;
};