#include "Shell.hpp"

Shell::Shell(){}

void Shell::systemInit(Disc disc, Memory memory, Scheduler scheduler){
	_disc = disc;
	_memory = memory;
	_scheduler = scheduler;
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
		_disc.create_file(parameters[1]);
	} else if (parameters[0] == "wf") {
		if (parameters.size() != 3) {
			ErrorPM();
			return 0;
		}
		if(_disc.open_file(parameters[1], true)){
			_disc.write_file(parameters[1], parameters[2]);
			_disc.close_file(parameters[1], true);
		}
	} else if (parameters[0] == "catf") {
		if (parameters.size() != 2) {
			ErrorPM();
			return 0;
		}
		_disc.print_file(parameters[1]);
	} else if (parameters[0] == "rmf") {
		if (parameters.size() != 2) {
			ErrorPM();
			return 0;
		}
		_disc.delete_file(parameters[1]);
	} else if (parameters[0] == "lsf") {
		if (parameters.size() != 1) {
			ErrorPM();
			return 0;
		}
		_disc.print_file_list();
	} else if (parameters[0] == "rnamef") {
		if (parameters.size() != 3) {
			ErrorPM();
			return 0;
		}
		if(_disc.open_file(parameters[1], true)){
			_disc.rename_file(parameters[1], parameters[2]);
			_disc.close_file(parameters[2], true);
		}
	}else if(parameters[0] == "weof"){
		if(parameters.size() != 3){
			ErrorPM();
			return 0;
		}
		if(_disc.open_file(parameters[1], true)){
			_disc.add_to_file(parameters[1], parameters[2]);
			_disc.close_file(parameters[1], true);
		}
	} else if (parameters[0] == "disk") {
		if (parameters.size() != 1) {
			ErrorPM();
			return 0;
		}
		_disc.printDisc();
	} else if (parameters[0] == "memory") {
		if (parameters.size() != 1) {
			ErrorPM();
			return 0;
		}
		_memory.show();
	} else if (parameters[0] == "exfile") {
		if (parameters.size() != 1) {
			ErrorPM();
			return 0;
		}
		_memory.showExchangeFile();
	} else if (parameters[0] == "fifo") {
		if (parameters.size() != 1) {
			ErrorPM();
			return 0;
		}
		_memory.showFIFO();
	} else if (parameters[0] == "pagetable") {
		if (parameters.size() != 2) {
			ErrorPM();
			return 0;
		}
		try{
			_memory.showPageTable(std::stoi(parameters[1]));
		}catch(std::exception exception){
			ErrorIP();
			return 0;
		}
	} else if (parameters[0] == "lsp") {
		if (parameters.size() == 1) {
			PrintProcessListInformation();
		} else if (parameters.size() == 2) {
			if (parameters[1] == "-a") {
				_scheduler.wyswietl_gotowe();
			} else if (parameters[1] == "-r") {
				_scheduler.print_running();
			} else {
				ErrorPM();
				return 0;
			}
		} else {
			ErrorPM();
			return 0;
		}
	} else if (parameters[0] == "cp") {
		if (parameters.size() == 3) {
			try{
				NewProcess(parameters[1], std::stoi(parameters[2]));
				_scheduler.Schedule();
			} catch (std::exception exception) {
				ErrorIP();
				return 0;
			}			
		} else if (parameters.size() == 4) {
			try {
				auto process = NewProcess(parameters[1], std::stoi(parameters[2]));
				process->SetFileName(parameters[3]);
				_scheduler.Schedule();
			}catch(std::exception exception){
				ErrorIP();
				return 0;
			}
		} else {
			ErrorPM();
			return 0;
		}
	} else if (parameters[0] == "killp") {
		if (parameters.size() != 2) {
			ErrorPM();
			return 0;
		}
		try{
			DeleteProcess(std::stoi(parameters[1]));
		}catch(std::exception exception){
			ErrorIP();
			return 0;
		}
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

void Shell::ErrorIP(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	std::cout << "Zla forma parametru" << "\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void Shell::printHelp(){
	std::cout << "exit                                             zakonczenie dzialania\n";
	std::cout << "cf          nazwa_pliku                          utworzenie pliku\n";
	std::cout << "wf          nazwa_pliku  dane                    zapis do pliku\n";
	std::cout << "catf        nazwa_pliku                          zawartosc pliku\n";
	std::cout << "rmf         nazwa_pliku                          usuniecie pliku\n";
	std::cout << "lsf                                              lista plikow\n";
	std::cout << "rnamef      nazwa_pliku  nowa_nazwa              zmiana nazwy pliku\n";
	std::cout << "weof        nazwa_pliku  dane                    zapis na koniec pliku\n";
	std::cout << "disk                                             informacje o dysku\n";
	std::cout << "memory                                           zawartosc pamieci\n";
	std::cout << "exfile                                           zawartosc pliku wymiany\n";
	std::cout << "fifo                                             zawartosc kolejki fifo\n";
	std::cout << "pagetable   pid                                  zawartosc pliku wymiany\n";
	std::cout << "lsp                                              lista procesow\n";
	std::cout << "lsp -r                                           proces aktualnie wykonywany\n";
	std::cout << "lsp -a                                           lista procesow gotowych\n";
	std::cout << "cp          nazwa_procesu id_grupy               utworzenie procesu pustego\n";
	std::cout << "cp          nazwa_procesu id_grupy nazwa_pliku   utworzenie procesu z pliku\n";
	std::cout << "killp                                            usuniecie procesu";
}
