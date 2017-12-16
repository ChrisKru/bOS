#pragma once
#include <fstream>
#include <string>

#include "../Memory/PageTable.h"

// struktura daj¹ca informacje o danych fragmentach pliku wymiany
struct OneProcess {
	int page;		// która to stronica procesu
	int PID;		// który to proces
};

// plik wymiany podzielony na struktury, maj¹ce w sobie:
struct tab {
	int PID;		// informacjê o ID procesu
	char data[16];	// dane
};


class ExchangeFile {
private:
	std::vector<OneProcess> container;		// w wektorze zapisywane informacje o tym, jakie procesy
											// s¹ w pliku wymiany
	std::vector<tab> file;					// plik wymiany jako vector stronnic po 16 znaków

public:
	ExchangeFile();
		// konstruktor domyœlny

	PageTable DataToFile(int& PID, std::string& fileName, int& size);
		// metoda przechwytuje plik txt, tworzy dla niego tablice stronic i tym samym 
		// zapisuje jego zawartosc do pliku wymiany

	tab getData(int ProcID, int pageNumber);
		// zwraca tê stronicê, która musi wejœæ do pamiêci

	int getFirstIndex(int PID, int commandCounter);
		// zwraca pocz¹tek komendy
		// metoda potrzebna w getCommand

	int getLastIndex(int PID, int commandCounter);
		// analogicznie do getFirstIndex
	
	void show();
		// wyœwietla zawartoœæ pliku wymiany

};