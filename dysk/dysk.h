#pragma once
#include <string>
#include <iostream>
#include "../ConditionVariable/CV.h"

struct Katalog
{
	std::string filename;		//nazwa
	int first_block;			//numer pierwszego bloku
	int size;					//rozmir pliku w znakach
	bool free;					//wolny do zapisu
	bool open = false;					//czy plik otwarty
	CV cv_;						//Zmienna warunkowa
};

class Disc
{
	char disc_[1024];
	Katalog katalog_[32];
	bool block_[32];		//blok zajety/wolny true-wolny

	bool file_exist(std::string filename);
	int free_catalog();
	int free_block();
	int find_file(std::string filename);
	void save_block(int nr_bloku, std::string data);
	int free_block_space();
	void delete_block(int nr_bloku);
	std::string print_block(int nr_bloku, int znaki);

public:

	Disc();

	//Musi byc wywolane dla kazdego pliku!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	void open_file(std::string filename);
	void close_file(std::string filename);


	void create_file(std::string filename);								//utworzenie pliku
	void write_file(std::string filename, std::string data);			//zapis do pliku
	void print_file(std::string filename);								//wyswietlenie zawartosci
	void delete_file(std::string filename);								//usuwanie pliku
	void print_file_list();												//wyswietla liste wszystkich plikow
	void rename_file(std::string filename, std::string new_filename);	//zmiana nazwy pliku
	void add_to_file(std::string filename, std::string data);			//dodanie danych do danych juz istniejacych NIE MOZE ZAPISAC DO PUSTEGO PLIKU
	std::string getFile(std::string filename);							//zwraca stringa
	void printDisc();													//Wypisuje zawartosc calego dysku (wszystkie znaki w tablicy)
};
