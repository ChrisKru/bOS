#pragma once
#include <vector>
#include <iostream>

class PageTable {
private:
	int ID;							// ID przechwyconego procesu do tablicy stronic
	int pages;						// iloœæ stronic zale¿na od rozmiaru procesu
public:
	std::vector<int> FrameNumber;	// tablica, zawieraj¹ca w sobie numer ramki dla ka¿dej ze stronic
	std::vector<bool> VIBit;		// bit, mówi¹cy czy dana stronica jest w pamiêci

	PageTable(int PID, int ProcessSize);
		// konstruktor przechwytuje ID procesu, tworzy dla niego tablicê stronic,
		// oblicza ile potrzeba stronic dla procesu, ustawia domyœlne wartoœci
		// tablicom FrameNumber oraz VIBit

	int getPageNumber(int LogicalAddress);
		// zwraca stronicê, w której powinien znajdowaæ siê wskazany adres

	int getPages();
		// zwraca iloœæ stronic

	int getID();
		// zwraca ID procesu, do którego nale¿y tablica stronic

	void show();
		// wyswietla zawartosc tablicy stronic
};