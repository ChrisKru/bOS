#pragma once
#include <iostream>
#include <deque>
#include <memory>

//extern bool obudz_nadawce = false, obudz_odbiorce = false;

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
	//int getId();
	//int getRozmair();
	std::string getTresc();
};

class Kolejka
{
public:
	//--------pola
	std::deque<std::shared_ptr<Komunikat>> kolejka;
	//Komunikat* wskaznik;	
	Komunikat niewskaznik;
	std::shared_ptr<Komunikat> wskaznik;
	//--------metody
	Kolejka();
	void dodaj_komunikat(std::shared_ptr<Komunikat> komunikat);
	void usun_komunikat();
	void send(int id_odbiorcy /*std::string odbiorca*/, std::shared_ptr<Komunikat> komunikat);
	std::shared_ptr<Komunikat> receive(int id_nadawcy /*std::string nadawca*/);
	void wyswietl();
};
