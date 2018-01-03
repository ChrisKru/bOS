#pragma once
#include <iostream>
#include <deque>
#include <memory>

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
	int getID();
	int getRozmiar();
	std::string getTresc();
};

class Kolejka
{
public:
	//--------pola
	std::deque<std::shared_ptr<Komunikat>> kolejka;
	std::shared_ptr<Komunikat> wskaznik;
	//--------metody
	Kolejka();
	void dodaj_komunikat(std::shared_ptr<Komunikat> komunikat);
	void usun_komunikat();
	bool send(std::string odbiorca, std::shared_ptr<Komunikat> komunikat);
	bool receive(std::string nadawca);
	void wyswietl();
};
