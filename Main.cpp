#include "Shell/Shell.hpp"

int main(){
	Disc disc;
	Memory memory;
	Shell shell;
	shell.systemInit(disc, memory);
	shell.Loop();
	return 0;
}