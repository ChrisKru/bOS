#ifndef komunikat_h
#define komunikat_h

#include<string>

class Komunikat
{
public:
	int id_nadawcy;
	int rozmiar_komunikatu;
	std::string tresc_komunikatu;

	Komunikat::Komunikat();
	Komunikat(int id, std::string tresc);
	int getId();
	int getRozmair();
	std::string getTresc();

};
#endif