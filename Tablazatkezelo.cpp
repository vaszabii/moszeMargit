/**
*
* \author Zöllei Alex, Varga Szabolcs, Szikonya Attila, Bogdán Ádám
*
* \date 2021/10/25
*
* Ez a táblázatkezelő fájl, paramétereket a felhasználó adja meg. Ez a fő kezelője a programnak.
*
* Created on 2021/10/25
*/


#include "Tablazat.h"
#include <regex>
#include <stdio.h>
#ifdef _WIN32 
#include <windows.h>
#endif
using namespace std;

//!A függvény kezeli a parancssorból kapott utasításokat.
/*!
	\param t A táblázat osztály egy egyede
*/
void Input(Spreadsheet& t) {
	const std::regex add(R"((?:\badd\b)\s([0-9]+)\s(\brows\b|\bcols\b))");
	const std::regex insert(R"((?:\binsert\b)\s([0-9]+)\s(\brows\b|\bcols\b)\s(\bbefore\b|\bafter\b)\s([a-zA-Z]|[0-9]+))");
	const std::regex del(R"((?:\bdelete\b)\s([a-zA-Z]|[0-9]+))");
	const std::regex edit(R"((?:\bedit\b)\s([a-zA-Z])([0-9]+)\s([^\r\n]+))");
	const std::regex save(R"((?:\bsave\b)\s(\b[\w]+\.csv\b)(?:\s(?:-sep)\s(.))?)");
	const std::regex align(R"((?:\balign\b)\s([a-zA-Z])([0-9]+)\s(\bleft\b|\bright\b))");
	const std::regex swap(R"((?:\bswap\b)\s([a-zA-Z])([0-9]+)\s([a-zA-Z])([0-9]+))");
	const std::regex rangeAlign(R"((?:\balign\b)\s([a-zA-Z])([0-9]+)(\b:\b)([a-zA-Z])([0-9]+)\s(\bleft\b|\bright\b))");
	const std::regex rangeClear(R"((?:\bclear\b)\s([a-zA-Z])([0-9]+)(\b:\b)([a-zA-Z])([0-9]+))");
	const std::regex sort(R"((?:\bsort by\b)\s([a-zA-Z]|[0-9]+)(?:\s(?:(\basc\b)|(\bdesc\b)))?)");
	const std::regex help(R"((?:\bhelp\b))");
	const std::string after = "after";
	const std::string before = "before";
	const std::string rows = "rows";

	int amount;
	std::string commandsStr;
	bool exitBool = false;
	std::cout << "Tablazatkezelo program. A help parancs megadasaval megjelennek a lehetseges parancsok" << std::endl << std::endl;
	t.printTable();
	do
	{
		std::smatch match;
		commandsStr = "";
		std::cout << std::endl << "Irjon be egy parancsot:";
		std::getline(std::cin, commandsStr);
		if (commandsStr == "exit") //Exit
		{
			exitBool = true;
		}
		else if (std::regex_match(commandsStr, match, help)) {
			std::cout << std::endl << "A lehetseges muveletek listaja" << std::endl << std::endl << "edit XY string : a megadott string beirasa az XY koordinataju cellaba (pl. A2)" << std::endl << std::endl << "add N rows/cols :  N db uj, ures cellakbol allo sort/oszlopot ad hozza a tablazat vegehez" << std::endl << std::endl << "delete X/Y : X oszlop vagy Y sor torlese(ha betu, akkor oszlop, ha szam, akkor sor)" << std::endl << std::endl << "insert N rows/cols before/after X/Y : A megadott pozicioba szur be N darab ures oszlopot/sort" << std::endl << std::endl << "save fajlnev.csv -sep kar: Megadott nevu CSV fajlba kiirja az aktualis tablazatot. Az ertekek elvalasztasa alapertelmezetten pontosvesszovel tortenik, de az opcionalis [-sep kar] paranccsal megvaltoztathato (pl. save tabla.csv -sep ,)" << std::endl << std::endl << "align XY left/right vagy align XY:MN left/right : Beallitja, hogy balra/jobbra igazitva legyen a cella(k) tartalma" << std::endl << std::endl << "clear MN:XY : A megadott cellak tartalmanak torlese range muvelettel" << std::endl << std::endl << "swap MN XY : A megadott ket cella tartalmanak felcserelese" << std::endl << std::endl << "sort by X/Y asc/desc : A tablazat sorait a megadott oszlop szerint (vagy oszlopait a megadott sor szerint) novekvo/csokkeno sorrendbe rendezi (alapertelmezetten novekvo)" << std::endl << std::endl << "exit : kilepes a programbol" << std::endl << std::endl;
		}
		else if (std::regex_match(commandsStr, match, add)) //Add
		{
			amount = stoi(match.str(1));
			if (match.str(2) == rows)
			{
				t.addRow(amount);
			}
			else
			{
				if (t.getColumnCounter() + amount > 26)
				{
					std::cout << "Nem lehet 26-nal tobb oszlop!" << std::endl;
				}
				else
				{
					t.addColumn(amount);
				}
			}
		}
		else if (std::regex_match(commandsStr, match, insert)) //Insert
		{
			amount = stoi(match.str(1));
			if (match.str(2) == rows)
			{
				int y = t.ynumberCheck(match.str(4));
				if (y > -1)
				{
					if (match.str(3) == after)
					{
						t.addRow(amount, y + 1);
					}
					else
					{
						t.addRow(amount, y);
					}
				}
				else if (y < -1) {
					std::cout << "Hibas parametert adtal meg!" << std::endl;
				}
				else if (y == -1)
				{
					std::cout << "Nem letezik a megadott Y koordinata (sor)!" << std::endl;
				}
			}
			else
			{
				if (t.getColumnCounter() + amount > 26)
				{
					std::cout << "Nem lehet 26-nal tobb oszlop!" << std::endl;
				}
				else
				{
					int x = t.xcharCheck(match.str(4));
					if (x > -1)
					{
						if (match.str(3) == after)
						{
							t.addColumn(amount, x + 1);
						}
						else
						{
							t.addColumn(amount, x);
						}
					}
					else if (x < -1) {
						std::cout << "Hibas parametert adtal meg!" << std::endl;
					}
					else if (x == -1)
					{
						std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
					}
				}
			}
		}
		else if (std::regex_match(commandsStr, match, del)) //Delete
		{
			int x = t.xcharCheck(match.str(1));
			if (x > -1)
			{
				if (t.getColumnCounter() > 1)
				{
					t.removeColumn(x);
				}
				else
				{
					for (size_t i = 0; i < t.getRowCounter(); i++)
					{
						t.editCell(0, i, "");
					}
				}
			}
			else if (x == -1)
			{
				std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
			}
			else
			{
				int y = t.ynumberCheck(match.str(1));
				if (y > -1)
				{
					if (t.getRowCounter() > 1)
					{
						t.removeRow(y);
					}
					else
					{
						for (size_t i = 0; i < t.getColumnCounter(); i++)
						{
							t.editCell(i, 0, "");
						}
					}
				}
				else if (y == -1)
				{
					std::cout << "Nem letezik a megadott Y koordinata (sor)!" << std::endl;
				}
			}
		}
		else if (std::regex_match(commandsStr, match, edit))  //Edit
		{
			int x = t.xcharCheck(match.str(1));
			int y = t.ynumberCheck(match.str(2));
			if (x > -1 && y > -1)
			{
				std::string editStr = match.str(3);
				editStr.erase(editStr.find_last_not_of(" \t\n\r\f\v") + 1); // Whitespacek levágása a sztring végéről
				t.editCell(x, y, editStr);
			}
			else
			{
				if (y == -1) {
					std::cout << "Nem letezik a megadott Y koordinata (sor)!" << std::endl;
				}
				if (x == -1)
				{
					std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
				}
			}
		}
		else if (std::regex_match(commandsStr, match, save)) //Save
		{
			char sep = ';';
			if (match.str(2) != "")
			{
				sep = match.str(2)[0];
			}
			t.writeTableToFile(match.str(1), sep);
		}
		else if (std::regex_match(commandsStr, match, align))//Align
		{
			int x = t.xcharCheck(match.str(1));
			int y = t.ynumberCheck(match.str(2));
			if (x > -1 && y > -1)
			{
				t.changeAlign(x, y, match.str(3));
			}
			else
			{
				if (y == -1) {
					std::cout << "Nem letezik a megadott Y koordinata (sor)!" << std::endl;
				}
				if (x == -1)
				{
					std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
				}
			}

		}
		else if (std::regex_match(commandsStr, match, swap)) //Swap
		{
			int x = t.xcharCheck(match.str(1));
			int y = t.ynumberCheck(match.str(2));
			int m = t.xcharCheck(match.str(3));
			int n = t.ynumberCheck(match.str(4));
			if (x > -1 && y > -1 && n > -1 && m > -1)
			{
				t.swap(x, y, m, n);
			}
			else
			{
				if (x == -1) {
					std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
				}
				if (m == -1) {
					std::cout << "Nem letezik a megadott M koordinata (oszlop)!" << std::endl;
				}
				if (y == -1)
				{
					std::cout << "Nem letezik a megadott Y koordinata (sor)!" << std::endl;
				}
				if (n == -1)
				{
					std::cout << "Nem letezik a megadott N koordinata (sor)!" << std::endl;
				}
			}

		}
		else if (std::regex_match(commandsStr, match, rangeAlign))
		{
			int x = t.xcharCheck(match.str(1));
			int y = t.ynumberCheck(match.str(2));
			int m = t.xcharCheck(match.str(4));
			int n = t.ynumberCheck(match.str(5));
			if (x > -1 && y > -1 && n > -1 && m > -1)
			{
				t.orderRangeParams(x, m);
				t.orderRangeParams(y, n);
				t.rangeAlign(x, y, m, n, match.str(6));
			}
			else
			{
				if (x == -1) {
					std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
				}
				if (m == -1) {
					std::cout << "Nem letezik a megadott M koordinata (oszlop)!" << std::endl;
				}
				if (y == -1) {
					std::cout << "Nem letezik a megadott Y koordinata (sor)!" << std::endl;
				}
				if (n == -1) {
					std::cout << "Nem letezik a megadott N koordinata (sor)!" << std::endl;
				}
			}
		}
		else if (std::regex_match(commandsStr, match, rangeClear)) {
			int x = t.xcharCheck(match.str(1));
			int y = t.ynumberCheck(match.str(2));
			int m = t.xcharCheck(match.str(4));
			int n = t.ynumberCheck(match.str(5));
			if (x > -1 && y > -1 && n > -1 && m > -1)
			{
				t.orderRangeParams(x, m);
				t.orderRangeParams(y, n);
				t.rangeClear(x, y, m, n);
			}
			else
			{
				if (x == -1) {
					std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
				}
				if (m == -1) {
					std::cout << "Nem letezik a megadott M koordinata (oszlop)!" << std::endl;
				}
				if (y == -1)
				{
					std::cout << "Nem letezik a megadott Y koordinata (sor)!" << std::endl;
				}
				if (n == -1)
				{
					std::cout << "Nem letezik a megadott N koordinata (sor)!" << std::endl;
				}
			}
		}
		else if (std::regex_match(commandsStr, match, sort)) //Sort by
		{
			int x = t.xcharCheck(match.str(1));
			std::string sortDirection;
			if (match.str(3) == "desc")
			{
				sortDirection = "desc";
			}
			else
			{
				sortDirection = "asc";
			}
			if (x > -1) // Ha létező oszlop szerint kell rendezni
			{
				t.sortBy(x, false, sortDirection);
			}
			else if (x == -1)
			{
				std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
			}
			else {
				int y = t.ynumberCheck(match.str(1));
				if (y > -1) // Ha létező sor szerint kell rendezni
				{
					t.sortBy(y, true, sortDirection);
				}
				else if (y == -1)
				{
					std::cout << "Nem letezik a megadott Y koordinata (sor)!" << std::endl;
				}
			}
		}
		else
		{
			std::cout << "Nem letezo parancsot adott meg!" << std::endl;
		}
		std::cout << std::endl;
		if (!exitBool)
		{
			t.printTable();
		}
	} while (!exitBool);
	if (exitBool)
	{
		exit(0);
	}
}

int main(int argc, char** argv)
{
	/**
	* Ékezetes betűk olvasására, kiíratására szolgál.
	*/
#ifdef _WIN32
	SetConsoleOutputCP(1250);
	SetConsoleCP(1250);
#endif

	if (argc > 1)
	{
		const std::regex filename(R"((\b[\w]+.csv\b))");
		std::string name = argv[1];
		if (std::regex_match(name, filename)) {
			char sep = ';';
			if (argc == 4 && strcmp(argv[2], "-sep") == 0)
			{
				std::string sepChar = argv[3];
				if (sepChar.length() == 1)
				{
					sep = sepChar[0];
				}
				else
				{
					std::cout << "Hibas szeparator karakter!" << std::endl;
				}
			}
			Spreadsheet t(name, sep);
			Input(t);
		}
		else
		{
			std::cout << "Hibas parametert adtal meg argumentumnkent! Alap tablazat betoltese..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			Spreadsheet t;
			Input(t);
		}
	}
	else
	{
		Spreadsheet t;
		Input(t);
	}
	return 0;
}
