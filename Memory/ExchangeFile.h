#pragma once
#include <fstream>
#include <string>

#include "../Memory/PageTable.h"

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
	ExchangeFile();
		// konstruktor domy�lny

	PageTable DataToFile(int& PID, std::string& fileName, int& size);
		// metoda przechwytuje plik txt, tworzy dla niego tablice stronic i tym samym 
		// zapisuje jego zawartosc do pliku wymiany

	tab getData(int ProcID, int pageNumber);
		// zwraca t� stronic�, kt�ra musi wej�� do pami�ci

	int getFirstIndex(int PID, int commandCounter);
		// zwraca pocz�tek komendy
		// metoda potrzebna w getCommand

	int getLastIndex(int PID, int commandCounter);
		// analogicznie do getFirstIndex
	
	void show();
		// wy�wietla zawarto�� pliku wymiany

};