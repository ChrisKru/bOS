#ifndef komunikacja_h
#define komunikacja_h

#include <deque>
#include "../ProcessM/ProcessM.h"

class Komunikat
{
public:
	//--------pola
	std::string nazwa_nadawcy;
	int rozmiar_komunikatu;
	std::string tresc_komunikatu;
	//--------metody
	Komunikat::Komunikat();
	Komunikat::Komunikat(std::string nadawca, std::string tresc);
	std::string Komunikat::getName();
	int Komunikat::getRozmair();
	std::string Komunikat::getTresc();
};

class Kolejka
{
public:
	//--------pola
	static std::deque<Komunikat> kolejka;
	Komunikat* wskaznik;	//mo¿e niech ka¿dy proces dysponuje takim wskaŸnikiem, którego bêdzie u¿ywa³ przy metodach zwracaj¹cych wskaŸniki, np przy odbierz
	Komunikat niewskaznik;
	//--------metody
	Kolejka::Kolejka();
	void Kolejka::dodaj_komunikat(Komunikat komunikat);
	void Kolejka::usun_komunikat();
	void Kolejka::send(std::string nazwa_odbiorcy, Komunikat komunikat);
	Komunikat Kolejka::receive(std::string nazwa_nadawcy);
	void Kolejka::wyswietl();
};
#endif
