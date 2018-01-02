#pragma once
#include <vector>
#include <iostream>

class PageTable {
private:
	int ID;							// ID przechwyconego procesu do tablicy stronic
	int pages;						// ilo�� stronic zale�na od rozmiaru procesu
public:
	std::vector<int> FrameNumber;	// tablica, zawieraj�ca w sobie numer ramki dla ka�dej ze stronic
	std::vector<bool> VIBit;		// bit, m�wi�cy czy dana stronica jest w pami�ci

	PageTable(int PID, int ProcessSize);
		// konstruktor przechwytuje ID procesu, tworzy dla niego tablic� stronic,
		// oblicza ile potrzeba stronic dla procesu, ustawia domy�lne warto�ci
		// tablicom FrameNumber oraz VIBit

	int getPageNumber(int LogicalAddress);
		// zwraca stronic�, w kt�rej powinien znajdowa� si� wskazany adres

	int getPages();
		// zwraca ilo�� stronic

	int getID();
		// zwraca ID procesu, do kt�rego nale�y tablica stronic

	void show();
		// wyswietla zawartosc tablicy stronic
};