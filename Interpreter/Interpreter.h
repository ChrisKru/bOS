#pragma once
#include "Scheduler.h"
#include "dysk.h"
#include "Memory.h"
#include <string>
#include <array>

/*
Do EQ mo¿na dorobiæ flagê
w runInstruction() zwracamy false, gdy nie wykonamy programu, czyli wystpia jakies bledy
*/

class Interpreter
{
public:
	int _PID;
	int _RegA;
	int _RegB;
	int _RegC;
	int _IP;
	bool _done;
	bool _flagEQ;

private:
	//std::string instruction;	
	//std::string operation;		// w niej znajduje siê operacja do wykonania
	std::array<std::string, 3> instruction;
	Disc dysk;
	Memory RAM;
	Scheduler scheduler;

public:
	Interpreter() {
		_PID = 0;
		_RegA = 0;
		_RegB = 0;
		_RegC = 0;
		_IP = 0;
	}

	void loadRegister() { // Running to wskaŸnik na PCB
		_PID = running->ProcessID;
		_RegA = running->A;
		_RegB = running->B;
		_RegC = running->C;
		_IP = running->CommandCounter;
	}

	std::string loadInstruction() {
		// wywo³anie pamiêci operacyjnej
		// zwraca nam rozkaz jaki ma zostaæ wykonany
		// return STRING Z ROZKAZEM
	}

	void saveRegisters() {
		running->SetCommandCounter(_IP);
		running->A = _RegA;
		running->B = _RegB;
		running->C = _RegC;
		running->Timmer++;
	}

	void showRegisters() {
		std::cout << std::endl << "Stan reejestrow dla procesu: " << _PID << std::endl;
		std::cout << "A: " << _RegA << std::endl;
		std::cout << "B: " << _RegA << std::endl;
		std::cout << "C: " << _RegA << std::endl;
		std::cout << "Licznik rozkazow: " << _IP << std::endl;
	}



	bool runInstruction() {
		// wczytujemy rozkaz do wykonania
		// instruction = loadInstruction();
		/*
			Pracê rozdzielamy sobie na tablicê Stringów
			rozkazy bêd¹ do niej zapisywane, maks rozmiar = 3.
		*/


		// operation = instruction.substr(0,2);
		std::string operation = instruction[0];


		// operator zamkniêcia procesu
		if (operation.compare("EX")) {
			// killujemy proces
			showRegisters();
			_IP += operation.length();

		}

		/* Operacje logiczne */
		// MV nazwa_rejestru liczba
		if (operation.compare("MV")) {
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
				else {
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
					_RegC = std::stoi(d2);
				}
			}
		}

		// EQ nazwa_rejestru nazwa_rejestru
		else if (operation.compare("EQ")) {
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			if (d1 == d2 || d1 == "A" && d2 == "B" && _RegA == _RegB || 
				d1 == "A" && d2 == "C" && _RegA == _RegC ||
				d1 == "B" && d2 == "A" && _RegB == _RegA ||
				d1 == "B" && d2 == "C" && _RegB == _RegC ||
				d1 == "C" && d2 == "A" && _RegC == _RegA ||
				d1 == "C" && d2 == "B" && _RegC == _RegB) {
				std::cout << "Wartosci sa sobie rowne!" << std::endl;
				_flagEQ = true;
			}
			else {
				_flagEQ = false;
				std::cout << "Wartosci sa rozne!" << std::endl;
			}
		}
		// JP gdzie
		else if (operation.compare("JP")) {
			std::string d1 = instruction[1];
			_IP = std::stoi(d1);
		}
		// JT gdzie(jeœli prawda)
		else if (operation.compare("JT")) {
			std::string d1 = instruction[1];
			if (_flagEQ == true) _IP = std::stoi(d1);
		}
		// JF gdzie(jeœli fa³sz)
		else if (operation.compare("JF")) {
			std::string d1 = instruction[1];
			if (_flagEQ == false) _IP = std::stoi(d1);
		}

		/* Operacje arytmetyczne */
		// AD nazwa_rejestru liczba/nazwa_rejestru
		else if (operation.compare("AD")) {
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
				else {
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
				else {
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
				else {
					_RegC += std::stoi(d2);
				}
			}
		}

		// ML nazwa_rejestru liczba/nazwa_rejestru
		else if (operation.compare("ML")) {
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
					_RegC *= std::stoi(d2);
				}
			}
		}

		// SB nazwa_rejestru liczba/nazwa_rejestru
		else if (operation.compare("SB")) {
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
					_RegC -= std::stoi(d2);
				}
			}
		}

		// DV nazwa_rejestru liczba/nazwa_rejestru
		else if (operation.compare("DV")) {
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
					_RegC /= std::stoi(d2);
				}
			}
		}

		// IC nazwa_rejestru
		else if (operation.compare("IC")) {
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
		else if (operation.compare("DC")) {
			std::string reg = instruction[1];
			if (reg == "A") {
				_RegA--;
			}
			else if (reg == "B") {
				_RegA--;
			}
			else if (reg == "C") {
				_RegA--;
			}
		}

		/* Operacje wykonywane na dysku */
		// CF nazwa_pliku
		else if (operation.compare("CF")) {
			std::string d1 = instruction[1];
			dysk.create_file(d1);
		}
		// WF nazwa_pliku dane
		else if (operation.compare("WF")) {
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			dysk.write_file(d1, d2);
		}
		// PF nazwa_pliku
		else if (operation.compare("PF")) {
			std::string d1 = instruction[1];
			dysk.print_file(d1);
		}
		// DF nazwa_pliku
		else if (operation.compare("DF")) {
			std::string d1 = instruction[1];
			dysk.delete_file(d1);
		}
		// RF nazwa_pliku_stara nazwa_pliku_nowa
		else if (operation.compare("RF")) {
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			dysk.rename_file(d1, d2);
		}
		// AF nazwa_pliku dane
		else if (operation.compare("AF")) {
			std::string d1 = instruction[1];
			std::string d2 = instruction[2];
			dysk.add_to_file(d1, d2);
		}
		// LF
		else if (operation.compare("LF")) {
			dysk.print_file_list();
		}
		// PD
		else if (operation.compare("PD")) {
			dysk.printDisc();
		}

		/* Operacje wykonywane na procesach */
		// CP nazwa_procesu


		/* Operacje wykonywane na komunikatach */
		// CP nazwa_procesu
		else if (operation.compare("CP")) {
			P
		}

		// Aktywne procesy: AP
		else if (operation.compare("AP")) {
			//scheduler
		}
		// Oczekuj¹ce proces: WP
		else if (operation.compare("WP")) {
			//scheduler
		}
		/* Operacje wykonywane na pamiêci RAM */
		// SR
		else if (operation.compare("SR")) {
			RAM.show();
		}
		// SF
		else if (operation.compare("SF")) {
			RAM.showFIFO(); 
		}
	}

};

