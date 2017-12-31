
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
/*
int Komunikat::getID()
{
	return id_nadawcy;
}

int Komunikat::getRozmair()
{
	return rozmiar_komunikatu;
}
*/
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
std::shared_ptr<Komunikat> Kolejka::receive(int id_nadawcy /*std::string nadawca*/)
{
	bool czy_istnieje = false;
	for (auto &v : ProcessGroupsList) 
	{
		for (auto &x : v.ProcessList) 
		{
			if (x->ProcessID == id_nadawcy)	//sprawdzenie, czy nadawca nie zakoñczy³ do tej pory dzia³ania
				czy_istnieje = true;
		}
	}
	if (czy_istnieje == true)
	{
		std::shared_ptr<PCB> nadawca = GetPCB(id_nadawcy);	//to bêdzie u¿ywane dalej, przy budzeniu
		std::cout << "Rozmiar kolejki: " << kolejka.size() << std::endl;
		if (kolejka.size() > 0)
		{
			for (auto &it : kolejka)
			{
				if (it->id_nadawcy == id_nadawcy)
				{
					std::shared_ptr<Komunikat> odebrany = kolejka.front();
					usun_komunikat();	//po odczytaniu komunikatu z kolejki, musi zostaæ z niej usuniêty
					if (nadawca->GetState() == State::OCZEKUJACY && kolejka.size()==2)
					{
						nadawca->SetState(State::GOTOWY);
						dodaj_do_procesow_gotowych(nadawca);
					}
					std::cout << "Tresc odebranego komunikatu: " << odebrany->tresc_komunikatu;
					return odebrany;
				}
			}

		}
		else if (kolejka.size() == 0)
		{
			std::cout << "Kolejka komunikatow dla tego procesu jest pusta" << std::endl;
			running->SetState(State::OCZEKUJACY);
			std::shared_ptr<Komunikat> odebrany = std::make_shared<Komunikat>(id_nadawcy, "----");
			return odebrany;
			//jeœli ju¿ bêdê robi³ tak, ¿e Komunikat bêdzie wskaŸnikiem, to tutaj zwrócê nullptr, bo kompilator sie sra, ¿e nic tutaj na razie nie jest zwracane
		}
	}
	else if (czy_istnieje == false)
	{
		std::cout << "Proces nadawcy nie istnieje, nie mozna odebrac komunikatu" << std::endl;
		std::shared_ptr<Komunikat> odebrany = std::make_shared<Komunikat>(id_nadawcy, "----");
		return odebrany;
	}
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
			std::cout << e->getTresc() << std::endl;		//chyba powinno wyœwietliæ, nie mam jeszcze pewnoœci
		}
	}
}
bool Kolejka::send(int id_odbiorcy, std::shared_ptr<Komunikat> komunikat)
{	//dostêp do kolejki: id procesu jest potrzebne. Wykorzystujê funkcjê GetPCB, któa zwraca shared pointer do PCB. Wtedy bêdzie PCB->kolejka
	//std::list<Group>ProcessGroupsList;
	bool wyslano = false;
	bool czy_pelna = false;
	bool czy_istnieje = false;
	for (auto &v : ProcessGroupsList)
	{
		for (auto &x : v.ProcessList)
		{
			if (x->ProcessID == id_odbiorcy)	//sprawdzenie, czy nadawca nie zakoñczy³ do tej pory dzia³ania
				czy_istnieje = true;
		}
	}
	if (czy_istnieje == true)
	{
		std::shared_ptr<PCB> odbiorca = GetPCB(id_odbiorcy);
		std::shared_ptr<PCB> nadawca = GetPCB(komunikat->id_nadawcy);
		if (odbiorca->kolejka.kolejka.size() >= 2)
		{
			nadawca->SetState(State::OCZEKUJACY);
			czy_pelna = true;
		}
		if (czy_pelna == false)
		{
			if (odbiorca->GetState() == State::OCZEKUJACY && odbiorca->kolejka.kolejka.size() == 0)
			{
				odbiorca->SetState(State::GOTOWY);
				dodaj_do_procesow_gotowych(odbiorca);
			}
			int grupa_odbiorcy = odbiorca->ProcessGroup;
			int grupa_nadawcy = nadawca->ProcessGroup;
			for (auto &it : ProcessGroupsList)
			{
				if (it.ProcessGroup == grupa_odbiorcy && it.ProcessGroup == grupa_nadawcy)
				{
					for (auto &e : it.ProcessList)
					{
						if (e->ProcessID == id_odbiorcy)
						{
							e->kolejka.dodaj_komunikat(komunikat);
							wyslano = true;
						}

					}
				}
			}
		}
		if (wyslano == false)
		{
			std::cout << "Nie wyslano komunikatu" << std::endl;
		}
	}
	else if (czy_istnieje == false)
	{
		std::cout << "Proces odbiorcy nie istnieje, nie mozna wyslac komunikatu" << std::endl;
		wyslano = true;
	}
	return wyslano;
}
