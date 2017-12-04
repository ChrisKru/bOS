#pragma once
#include <fstream>

#include "PageTable.h"

class ExchangeFile {
public:
	char file[4000];		// plik wymiany jako tablica charów
	int firstFreeIndex = 0;	// pierwszy wolny index w pliku wymiany

	ExchangeFile() {
		// konstruktor domyœlny
	}

	PageTable DataInFile(int PID, std::string fileName) {
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
		pages = table.getPages;

	}

	void deleteProcessData(int PID) {
		// metoda uruchamiana gdy proces koñczy swój ¿ywot i dane nale¿y usun¹æ
		// z pliku wymiany
	}

	
	std::string getFrame(int PID, int pageNumber) {
		// pobiera tylko te stronice, o ktora prosimy
	}

	/* probably niepotrzebne rzeczy
	void addPage(int PID, int pageNumber, string data) {
		// dodaje
	}*/

	void show() {
		// wyœwietla zawartoœæ pliku wymiany
	}

};