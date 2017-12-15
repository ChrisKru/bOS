#pragma once
#include <fstream>
#include <string>

#include "PageTable.h"

// struktura reprezentuj¹ca jeden proces
struct OneProcess {
	int page;
	int PID;						
};

struct tab {
	int PID;
	char data[16];
};


class ExchangeFile {
private:
	std::vector<OneProcess> container;		// w wektorze zapisywane informacje o tym, jakie procesy
											// s¹ w pliku wymiany
public:
	std::vector<tab> file;					// plik wymiany jako vector stronnic po 16 znaków
	
	ExchangeFile() {
		// konstruktor domyœlny
	}

	PageTable DataToFile(int& PID, std::string& fileName, int& size) {
		// metoda przechwytuje plik txt, tworzy dla niego tablice stronic i tym samym 
		// zapisuje jego zawartosc do pliku wymiany
		int pages;
		std::string line, allfile;
		std::fstream txtfile;
		txtfile.open(fileName);
		if (txtfile) {
			// ustalanie wielkosci pliku - wielkosci procesu
			//txtfile.seekg(0, std::ios::end);
			//size = txtfile.tellg();
			//txtfile.seekg(0, std::ios::beg);
			
			// wpisanie wartoœci pliku txt do tymczasowej tablicy
			while (!txtfile.eof()) {
				std::getline(txtfile, line);
				allfile += line;
			}
		}
		txtfile.close();

		// utworzenie tablicy stronnic dla procesu
		PageTable pgtable(PID, size);
		pages = pgtable.getPages();

		// teraz tymczasow¹ tablicê podzielimy na tablice po 16 znaków i
		// wpiszemy je do pliku wymiany
		int fromfile = 0;
		for (int i = 0;i < pages;i++) {
			
			char helpful[16];
			// for zapisuje pojedyncze znaki do pomocniczego chara
			for (int k = 0;k < 16;k++) {
				if (fromfile != allfile.size()) {
					helpful[k] = allfile[fromfile++];
					//fromfile++;
				}
				else break;
			}
			
			// wrzucenie stronicy po 16 znaków do pliku wymiany
			tab table;
			for (int j = 0;j < 16;j++) {
				table.data[j] = helpful[j];
			}
			table.PID = PID;
			file.push_back(table);
			
			// dodanie informacji o wrzucanych danych
			OneProcess thisproc;
			thisproc.page = i;
			thisproc.PID = PID;
			container.push_back(thisproc);
		}

		return pgtable;
		}


	tab getData(int ProcID, int pageNumber) {
		// zwraca tê stronicê, która musi wejœæ do pamiêci

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

/* raczej niepotrzebne gdy plik wymiany ma byæ vectorem
	void deleteProcessData(int ProcID) {
		// usuwa zawartoœæ procesu z pliku wymiany
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
	*/
	
	void show() {
		// wyœwietla zawartoœæ pliku wymiany

		std::cout << "PLIK WYMIANY" << std::endl;
		for (int i = 0;i < file.size();i++) {
			std::cout << file[i].data;
		}
	
	}

};