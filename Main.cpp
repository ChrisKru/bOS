#include "Shell/Shell.hpp"

int main(){
	Disc disc;
	Memory memory;
	Scheduler scheduler(0);
	Shell shell;
	shell.systemInit(std::move(disc), std::move(memory), std::move(scheduler));
	shell.Loop();
	return 0;
}