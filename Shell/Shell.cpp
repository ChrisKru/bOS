#include "Shell.hpp"

Shell::Shell(){
	disc = Disc();
}

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
	if(parameters.size() == 0){
		return 0;
	}
	if (parameters[0] == "exit") {
		if(parameters.size() != 1){
			ErrorPM();
			return 0;
		}
		return 1;
	} else if (parameters[0] == "lsp") {
		if(parameters.size() != 1){
			ErrorPM();
			return 0;
		}
		PrintProcessListInformation();
	} else if (parameters[0] == "cf") {
		if (parameters.size() != 2) {
			ErrorPM();
			return 0;
		}
		disc.create_file(parameters[1]);
	} else if (parameters[0] == "wf") {
		if (parameters.size() != 3) {
			//przyjeb edytor
			ErrorPM();
			return 0;
		}
		disc.open_file(parameters[1]);
		disc.write_file(parameters[1], parameters[2]);
		disc.close_file(parameters[1]);
	}else if(parameters[0] == "catf"){
		if(parameters.size() != 2){
			ErrorPM();
			return 0;
		}
		disc.open_file(parameters[1]);
		disc.print_file(parameters[1]);
		disc.close_file(parameters[1]);
	}else if(parameters[0] == "dinfo"){
		if (parameters.size() != 1) {
			ErrorPM();
			return 0;
		}
		disc.printDisc();
	}else{
		ErrorIC();
	}
	return 0;
}

void Shell::ErrorIC(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	std::cout << "Nie znaleziono komendy" << "\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void Shell::ErrorPM(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	std::cout << "Zla liczba parametrow" << "\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
