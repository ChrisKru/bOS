#include "Shell/Shell.hpp"

int main(){
	Disc disc;
	Memory memory;
	Scheduler scheduler(0);
	Kolejka kolejka;
	Interpreter interpreter;
	Shell shell;
	shell.systemInit(std::move(disc), std::move(memory), std::move(scheduler), std::move(kolejka), std::move(interpreter));
	shell.Loop();
	return 0;
}