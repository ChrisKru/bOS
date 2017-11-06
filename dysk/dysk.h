#pragma once
#include <string>
#include <iostream>

struct Katalog
{
	std::string filename, ext;		//nazwa, typ
	int first_block;				//numer pierwszego bloku
	int size;						//rozmir pliku w znakach
	bool free;						//wolny do zapisu
	//bool data;						//czy zawiera dane
};

class Disc
{
	char disc_[1024];
	Katalog katalog_[32];
	int free_space_ = 992;	//ilosc wolnego miejsc - 32 bajty zarezerwowane
	bool block_[32];		//blok zajety/wolny true-wolny - szybsze sprawdzanie czy dany blok jest wolny

	bool file_exist(std::string filename, std::string ext);
	int free_catalog();
	int free_block();
	void available_free_space();
	int find_file(std::string filename, std::string ext);
	void save_block(int nr_bloku, std::string data);
	int free_block_space();
public:

	Disc();

	void create_file(std::string filename, std::string ext);							//utworzenie pliku
	void write_file(std::string filename, std::string ext, std::string data);			//zapis do pliku - jesli plik nie istnieje tworzy go
	void print_file(std::string filename, std::string ext);								//wyswietlenie zawartosci
	void delete_file(std::string filename, std::string ext);							//usuwanie pliku
	void print_file_list();																//wyswietla liste wszystkich plikow
	void rename_file(std::string filename, std::string ext, std::string new_filename);	//zmiana nazwy pliku
	void add_to_file(std::string filename, std::string ext, std::string data);			//dodanie do danych do juz istniejacych
	void show_file_info(std::string filename, std::string ext);							//data godzina itp
};
