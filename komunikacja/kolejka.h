#ifndef kolejka_h
#define kolejka_h

#include "komunikat.h"
#include <deque>

class Kolejka
{
public:
	std::deque<Komunikat> kolejka;
	Komunikat* wskaznik;	//mo�e niech ka�dy proces dysponuje takim wska�nikiem, kt�rego b�dzie u�ywa� przy metodach zwracaj�cych wska�niki, np przy odbierz
	Komunikat niewskaznik;
	//zrobi� bez konstruktora na razie i zobaczymy, co wyjdzie EDIT: nie no musi by�
	Kolejka();	//domy�lny konstruktor?
	void dodaj_komunikat(Komunikat komunikat);
	void usun_komunikat();
	Komunikat odbierz_komunikat();
	//void wyslij_komunikat(int id_odbiorcy, Komunikat komunikat);
	void wyswietl();	//wyswieltanie zawarto�ci kolejki
};



#endif
