#include "Shell.hpp"

void Shell::Loop(){
	while(true){
		if(executeCommand(parseInput(waitUserInput()))){
			return;
		}
	}
}

std::string Shell::waitUserInput(){
	std::string input{};
	std::getline(std::cin, input);
	return input;
}

std::vector<std::string> Shell::parseInput(std::string input){
	std::vector<std::string> parameters{};
	std::istringstream parser(input);
	std::string param{};
	while(parser >> param){
		parameters.push_back(param);
	}
	return parameters;
}

bool Shell::executeCommand(std::vector<std::string> parameters){
	Disc disc;
	if(parameters.size() == 0){
		return 0;
	}
	if (parameters[0] == "exit") {
		return 1;
	} else if (parameters[0] == "lsp") {
		PrintProcessListInformation();
	}else if(parameters[0] == "cf"){
		
	}else if(parameters[0] == "dinfo"){
		disc.printDisc();
	}else{
		std::cout << "command not found" << "\n";
		//w razie zlego parametru - invalid option
		//prosto z basza
	}
	return 0;
}
