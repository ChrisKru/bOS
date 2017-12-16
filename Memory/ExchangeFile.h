#pragma once
#include <fstream>
#include <string>

#include "PageTable.h"

// struktura daj�ca informacje o danych fragmentach pliku wymiany
struct OneProcess {
	int page;		// kt�ra to stronica procesu
	int PID;		// kt�ry to proces
};

// plik wymiany podzielony na struktury, maj�ce w sobie:
struct tab {
	int PID;		// informacj� o ID procesu
	char data[16];	// dane
};


class ExchangeFile {
private:
	std::vector<OneProcess> container;		// w wektorze zapisywane informacje o tym, jakie procesy
											// s� w pliku wymiany
	std::vector<tab> file;					// plik wymiany jako vector stronnic po 16 znak�w

public:
	ExchangeFile() {
		// konstruktor domy�lny
	}

	PageTable DataToFile(int& PID, std::string& fileName, int& size) {
		// metoda przechwytuje plik txt, tworzy dla niego tablice stronic i tym samym 
		// zapisuje jego zawartosc do pliku wymiany
		int pages;
		std::string line, allfile;
		std::fstream txtfile;
		txtfile.open(fileName);
		if (txtfile) {		
			// wpisanie warto�ci pliku txt do tymczasowego stringa
			while (!txtfile.eof()) {
				std::getline(txtfile, line);
				allfile += line + ';';
			}
		}
		txtfile.close();

		// utworzenie tablicy stronnic dla procesu
		PageTable pgtable(PID, size);
		pages = pgtable.getPages();

		// teraz tymczasow� tablic� podzielimy na tablice po 16 znak�w i
		// wpiszemy je do pliku wymiany
		int fromfile = 0;
		for (int i = 0;i < pages;i++) {
			
			tab table;

			// for zapisuje pojedyncze znaki do pomocniczego chara
			for (int k = 0;k < 16;k++) {
				if (fromfile < allfile.size()) {
					table.data[k] = allfile[fromfile++];
					//fromfile++;
				}
				else {
					table.data[k] = ' ';
				}
			}
			//std::cout << table.data << std::endl; //<-- sprawdza�em czy dobrze zapisuje

			table.PID = PID;
			file.push_back(table);
			file;
			// dodanie informacji o wrzucanych danych
			OneProcess thisproc;
			thisproc.page = i;
			thisproc.PID = PID;
			container.push_back(thisproc);
		}

		return pgtable;
		}


	tab getData(int ProcID, int pageNumber) {
		// zwraca t� stronic�, kt�ra musi wej�� do pami�ci

		tab returnthis;
		for (int i = 0;i < container.size();i++) {
			if ((container[i].PID == ProcID) && (container[i].page == pageNumber)) {
				for (int j = 0;j < 16;j++) {
					returnthis.data[j] = file[i].data[j];
				}
				return returnthis;
			}
		}
	}

	int getFirstIndex(int PID, int commandCounter) {
		// zwraca pocz�tek komendy
		// metoda potrzebna w getCommand
		
		if (commandCounter == 0) return 0;

		int separators = 0;
		for (int i = 0;i < file.size();i++) {
			if (file[i].PID == PID) {
				for (int j = 0;j < 16;j++) {
					if (file[i].data[j] == ';') {
						separators++;
						if (separators == commandCounter) { return (container[i].page * 16 + j + 1); break; }
					}
				}
			}
		}
	}

	int getLastIndex(int PID, int commandCounter) {
		// analogicznie do getFirstIndex

		int separators = 0;
		for (int i = 0;i < file.size();i++) {
			if (file[i].PID == PID) {
				for (int j = 0;j < 16;j++) {
					if (file[i].data[j] == ';') {
						separators++;
						if (separators == commandCounter+1) { return (container[i].page * 16 + j - 1); break; }
					}
				}
			}
		}
	}
	
	void show() {
		// wy�wietla zawarto�� pliku wymiany

		std::cout << "PLIK WYMIANY" << std::endl;
		for (int i = 0;i < file.size();i++) {
			std::cout << file[i].data;
		}
	}

};