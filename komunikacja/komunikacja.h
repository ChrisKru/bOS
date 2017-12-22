#ifndef komunikacja_h
#define komunikacja_h

#include <iostream>
#include <deque>
#include "../ProcessM/ProcessM.h"

class Komunikat
{
public:
	//--------pola
	int id_nadawcy;
	int rozmiar_komunikatu;
	std::string tresc_komunikatu;
	//--------metody
	Komunikat();
	Komunikat(int nadawca, std::string tresc);
	//int Komunikat::getId();
	//int Komunikat::getRozmair();
	std::string getTresc();
};

class Kolejka
{
public:
	//--------pola
	std::deque<Komunikat> kolejka;
	Komunikat* wskaznik;	//mo¿e niech ka¿dy proces dysponuje takim wskaŸnikiem, którego bêdzie u¿ywa³ przy metodach zwracaj¹cych wskaŸniki, np przy odbierz
	Komunikat niewskaznik;
	//--------metody
	Kolejka();
	void dodaj_komunikat(Komunikat komunikat);
	void usun_komunikat();
	void send(int id_odbiorcy, Komunikat komunikat);
	Komunikat receive(int id_nadawcy);
	void wyswietl();
};
#endif
