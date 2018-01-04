#include "Interpreter.h"


Interpreter::Interpreter() {
	_PID = 0;
	_RegA = 0;
	_RegB = 0;
	_RegC = 0;
	_IP = 0;
}



void Interpreter::loadRegister() { // Running to wskaznik na PCB
	_PID = running->ProcessID;
	_RegA = running->A;
	_RegB = running->B;
	_RegC = running->C;
	_IP = running->CommandCounter;
}

std::string Interpreter::loadInstruction(Memory& RAM) {
	// wywolanie pamieci operacyjnej
	// zwraca nam rozkaz jaki ma zostac wykonany
	// return STRING Z ROZKAZEM
	return RAM.getCommand(_PID,_IP);
}

void Interpreter::setInstruction(Memory& RAM, int num_parameters, std::string rozkaz)
{
	std::string command;
	if (rozkaz == "runp") {
		command = loadInstruction(RAM);
	}
	else {
		command = rozkaz;
	}
	instruction[0] = "";
	instruction[1] = "";
	instruction[2] = "";
	instruction[3] = "";
	instruction[0] = command.substr(0, 2);
	command = command.substr(2);
	if (num_parameters == 1) {
		int i = 1;
		while (command[i] != ' ' && command[i] != '\0') {
			instruction[1] += command[i];
			i++;
		}
	}
	if (num_parameters == 2) {
		int i = 1;
		while (command[i] != ' ') {
			instruction[1] += command[i];
			i++;
		}
		i++;
		while (command[i] != ' ' && command[i] != '\0') {
			instruction[2] += command[i];
			i++;
		}
	}
	if (num_parameters == 3) {
		int i = 1;
		while (command[i] != ' ') {
			instruction[1] += command[i];
			i++;
		}
		i++;
		while (command[i] != ' ') {
			instruction[2] += command[i];
			i++;
		}
		i++;
		instruction[3] = command.substr(i);
	}
}



void Interpreter::eraseReg()
{
	_PID = 0;
	_RegA = 0;
	_RegB = 0;
	_RegC = 0;
	_IP = -1;
}

void Interpreter::saveRegisters() {
	running->SetCommandCounter(_IP);
	running->A = _RegA;
	running->B = _RegB;
	running->C = _RegC;
	running->Timmer++;
}

void Interpreter::showRegisters() {
	std::cout << "Stan rejestrow dla procesu: " << running->GetName() << " PID: " <<_PID << std::endl;
	std::cout << "A: " << _RegA << std::endl;
	std::cout << "B: " << _RegB << std::endl;
	std::cout << "C: " << _RegC << std::endl;
	std::cout << "Licznik rozkazow: " << _IP << std::endl;
}



