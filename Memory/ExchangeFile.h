#pragma once
#include <fstream>
#include <string>

#include "PageTable.h"

struct PgTable {
	int PID;
	int firstIndex;
	int lastIndex;
};

class ExchangeFile {
public:
	char file[4000];				// plik wymiany jako tablica char�w
	int firstFreeIndex = 0;			// pierwszy wolny index w pliku wymiany
	std::vector<PgTable> container;	// w wektorze zapisywane informacje o tym,
									// gdzie zaczyna si� i ko�czy dany proces

	ExchangeFile() {
		// konstruktor domy�lny
	}

	void DataInFile(int PID, std::string fileName) {
		// metoda przechwyca plik txt, tworzy dla niego tablice stronic i tym samym 
		// zapisuje jego zawartosc do pliku wymiany

		char odczyt[400];
		int size; int pages;
		std::fstream file;
		file.open(fileName);
		if (file) {
			// ustalanie wielkosci pliku - wielkosci procesu
			file.seekg(0, std::ios::end);
			size = file.tellg();
			file.seekg(0, std::ios::beg);
		}
		PageTable table(PID, size);
		//pages = table.getPages;
		
		// JAK ROZPOZNA� ZNAK NOWEJ LINI? MA BY� ZNAKIEM SPACJI CZY CHCECIE �EBY BY�O INACZEJ?


		// ... do dopracowania

		// konieczny pushback do container
		// zmienia first free index!
		}

	void deleteProcessData(int ProcID) {
		// usuwa zawarto�� procesu z pliku wymiany
		char str = ' ';
		for (int i = 0;i < container.size();i++) {
			if (container[i].PID == ProcID) {
				int first, last;
				first = container[i].firstIndex;
				last = container[i].lastIndex;
				for (int k = first;k < (last - first + 1);k++) {
					file[k] = str;
				}
			} break;
		}
	}

	
	std::string getData(int ProcID, int pageNumber) {
		// zwraca t� stronic� z pliku wymiany, kt�r� chcemy
		// wrzuci� do pami�ci w postaci tablicy char�w

		std::string data = "";
		for (int i = 0;i < container.size();i++) {
			if (container[i].PID == ProcID) {
				int first, last;
				first = container[i].firstIndex;
				last = container[i].lastIndex;
				int thisPagefirstIndex = 16 * pageNumber + first;
				for (int k = thisPagefirstIndex;k < 16;k++) {
					data = data + file[k];
				}
			} break;
		}
		return data;
	}

	void show() {
		// wy�wietla zawarto�� pliku wymiany

		std::cout << "PLIK WYMIANY" << std::endl;
		for (int i = 0;i < 4000;i++) {
			std::cout << file << std::endl;
		}
	
	}

};