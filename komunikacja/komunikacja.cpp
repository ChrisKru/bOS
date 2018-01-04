
#include "komunikacja.h"
#include "../ProcessM/ProcessM.h"
#include "../procesor/Scheduler.h"

//--------metody dla komuniaktu

Komunikat::Komunikat()
{

}
Komunikat::Komunikat(int nadawca, std::string tresc)
{
	id_nadawcy = nadawca;
	tresc_komunikatu = tresc;
	rozmiar_komunikatu = tresc_komunikatu.size();
}

int Komunikat::getID()
{
	return id_nadawcy;
}

int Komunikat::getRozmiar()
{
	return rozmiar_komunikatu;
}

std::string Komunikat::getTresc()
{
	return tresc_komunikatu;
}

//---------metody dla kolejki
Kolejka::Kolejka()
{

}
void Kolejka::dodaj_komunikat(std::shared_ptr<Komunikat> komunikat)
{
	kolejka.push_back(komunikat);

}
void Kolejka::usun_komunikat()
{
	if (kolejka.size() == 0)
	{
		std::cout << "Kolejka komunikatow dla tego procesu jest pusta" << std::endl;
	}
	else
		kolejka.pop_front();
}
bool Kolejka::receive(std::string nazwa_nadawcy)
{
	bool flagR = false;
	bool czy_istnieje = false;
	bool rozne_grupy = true;
	int id_nadawcy;
	if (nazwa_nadawcy == running->ProcessName)
	{
		std::cout << "Proba odebrania komunikatu od samego siebie, zdefiniuj innego nadawce" << std::endl;
		flagR = true;
		return flagR;
	}
	for (auto &v : ProcessGroupsList) 
	{
		for (auto &x : v.ProcessList) 
		{
			if (x->ProcessName == nazwa_nadawcy)	//sprawdzenie, czy nadawca nie zakoñczy³ do tej pory dzia³ania(tzn czy jest na liœcie procesów)
			{
				czy_istnieje = true;
				id_nadawcy = x->GetID();

			}
		}
	}
	if (czy_istnieje == true)
	{
		std::shared_ptr<PCB> nadawca = GetPCB(id_nadawcy);
		std::cout << "Rozmiar kolejki: " << kolejka.size() << std::endl;
		int grupa_odbiorcy = running->ProcessGroup;
		int grupa_nadawcy = nadawca->ProcessGroup;
		for (auto &it : ProcessGroupsList)
		{
			if (it.ProcessGroup == grupa_odbiorcy && it.ProcessGroup == grupa_nadawcy)
				rozne_grupy = false;
		}
		if (kolejka.size() > 0 && rozne_grupy==false)
		{
					for (auto &it : kolejka)
					{
						if (it->id_nadawcy == id_nadawcy)
						{
							if (nadawca->GetState() == State::OCZEKUJACY && kolejka.size() == 8)	//budzenie nadawcy, jeœli jest uœpiony i kolejka odbiorcy by³a pe³na
							{
								nadawca->SetState(State::GOTOWY);
								dodaj_do_procesow_gotowych(nadawca);
							}
							std::shared_ptr<Komunikat> odebrany = kolejka.front();
							flagR = true;
							usun_komunikat();	//po odczytaniu komunikatu z kolejki, musi zostaæ z niej usuniêty
							std::cout << "Tresc odebranego komunikatu: " << odebrany->tresc_komunikatu << std::endl;
							return flagR;
						}
					}

		}
		else if (kolejka.size() == 0 && rozne_grupy == false)	//usypianie odbiorcy, gdy jego kolejka jest pusta
		{
			std::cout << "Kolejka komunikatow dla tego procesu jest pusta" << std::endl;
			running->SetState(State::OCZEKUJACY);
			return flagR;
		}
	}
	else if (czy_istnieje == false)
	{
		std::cout << "Proces nadawcy nie istnieje, nie mozna odebrac komunikatu" << std::endl;
		flagR = true;
		return flagR;
	}
	if (rozne_grupy == true)
	{
		std::cout << "Procesy naleza do roznych grupy, nie udalo sie odebrac komunikatu" << std::endl;
		flagR = true;
		return flagR;
	}
	return flagR;
}
void Kolejka::wyswietl()
{
	if (kolejka.size() == 0)
		std::cout << "Kolejka komunikatow dla tego procesu jest pusta" << std::endl;
	else
	{
		std::cout << "Kolejka zawiera nastepujace komunikaty: " << std::endl;
		for (auto e : kolejka)
		{
			std::cout << e->getTresc() << std::endl;
		}
	}
}
bool Kolejka::send(std::string nazwa_odbiorcy, std::shared_ptr<Komunikat> komunikat)
{
	bool flagS = false;
	bool czy_pelna = false;
	bool czy_istnieje = false;
	bool rozne_grupy = true;
	int id_odbiorcy;
	if (nazwa_odbiorcy == running->ProcessName)
	{
		std::cout << "Proba wyslania komunikatu do samego siebie, zdefiniuj innego odbiorce" << std::endl;
		flagS = true;
		return flagS;
	}
	for (auto &v : ProcessGroupsList)
	{
		for (auto &x : v.ProcessList)
		{
			if (x->ProcessName == nazwa_odbiorcy)	//sprawdzenie, czy nadawca nie zakoñczy³ do tej pory dzia³ania
			{
				czy_istnieje = true;
				id_odbiorcy = x->GetID();
			}
		}
	}
	if (czy_istnieje == true)
	{
		std::shared_ptr<PCB> odbiorca = GetPCB(id_odbiorcy);
		std::shared_ptr<PCB> nadawca = GetPCB(komunikat->id_nadawcy);
		int grupa_odbiorcy = odbiorca->ProcessGroup;
		int grupa_nadawcy = nadawca->ProcessGroup;
		for (auto &it : ProcessGroupsList)
		{
			if (it.ProcessGroup == grupa_odbiorcy && it.ProcessGroup == grupa_nadawcy)
				rozne_grupy = false;
		}
		if (odbiorca->kolejka.kolejka.size() >= 8 && rozne_grupy==false)	//usypianie nadawcy, gdy kolejka odbiorcy jest pe³na
		{
			nadawca->SetState(State::OCZEKUJACY);
			czy_pelna = true;
		}
		if (czy_pelna == false && rozne_grupy == false)
		{
			if (odbiorca->GetState() == State::OCZEKUJACY && odbiorca->kolejka.kolejka.size() == 0)	//budzenie odbiorcy, jeœli jest uœpiony i jego kolejka jest pusta
				{
					odbiorca->SetState(State::GOTOWY);
					dodaj_do_procesow_gotowych(odbiorca);
				}
				for (auto &it : ProcessGroupsList)
				{
					for (auto &e : it.ProcessList)
					{
						if (e->ProcessID == id_odbiorcy)
						{
							e->kolejka.dodaj_komunikat(komunikat);
							flagS = true;
						}

					}
				}
		}
		if (flagS == false && rozne_grupy==false)
		{
			std::cout << "Nie wyslano komunikatu" << std::endl;
		}
		else if (flagS == false && rozne_grupy == true)
		{
			std::cout << "Procesy naleza do roznych grup, nie udalo sie wyslac komunikatu" << std::endl;
			flagS = true;
		}
	}
	else if (czy_istnieje == false)
	{
		std::cout << "Proces odbiorcy nie istnieje, nie mozna wyslac komunikatu" << std::endl;
		flagS = true;
	}
	return flagS;
}
