#ifndef kolejka_h
#define kolejka_h

#include "komunikat.h"
#include <deque>

class Kolejka
{
public:
	std::deque<Komunikat> kolejka;
	Komunikat* wskaznik;	//mo¿e niech ka¿dy proces dysponuje takim wskaŸnikiem, którego bêdzie u¿ywa³ przy metodach zwracaj¹cych wskaŸniki, np przy odbierz
	Komunikat niewskaznik;
	//zrobiê bez konstruktora na razie i zobaczymy, co wyjdzie EDIT: nie no musi byæ
	Kolejka();	//domyœlny konstruktor?
	void dodaj_komunikat(Komunikat komunikat);
	void usun_komunikat();
	Komunikat odbierz_komunikat();
	//void wyslij_komunikat(int id_odbiorcy, Komunikat komunikat);
	void wyswietl();	//wyswieltanie zawartoœci kolejki
};



#endif
