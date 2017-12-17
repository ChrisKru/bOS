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
	bool edit_mode = false;
	while(parser >> param){
		parameters.push_back(param);
		if (edit_mode) {
			getline(parser, param);
			param.erase(0, 1);
			parameters.push_back(param);
			break;
		}
		if (param == "wf" || param == "weof") {
			edit_mode = true;
		}
	}
	return parameters;
}

bool Shell::executeCommand(std::vector<std::string> parameters) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE);
	if (parameters.size() == 0) {
		return 0;
	}
	if (parameters[0] == "exit") {
		if (parameters.size() != 1) {
			ErrorPM();
			return 0;
		}
		return 1;
	}else if(parameters[0] == "help"){
		if(parameters.size() != 1){
			ErrorPM();
			return 0;
		}
		printHelp();
	} else if (parameters[0] == "cf") {
		if (parameters.size() != 2) {
			ErrorPM();
			return 0;
		}
		disc.create_file(parameters[1]);
	} else if (parameters[0] == "wf") {
		if (parameters.size() != 3) {
			ErrorPM();
			return 0;
		}
		disc.open_file(parameters[1]);
		disc.write_file(parameters[1], parameters[2]);
		disc.close_file(parameters[1]);
	} else if (parameters[0] == "catf") {
		if (parameters.size() != 2) {
			ErrorPM();
			return 0;
		}
		disc.open_file(parameters[1]);
		disc.print_file(parameters[1]);
		disc.close_file(parameters[1]);
	} else if (parameters[0] == "rmf") {
		if (parameters.size() != 2) {
			ErrorPM();
			return 0;
		}
		disc.delete_file(parameters[1]);
	} else if (parameters[0] == "lsf") {
		if (parameters.size() != 1) {
			ErrorPM();
			return 0;
		}
		disc.print_file_list();
	} else if (parameters[0] == "rnamef") {
		if (parameters.size() != 3) {
			ErrorPM();
			return 0;
		}
		disc.open_file(parameters[1]);
		disc.rename_file(parameters[1], parameters[2]);
		disc.close_file(parameters[2]);
	}else if(parameters[0] == "weof"){
		if(parameters.size() != 3){
			ErrorPM();
			return 0;
		}
		disc.open_file(parameters[1]);
		disc.add_to_file(parameters[1], parameters[2]);
		disc.close_file(parameters[1]);
	}else if(parameters[0] == "lsdisk"){
		if (parameters.size() != 1) {
			ErrorPM();
			return 0;
		}
		disc.printDisc();
	}else{
		ErrorIC();
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	return 0;
}

void Shell::ErrorIC(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	std::cout << "Nie znaleziono komendy" << "\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void Shell::ErrorPM(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	std::cout << "Zla liczba parametrow" << "\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void Shell::printHelp(){
	std::cout << "exit                              zakonczenie dzialania\n";
	std::cout << "cf      nazwa_pliku               utworzenie pliku\n";
	std::cout << "wf      nazwa_pliku dane          zapis do pliku\n";
	std::cout << "catf    nazwa_pliku               zawartosc pliku\n";
	std::cout << "rmf     nazwa_pliku               usuniecie pliku\n";
	std::cout << "lsf                               lista plikow\n";
	std::cout << "rnamef   nazwa_pliku nowa_nazwa    zmiana nazwy pliku\n";
	std::cout << "weof    nazwa_pliku dane          zapis na koniec pliku\n";
	std::cout << "lsdisk                            informacje o dysku\n";
}
