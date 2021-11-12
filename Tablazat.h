/**
* \class Spreadsheet
*
* \author Zöllei Alex, Varga Szabolcs, Szikonya Attila, Bogdán Ádám
*
* \date 2021/10/19
*
* \brief Spreadsheet class in Tablazat.h
*
* Ez a táblázat osztály, ami tartalmazza a táblázathoz tartozó függvények és funkciók deklarálását.
*
* Created on 2021/10/19
*/


#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>

class Spreadsheet {
private:
	const std::string emptyCell = ""; ///< Üres cella.
	size_t columnCounter = 1; ///< Oszlopok száma.
	size_t rowCounter = 1; ///< Sorok száma.
	std::vector<std::vector<std::string>> table; ///< Táblázat tartalmának tárolása.
	std::vector<std::vector<std::string>> tableAlign; ///< Cellaformázás tárolása.
	std::vector<std::vector<std::pair<std::string, bool>>> tableAggregate;  ///< Aggregátor fv.-k eredményének tárolása.
public:
	//!konstruktor
	Spreadsheet();
	//!Paraméterezett konstruktor, az argumentumként kapott CSV táblázatot letárol.
	/*!
	\param filename a fájl neve
	\param separator a szeparátor karaktere
	*/
	Spreadsheet(std::string filename, char separator);
	/*!
	\param amount a mennyiség, amit hozzá kívánunk adni
	*/
	void addColumn(int amount, int x = -1); ///< Egy oszlop hozzáadása.
	/*!
	\param amount a mennyiség, amit hozzá kívánunk adni
	*/
	void addRow(int amount = 1, int y = -1); ///< Egy sor hozzáadása.
	/*!
	\param x az oszlop koordinátája
	\param y a sor koordinátája
	\param text a módosítani kívánt cellatartalom
	*/
	void editCell(int x, int y, std::string text); ///< Egy cella szerkesztése.
	void removeColumn(int x); ///< Egy oszlop eltávolátasa.
	void removeRow(int y); ///< Egy sor eltávolítása.
	void printTable(); ///< Táblázat megjelenítése.
	//! Táblázat kiírása fájlba
	/*!
	\param name az elmenteni kívánt csv fájl neve
	\param separator az adatokat elkülönítő operátor
	*/
	void writeTableToFile(std::string name, char separator);
	size_t getColumnCounter(); ///< Oszlopok számának a meghatározása
	size_t getRowCounter(); ///< Sorok számának a meghatározása
	/*!
	\param x az oszlop koordinátája
	\param y a sor koordinátája
	\param align a cella tartalmának igazítása
	*/
	void changeAlign(int x, int y, std::string align); ///< Cellaigazítás megváltoztatása.
	void swap(int m, int n, int x, int y); ///< Két cella adatának megcserélése.
	void rangeAlign(int x, int y, int m, int n, std::string align); ///< Cellaigazítás az összes kijelölt cellán .
	void clearCell(int x, int y); ///< Cella tartalmának törlése.
	void rangeClear(int x, int y, int m, int n); ///< Tartalomtörlés az összes kijelölt cellán.
	/*!
	\param by rendezni kívánt sor/oszlop
	\param is_it_row sor vagy oszlop-e
	\param direction növekvő vagy csökkeni rendezés
	*/
	void sortBy(int by, bool is_it_row, std::string direction); ///< Táblázat rendezése oszlop/sor szerint.
	/*!
		\param yPar Az oszlop indexe
	*/
	int xcharCheck(std::string xPar); ///< Ellenőrzi, hogy létezik-e olyan oszlop, ha igen akkor visszaadja táblázat oszlopindexét.
	/*!
		\param yPar A sor indexe
	*/
	int ynumberCheck(std::string yPar); ///< Ellenőrzi, hogy létezik-e olyan sor, ha igen akkor visszaadja táblázat sorindexét.
	void orderRangeParams(int& a, int& b); ///< A range műveletek paraméterének helyes sorrenbe állítása.
	void updateAggregateTable(); ///< Aggregátor fv.-k vektorának frissítése a sima táblázat kiíratásakor.
	bool cycle(int x, int y, int m, int n);
	std::string get(int row, int column); ///< A kiírandó táblázat egy elemét adja vissza (teszteléshez szükséges)
};
