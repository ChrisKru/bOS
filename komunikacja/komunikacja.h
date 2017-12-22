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
	Komunikat::Komunikat();
	Komunikat::Komunikat(int nadawca, std::string tresc);
	//int Komunikat::getId();
	//int Komunikat::getRozmair();
	std::string Komunikat::getTresc();
};

class Kolejka
{
public:
	//--------pola
	std::deque<Komunikat> kolejka;
	Komunikat* wskaznik;	//mo¿e niech ka¿dy proces dysponuje takim wskaŸnikiem, którego bêdzie u¿ywa³ przy metodach zwracaj¹cych wskaŸniki, np przy odbierz
	Komunikat niewskaznik;
	//--------metody
	Kolejka::Kolejka();
	void Kolejka::dodaj_komunikat(Komunikat komunikat);
	void Kolejka::usun_komunikat();
	void Kolejka::send(int id_odbiorcy, Komunikat komunikat);
	Komunikat Kolejka::receive(int id_nadawcy);
	void Kolejka::wyswietl();
};
#endif