void Interpreter::runInstruction(Disc& dysk, Memory& RAM, Scheduler& scheduler, std::string command) {

	if (running->ProcessState == State::AKTYWNY) {
		_done = true; // ustalamy, ¿e rozkaz siê wykona. Jak nast¹pi¹ probelmy to _done = false;
		loadRegister();
		std::string operation;
		// Rozkaz do wykonania
		if (command == "runp") {
			operation = loadInstruction(RAM).substr(0, 2);
			std::cout << "Rozkaz:$ " << RAM.getCommand(_PID, _IP) << " $ dla procesu o ID: " << _PID << " i nazwie: " << running->GetName() << std::endl;
		}
		else {
			operation = command.substr(0, 2);
			std::cout << "Rozkaz:$ " << RAM.getCommand(_PID, _IP) << " $ dla procesu o ID: " << _PID << " i nazwie: " << running->GetName() << std::endl;
		}

		/* Operacje logiczne */
		// MV nazwa_rejestru liczba
		if (operation == ("MV")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			if (d1 == "A") {
				if (d2 == "B") {
					_RegA = _RegB;
				}
				else if (d2 == "C") {
					_RegA = _RegC;
				}
				else if (d2 == "A") {
					_RegA = _RegA;
				}
				else if (d2[0] == '[') {
					// wczytaæ Macieja, przekazuj¹c mu liczbê
					bool good_param = true;
					d2 = d2.substr(1);
					std::string param = "";
					for (int i = 0; i < d2.length(); i++)
					{
						if (!isdigit(d2[i]))
						{
							good_param = false;
							break;
						}
						else {
							param += d2[i];
						}
					}
					// i tutaj Maciejowi przekazuje te liczbe: 200
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegA = std::stoi(d2);
				}
			}
			else if (d1 == "B") {
				if (d2 == "A") {
					_RegB = _RegA;
				}
				else if (d2 == "C") {
					_RegB = _RegC;
				}
				else if (d2 == "B") {
					_RegB = _RegB;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegB = std::stoi(d2);
				}
			}
			else if (d1 == "C") {
				if (d2 == "A") {
					_RegC = _RegA;
				}
				else if (d2 == "B") {
					_RegC = _RegB;
				}
				else if (d2 == "C") {
					_RegC = _RegC;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegC = std::stoi(d2);
				}
			}
		}

		// EQ nazwa_rejestru nazwa_rejestru
		else if (operation == ("EQ")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			if (d1 == d2 || d1 == "A" && d2 == "B" && _RegA == _RegB ||
				d1 == "A" && d2 == "C" && _RegA == _RegC ||
				d1 == "B" && d2 == "A" && _RegB == _RegA ||
				d1 == "B" && d2 == "C" && _RegB == _RegC ||
				d1 == "C" && d2 == "A" && _RegC == _RegA ||
				d1 == "C" && d2 == "B" && _RegC == _RegB) {
				//std::cout << "Wartosci sa sobie rowne!" << std::endl;
				_flagEQ = true;
			}
			else if(forEQ(d2)) {
				if (d1 == "A" && _RegA == std::stoi(d2) ||
					d1 == "B" && _RegB == std::stoi(d2) ||
					d1 == "C" && _RegC == std::stoi(d2)) {
					_flagEQ = true;
				}
				else {
					_flagEQ = false;
				}
			}
			else {
				_flagEQ = false;
				//std::cout << "Wartosci sa rozne!" << std::endl;
			}
		}
		// JP gdzie
		else if (operation == ("JP")) {
			setInstruction(RAM, 1,command);
			std::string d1 = instruction[1];
			_IP = std::stoi(d1);
		}
		// JT gdzie(jesli prawda)
		else if (operation == ("JT")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
			if (_flagEQ == true) _IP = std::stoi(d1);
		}
		// JF gdzie(jeœli falsz)
		else if (operation == ("JF")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
			if (_flagEQ == false) {
				_IP = std::stoi(d1);
				_IP--;
			}
		}
		else if (operation == ("SR")) {
			setInstruction(RAM, 0, command);
			showRegisters();
		}

		/* Operacje arytmetyczne */
		// AD nazwa_rejestru liczba/nazwa_rejestru
		else if (operation == ("AD")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			if (d1 == "A") {
				if (d2 == "B") {
					_RegA += _RegB;
				}
				else if (d2 == "C") {
					_RegA += _RegC;
				}
				else if (d2 == "A") {
					_RegA += _RegA;
				}

				else if (d2 == "[B]") {
					_RegA += std::stoi(RAM.readFromMemory(_RegB));
				}
				else if (d2 == "[C]") {
					_RegA += std::stoi(RAM.readFromMemory(_RegC));
				}
				else if (d2 == "[A]") {
					_RegA += std::stoi(RAM.readFromMemory(_RegA));
				}

				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegA += std::stoi(d2);
				}
			}
			else if (d1 == "B") {
				if (d2 == "A") {
					_RegB += _RegA;
				}
				else if (d2 == "C") {
					_RegB += _RegC;
				}
				else if (d2 == "B") {
					_RegB += _RegB;
				}
				else if (d2 == "[B]") {
					_RegB += std::stoi(RAM.readFromMemory(_RegB));
				}
				else if (d2 == "[C]") {
					_RegB += std::stoi(RAM.readFromMemory(_RegC));
				}
				else if (d2 == "[A]") {
					_RegB += std::stoi(RAM.readFromMemory(_RegA));
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegB += std::stoi(d2);
				}
			}
			else if (d1 == "C") {
				if (d2 == "A") {
					_RegC += _RegA;
				}
				else if (d2 == "B") {
					_RegC += _RegB;
				}
				else if (d2 == "C") {
					_RegC += _RegC;
				}

				else if (d2 == "[B]") {
					_RegC += std::stoi(RAM.readFromMemory(_RegB));
				}
				else if (d2 == "[C]") {
					_RegC += std::stoi(RAM.readFromMemory(_RegC));
				}
				else if (d2 == "[A]") {
					_RegC += std::stoi(RAM.readFromMemory(_RegA));
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegC += std::stoi(d2);
				}
			}
		}

		// ML nazwa_rejestru liczba/nazwa_rejestru
		else if (operation == ("ML")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			if (d1 == "A") {
				if (d2 == "B") {
					_RegA *= _RegB;
				}
				else if (d2 == "C") {
					_RegA *= _RegC;
				}
				else if (d2 == "A") {
					_RegA *= _RegA;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegA *= std::stoi(d2);
				}
			}
			else if (d1 == "B") {
				if (d2 == "A") {
					_RegB *= _RegA;
				}
				else if (d2 == "C") {
					_RegB *= _RegC;
				}
				else if (d2 == "B") {
					_RegB *= _RegB;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegB *= std::stoi(d2);
				}
			}
			else if (d1 == "C") {
				if (d2 == "A") {
					_RegC *= _RegA;
				}
				else if (d2 == "B") {
					_RegC *= _RegB;
				}
				else if (d2 == "C") {
					_RegC *= _RegC;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegC *= std::stoi(d2);
				}
			}
		}

		// SB nazwa_rejestru liczba/nazwa_rejestru
		else if (operation == ("SB")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			if (d1 == "A") {
				if (d2 == "B") {
					_RegA -= _RegB;
				}
				else if (d2 == "C") {
					_RegA -= _RegC;
				}
				else if (d2 == "A") {
					_RegA -= _RegA;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegA -= std::stoi(d2);
				}
			}
			else if (d1 == "B") {
				if (d2 == "A") {
					_RegB -= _RegA;
				}
				else if (d2 == "C") {
					_RegB -= _RegC;
				}
				else if (d2 == "B") {
					_RegB -= _RegB;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegB -= std::stoi(d2);
				}
			}
			else if (d1 == "C") {
				if (d2 == "A") {
					_RegC -= _RegA;
				}
				else if (d2 == "B") {
					_RegC -= _RegB;
				}
				else if (d2 == "C") {
					_RegC -= _RegC;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegC -= std::stoi(d2);
				}
			}
		}

		// DV nazwa_rejestru liczba/nazwa_rejestru
		else if (operation == ("DV")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			if (d1 == "A") {
				if (d2 == "A") {
					_RegA /= _RegA;
				}
				else if (d2 == "B") {
					_RegA /= _RegB;
				}
				else if (d2 == "C") {
					_RegA /= _RegC;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegA /= std::stoi(d2);
				}
			}
			else if (d1 == "B") {
				if (d2 == "A") {
					_RegB /= _RegA;
				}
				else if (d2 == "B") {
					_RegB /= _RegB;
				}
				else if (d2 == "C") {
					_RegB /= _RegC;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else
						_RegB /= std::stoi(d2);
				}
			}
			else if (d1 == "C") {
				if (d2 == "A") {
					_RegC /= _RegA;
				}
				else if (d2 == "B") {
					_RegC /= _RegB;
				}
				else if (d2 == "C") {
					_RegC /= _RegC;
				}
				else {
					if (!isNum(d2)) {
						_done = false;
					}
					else {
						if (std::stoi(d2) == 0) {
							std::cout << "ERROR! Nie mozna dzielic przez 0\n";
						}
						else _RegC /= std::stoi(d2);
					}
				}
			}
		}

		// IC nazwa_rejestru
		else if (operation == ("IC")) {
			setInstruction(RAM, 1, command);
			std::string reg = instruction[1];
			if (reg == "A") {
				_RegA++;
			}
			else if (reg == "B") {
				_RegA++;
			}
			else if (reg == "C") {
				_RegA++;
			}
		}
		// DC nazwa_rejestru
		else if (operation == ("DC")) {
			setInstruction(RAM, 1, command);
			std::string reg = instruction[1];
			if (reg == "A") {
				_RegA--;
			}
			else if (reg == "B") {
				_RegB--;
			}
			else if (reg == "C") {
				_RegC--;
			}
		}

		/* Operacje wykonywane na dysku */
		// OF nazwa_pliku
		else if (operation == ("OF")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
			if (d1 == ("")) {
				std::cout << "Blad w parametrach" << std::endl;
			}
			else {
				if (!dysk.open_file(d1, false)) {
					_done = false;
				}
			}
		}
		// ZF nazwa_pliku
		else if (operation == ("ZF")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
			if (d1 == ("")) {
				std::cout << "Blad w parametrach" << std::endl;
			}
			else {
				dysk.close_file(d1, false);
			}
		}
		// CF nazwa_pliku
		else if (operation == ("CF")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
			if (d1 == ("")) {
				std::cout << "Blad w parametrach" << std::endl;
			}
			else {
				dysk.create_file(d1);
			}
		}
		// WF nazwa_pliku dane
		else if (operation == ("WF")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			dysk.write_file(d1, d2);
		}
		// PF nazwa_pliku
		else if (operation == ("PF")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
			dysk.print_file(d1);
		}
		// DF nazwa_pliku
		else if (operation == ("DF")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
			dysk.delete_file(d1);
		}
		// RF nazwa_pliku_stara nazwa_pliku_nowa
		else if (operation == ("RF")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			dysk.rename_file(d1, d2);
		}
		// AF nazwa_pliku dane
		else if (operation == ("AF")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			if (d2 == "[A]") {
				d2 = std::to_string(_RegA);
			}
			else if (d2 == "[B]") {
				d2 = std::to_string(_RegB);
			}
			else if (d2 == "[C]") {
				d2 = std::to_string(_RegC);
			}
			dysk.add_to_file(d1, d2);
		}
		// LF
		else if (operation == ("LF")) {
			setInstruction(RAM, 0, command);
			dysk.print_file_list();
		}
		// PD
		else if (operation == ("PD")) {
			setInstruction(RAM, 0, command);
			dysk.printDisc();
		}




		/* Operacje wykonywane na komunikatach */
		// SC PID komunikat, gdzie ID_procesu to do kogo
		else if (operation == ("SC")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			std::shared_ptr<Komunikat> kom = std::make_shared<Komunikat>(running->GetID(), d2);
				if (!(running->kolejka.send(d1, kom))) {
					_done = false;
				}
		}
		// RC PID
		else if (operation == ("RC")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
				if (!running->kolejka.receive(d1)) {
					_done = false;
				}
		}
		// PC
		else if (operation == ("PC")) {
			setInstruction(RAM, 0, command);
			running->kolejka.wyswietl();
		}

		/* Operacje wykonywane na procesach */
		// CP nazwa_procesu grupa_procesu
		else if (operation == ("CP")) {
			setInstruction(RAM, 3, command);
			//create a new process
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			std::string d3 = instruction[3];
			//running->SetState(State::OCZEKUJACY);
			/*std::shared_ptr<PCB> process = NewProcess(d1, std::stoi(d2));
			process->SetFileName(d3);
			RAM.loadProcess(process->GetID(), d3);*/
			//scheduler.Schedule();
			if (!isNum(d2) || d1 == ("") || d2 == ("") || d3 == ("")) {
				std::cout << "Blad w parametrach" << std::endl;
			}
			else {
				try {
					if (!std::ifstream(d3)) {
						std::cout << "Brak pliku" << std::endl;
					}
					else {
						if (d2 == "0") {
							std::cout << "Nie mozna utworzyc procesu z grupa 0" << std::endl;
						}
						else {
							std::shared_ptr<PCB> process = NewProcess(d1, std::stoi(d2));
							if (process != NULL) {
								process->SetFileName(d3);
								RAM.loadProcess(process->GetID(), d3);
							}
							else {
								std::cout << "Nie mozna utworzyc procesu" << std::endl;
							}
						}
					}
				}
				catch (std::exception exception) {
					std::cout << "Zla forma parametru" << std::endl;
					_done = false;
				}
			}
		}
		// komenda zrobiona przez dobrego ziomka z zarzadzania procesami 
		else if (operation == ("CG")) {
			setInstruction(RAM, 2, command);
			//create a new process
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			//running->SetState(State::OCZEKUJACY);
			/*std::shared_ptr<PCB> process = NewProcess(d1, std::stoi(d2));
			process->SetFileName(d3);
			RAM.loadProcess(process->GetID(), d3);*/
			//scheduler.Schedule();
			if (d1 == ("") || d2 == ("")) {
				std::cout << "Blad w parametrach" << std::endl;
			}
			else {
				try {
					if (!std::ifstream(d2)) {
						std::cout << "Brak pliku" << std::endl;
					}
					else {
						std::shared_ptr<PCB> process = NewProcessGroupProcess(d1);
						if (process != NULL) {
							process->SetFileName(d2);
							RAM.loadProcess(process->GetID(), d2);;
						}
						else {
							std::cout << "Nie mozna utworzyc procesu" << std::endl;
						}
					}
				}
				catch (std::exception exception) {
					std::cout << "Zla forma parametru" << std::endl;
					_done = false;
				}
			}
		}
		// DP PID
		else if (operation == ("DP")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
			/*if (!isNum(d1)) {
				_done = false;
			}
			else {
				DeleteProcess(std::stoi(d1));
			}*/
			// dodaæ Macieja tutaj! Potrzebna metoda, która po podaniu nazwy procesu, zwraca nam jej PID :P
			DeleteProcessName(d1);
			scheduler.usun(d1);
			if (d1 == running->GetName()) {
				RAM.deleteProcess(running->GetID());
				scheduler.killprocess();
			}
		}

		// Aktywny proces: AP
		else if (operation == ("AP")) {
			setInstruction(RAM, 0, command);
			scheduler.print_running();
		}
		// Gotowe procesy: RP
		else if (operation == ("RP")) {
			setInstruction(RAM, 0, command);
			scheduler.wyswietl_gotowe();
		}
		// Ustaw wykonywany proces na stan oczekiwania: WP
		else if (operation == ("WP")) {
			setInstruction(RAM, 0, command);
			running->SetState(State::OCZEKUJACY);
		}
		else if (operation == ("NP")) {
			setInstruction(RAM, 0, command);
			std::cout << "Nazwa aktualnego wykonywanego procesu: " << running->GetName() << "\nPID: " << running->GetID() << std::endl;
		}

		/* Operacje wykonywane na pamieci RAM */
		// SR
		else if (operation == ("SR")) {
			setInstruction(RAM, 0, command);
			RAM.show();
		}
		// SF
		else if (operation == ("SF")) {
			setInstruction(RAM, 0, command);
			RAM.showFIFO();
		}
		// WR nazwa_rejestru(adres tam zapisany) dane_do_zapisu
		else if (operation == ("WR")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			if (d1 == "A") {
				RAM.writeToMemory(_RegA, d2);
			}
			else if (d1 == "B") {
				RAM.writeToMemory(_RegB, d2);
			}
			else if (d1 == "C") {
				RAM.writeToMemory(_RegC, d2);
			}
		}
		// RR skad dokad
		else if (operation == ("RR")) {
			setInstruction(RAM, 2, command);
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			std::string param;
			if (d1 == "A") {
				param = RAM.readFromMemory(_RegA);
			}
			else if (d1 == "B") {
				param = RAM.readFromMemory(_RegB);
			}
			else if (d1 == "C") {
				param = RAM.readFromMemory(_RegC);
			}

			if (d2 == "A") {
				_RegA = std::stoi(param);
			}
			else if (d2 == "B") {
				_RegB = std::stoi(param);
			}
			else if (d2 == "C") {
				_RegC = std::stoi(param);
			}
		}
		// GA gdzie
		else if (operation == ("GA")) {
			setInstruction(RAM, 1, command);
			std::string d1 = instruction[1];
			if (d1 == "A") {
				_RegA = RAM.getAddress();
			}
			else if (d1 == "B") {
				_RegB = RAM.getAddress();
			}
			else if (d1 == "C") {
				_RegC = RAM.getAddress();
			}
		}

		// operator zamkniecia procesu
		else if (operation == ("EX")) {
			setInstruction(RAM, 0, command);
			// killujemy proces
		/*	DeleteProcess(_PID);
			running->SetState(State::ZAKONCZONY);*/
			RAM.deleteProcess(running->GetID());
			scheduler.killprocess();
			eraseReg();
		}
		else if (operation == ("DN")) {
			// Do nothing! Wiêc nic nie robimy :) Ma³y psikus dla procesu!
		}
		// Blad interpretacji
		else {
			std::cout << "Brak takiej instrukcji" << std::endl;
			_done = false;
		}
		if (_done && command == ("runp")) {
			_IP++;
			saveRegisters();
		}
		else if (_done) {
			saveRegisters();
		}
	}
	else {
		std::cout << "dziala proces bezczynnosci\n";
	}
}

bool Interpreter::isNum(std::string param)
{
	bool good_param = true;
	for (int i = 0; i < param.length(); i++)
	{
		if (!isdigit(param[i]))
		{
			std::cout << "Blad w parametrach" << std::endl;
			good_param = false;
			break;
		}
	}
	return good_param;
}

bool Interpreter::forEQ(std::string param)
{
	bool good_param = true;
	for (int i = 0; i < param.length(); i++)
	{
		if (!isdigit(param[i]))
		{
			good_param = false;
			break;
		}
	}
	return good_param;
}


/*
ROZKAZY -> deasmbleracja
D1 - pierwsza dana
D2 - druga dana
D3 - trzecia dana

1. Operacja zamkniecia i bezczynnosci
	EX										=> zamyka proces, kill program
	DN										=> Do nothing, nic nie robi

2. Operacje logiczne
	MV nazwa_rejestru nazwa_rejestru/liczba => D1 <- D2
	EQ nazwa_rejestru nazwa_rejestru		=> true if d1 == d2, else false
	JP gdzie								=> jump D1
	JT gdzie								=> if true jump D1
	JF gdzie								=> if false jump D1

3. Operacje arytmetyczne 
	AD nazwa_rejestru liczba/nazwa_rejestru => D1 += D2
	ML nazwa_rejestru liczba/nazwa_rejestru => D1 *= D2
	SB nazwa_rejestru liczba/nazwa_rejestru => D1 -= D2
	DV nazwa_rejestru liczba/nazwa_rejestru => D1 /= D2
	IC nazwa_rejestru						=> D1++
	DC nazwa_rejestru						=> D1--

4. Operacje wykonywane na dysku
	OF nazwa_pliku							=> open file
	ZF nazwa_pliku							=> close file
	CF nazwa_pliku							=> create file
	WF nazwa_pliku dane						=> write to file
	PF nazwa_pliku							=> Print File
	DF nazwa_pliku							=> Delete File
	RF nazwa_pliku_stara nazwa_pliku_nowa	=> Rename File
	AF nazwa_pliku dane						=> Add to File. Dopisuje do pliku
	LF										=> list file, wyœwietla pliki na dysku
	PD										=> print disk, wyœwietla cala zawartosc dysku

5. Operacje wykonywane na komunikatach
	SC nazwa_procesu						=> Send communicate
	RC nazwa_procesu						=> Receive communicate
	PC										=> Print communicate
	

6. Operacje wykonywane na procesach
	CP nazwa_procesu numer_grupy plik.txt	=> Create a new process
	CG nazwa_procesu                        => Create a group with process
	DP nazwa_procesu						=> Delete a process
	AP										=> Print active process
	RP										=> print ready process
	WP										=> waiting process
	NP										=> print info about running->process

7. Operacje wykonywane na pamieci RAM
	SR										=> Show RAM
	SF										=> Show FIFO
	WR logic_adr data						=> write to RAM
	GA gdzie								=> get adress from RAM, write this adress to D1, D1 = A | B | C
	RR skad dokad							=> read from RAM
*/