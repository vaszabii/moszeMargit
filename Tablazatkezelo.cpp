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
#include <stdio.h>
#ifdef _WIN32 
#include <windows.h>
#endif
using namespace std;

//!A függvény kezeli a parancssorból kapott utasításokat.
/*!
	\param t A táblázat osztály egy egyede
*/
void Input(std::vector<Spreadsheet*>& Spreadsheets) {
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
	const std::regex barchart(R"((?:\bbarchart\b)\s([a-zA-Z])([0-9]+)(\b:\b)([a-zA-Z])([0-9]+)\s(\b[\w]+\b))");
	const std::regex open(R"((?:\bopen\b)\s(\b[\w]+\b)(\.\b[\w]+\b)?(?:\s(?:-sep)\s(.))?)");
	const std::regex newsheet(R"((?:\bnew\b)\s(?:\bsheet\b)\s([^\r\n]+))");
	const std::regex switchsheet(R"((?:\bswitch\b)\s([0-9]+))");
	const std::regex closesheet(R"((?:\bclose\b)\s([0-9]+))");
	const std::regex rename(R"((?:\brename\b)\s([0-9]+\s([^\r\n]+)))");
	const std::regex help(R"((?:\bhelp\b))");
	const std::string after = "after";
	const std::string before = "before";
	const std::string rows = "rows";
	Spreadsheet* t = Spreadsheets[0];
	int amount;
	std::string commandsStr;
	bool exitBool = false;
	std::cout << "Tablazatkezelo program. A help parancs megadasaval megjelennek a lehetseges parancsok" << std::endl << std::endl;
	do
	{
		if (!exitBool)
		{
			t->printTable();
			std::cout << std::endl;
			for (size_t i = 0; i < Spreadsheets.size(); i++)
			{
				if (Spreadsheets[i] == t)
				{
					std::cout << i << "*: " << Spreadsheets[i]->getName() << " ";
				}
				else
				{
					std::cout << i << ": " << Spreadsheets[i]->getName() << " ";
				}
			}
			std::cout << std::endl;
		}
		std::smatch match;
		commandsStr = "";
		std::cout << std::endl << "Irjon be egy parancsot:";
		std::getline(std::cin, commandsStr);
		if (commandsStr == "exit") //Exit
		{
			exitBool = true;
		}
		else if (std::regex_match(commandsStr, match, help)) {
			std::cout << std::endl << "A lehetseges muveletek listaja" << std::endl << std::endl << "edit XY string : a megadott string beirasa az XY koordinataju cellaba (pl. A2)" << std::endl << std::endl << "add N rows/cols :  N db uj, ures cellakbol allo sort/oszlopot ad hozza a tablazat vegehez" << std::endl << std::endl << "delete X/Y : X oszlop vagy Y sor torlese(ha betu, akkor oszlop, ha szam, akkor sor)" << std::endl << std::endl << "insert N rows/cols before/after X/Y : A megadott pozicioba szur be N darab ures oszlopot/sort" << std::endl << std::endl << "save fajlnev.csv -sep kar: Megadott nevu CSV fajlba kiirja az aktualis tablazatot. Az ertekek elvalasztasa alapertelmezetten pontosvesszovel tortenik, de az opcionalis [-sep kar] paranccsal megvaltoztathato (pl. save tabla.csv -sep ,)" << std::endl << std::endl << "align XY left/right vagy align XY:MN left/right : Beallitja, hogy balra/jobbra igazitva legyen a cella(k) tartalma" << std::endl << std::endl << "clear MN:XY : A megadott cellak tartalmanak torlese range muvelettel" << std::endl << std::endl << "swap MN XY : A megadott ket cella tartalmanak felcserelese" << std::endl << std::endl << "sort by X/Y asc/desc : A tablazat sorait a megadott oszlop szerint (vagy oszlopait a megadott sor szerint) novekvo/csokkeno sorrendbe rendezi (alapertelmezetten novekvo)" << std::endl << std::endl << "barchart XY:MN filename : A megadott tartomanybol eloallit egy csoportositott oszlopdiagramot abrazolo SVG abrat HTML-be agyazva" << std::endl << std::endl << std::endl
				<<"Talbazatok kezelesere vonatkozo muveletek: " << std::endl << std::endl
				<< "new sheet name : letrehoz egy uj tablazatot a megadott nevvel es beallitja aktivnak" << std::endl << std::endl
				<< "switch N : beallitja az N indexu tablazatot aktivnak" << std::endl << std::endl
				<< "open filename [-sep ,] : a megadott fajlbol betolti a tablazatot" << std::endl << std::endl
				<< "close N : bezarja az N indexu tablazatot" << std::endl << std::endl
				<< "rename N newname : atnevezi az N indexu tablazatot a megadott uj nevre" << std::endl << std::endl
				<< "exit : kilepes a programbol" << std::endl << std::endl;
		}
		else if (std::regex_match(commandsStr, match, add)) //Add
		{
			amount = stoi(match.str(1));
			if (match.str(2) == rows)
			{
				t->addRow(amount);
			}
			else
			{
				if (t->getColumnCounter() + amount > 26)
				{
					std::cout << "Nem lehet 26-nal tobb oszlop!" << std::endl;
				}
				else
				{
					t->addColumn(amount);
				}
			}
		}
		else if (std::regex_match(commandsStr, match, insert)) //Insert
		{
			amount = stoi(match.str(1));
			if (match.str(2) == rows)
			{
				int y = t->ynumberCheck(match.str(4));
				if (y > -1)
				{
					if (match.str(3) == after)
					{
						t->addRow(amount, y + 1);
					}
					else
					{
						t->addRow(amount, y);
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
				if (t->getColumnCounter() + amount > 26)
				{
					std::cout << "Nem lehet 26-nal tobb oszlop!" << std::endl;
				}
				else
				{
					int x = t->xcharCheck(match.str(4));
					if (x > -1)
					{
						if (match.str(3) == after)
						{
							t->addColumn(amount, x + 1);
						}
						else
						{
							t->addColumn(amount, x);
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
			int x = t->xcharCheck(match.str(1));
			if (x > -1)
			{
				if (t->getColumnCounter() > 1)
				{
					t->removeColumn(x);
				}
				else
				{
					for (size_t i = 0; i < t->getRowCounter(); i++)
					{
						t->editCell(0, i, "");
					}
				}
			}
			else if (x == -1)
			{
				std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
			}
			else
			{
				int y = t->ynumberCheck(match.str(1));
				if (y > -1)
				{
					if (t->getRowCounter() > 1)
					{
						t->removeRow(y);
					}
					else
					{
						for (size_t i = 0; i < t->getColumnCounter(); i++)
						{
							t->editCell(i, 0, "");
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
			int x = t->xcharCheck(match.str(1));
			int y = t->ynumberCheck(match.str(2));
			if (x > -1 && y > -1)
			{
				std::string editStr = match.str(3);
				editStr.erase(editStr.find_last_not_of(" \t\n\r\f\v") + 1); // Whitespacek levágása a sztring végéről
				t->editCell(x, y, editStr);
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
			t->writeTableToFile(match.str(1), sep);
		}
		else if (std::regex_match(commandsStr, match, align))//Align
		{
			int x = t->xcharCheck(match.str(1));
			int y = t->ynumberCheck(match.str(2));
			if (x > -1 && y > -1)
			{
				t->changeAlign(x, y, match.str(3));
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
			int x = t->xcharCheck(match.str(1));
			int y = t->ynumberCheck(match.str(2));
			int m = t->xcharCheck(match.str(3));
			int n = t->ynumberCheck(match.str(4));
			if (x > -1 && y > -1 && n > -1 && m > -1)
			{
				t->swap(x, y, m, n);
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
		else if (std::regex_match(commandsStr, match, barchart))
		{
			int x = t->xcharCheck(match.str(1));
			int y = t->ynumberCheck(match.str(2));
			int m = t->xcharCheck(match.str(4));
			int n = t->ynumberCheck(match.str(5));
			std::string filename = match.str(6);
			if (x > -1 && y > -1 && n > -1 && m > -1)
			{
				t->orderRangeParams(x, m);
				t->orderRangeParams(y, n);
				t->barChart(x, y, m, n, filename);
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
		else if (std::regex_match(commandsStr, match, rangeAlign))
		{
			int x = t->xcharCheck(match.str(1));
			int y = t->ynumberCheck(match.str(2));
			int m = t->xcharCheck(match.str(4));
			int n = t->ynumberCheck(match.str(5));
			if (x > -1 && y > -1 && n > -1 && m > -1)
			{
				t->orderRangeParams(x, m);
				t->orderRangeParams(y, n);
				t->rangeAlign(x, y, m, n, match.str(6));
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
			int x = t->xcharCheck(match.str(1));
			int y = t->ynumberCheck(match.str(2));
			int m = t->xcharCheck(match.str(4));
			int n = t->ynumberCheck(match.str(5));
			if (x > -1 && y > -1 && n > -1 && m > -1)
			{
				t->orderRangeParams(x, m);
				t->orderRangeParams(y, n);
				t->rangeClear(x, y, m, n);
			}
			else
			{

				if (m == -1) {
					std::cout << "Nem letezik a megadott M koordinata (oszlop)!" << std::endl;
				}
				if (x == -1) {
					std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
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
			int x = t->xcharCheck(match.str(1));
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
				t->sortBy(x, false, sortDirection);
			}
			else if (x == -1)
			{
				std::cout << "Nem letezik a megadott X koordinata (oszlop)!" << std::endl;
			}
			else {
				int y = t->ynumberCheck(match.str(1));
				if (y > -1) // Ha létező sor szerint kell rendezni
				{
					t->sortBy(y, true, sortDirection);
				}
				else if (y == -1)
				{
					std::cout << "Nem letezik a megadott Y koordinata (sor)!" << std::endl;
				}
			}
		}
		//else if (commandsStr == "new") { //test
		//	Spreadsheets.push_back(new Spreadsheet("asd"));
		//	t = Spreadsheets[Spreadsheets.size() - 1];
		//}
		//else if (commandsStr == "switch") {
		//	t = Spreadsheets[0];
		//}
		else if (std::regex_match(commandsStr, match, open)) //Open
		{
			char sep = ';';
			if (match.str(3) != "")
			{
				sep = match.str(3)[0];
			}
			std::string filename = match.str(1);
			std::string filewithexten = filename + match.str(2);
			try
			{
				Spreadsheets.push_back(new Spreadsheet(filewithexten, sep, filename));
				t = Spreadsheets[Spreadsheets.size() - 1];
			}
			catch (const std::runtime_error&)
			{
				//Parancs figyelmen kívül hagyása
			}
		}
		else if (std::regex_match(commandsStr, match, newsheet)) //New Sheet
		{
			std::string tableName = match.str(1);
			Spreadsheets.push_back(new Spreadsheet(tableName));
			t = Spreadsheets[Spreadsheets.size() - 1];
		}
		else if (std::regex_match(commandsStr, match, switchsheet))//Switch sheet
		{
			std::string indexstring = match.str(1);
			size_t index = std::stoi(indexstring);
			if (index >= 0 && index < Spreadsheets.size())
			{
				t = Spreadsheets[index];
			}
			else
			{
				std::cout << "Nem letezik a megadott indexu tabla!" << std::endl;
			}
		}
		else if (std::regex_match(commandsStr, match, rename))//Rename sheet
		{
			std::string indexstring = match.str(1);
			std::string newName = match.str(2);
			size_t index = std::stoi(indexstring);
			if (index >= 0 && index < Spreadsheets.size())
			{
				Spreadsheets[index]->setName(newName);
			}
			else
			{
				std::cout << "Nem letezik a megadott indexu tabla!" << std::endl;
			}
		}
		else if (std::regex_match(commandsStr, match, closesheet)) //Close sheet
		{
			std::string indexstring = match.str(1);
			size_t index = std::stoi(indexstring);
			if (index >= 0 && index < Spreadsheets.size())
			{
				if (t == Spreadsheets[index] && Spreadsheets.size() > 1)
				{
					if (index == 0)
					{
						t = Spreadsheets[index + 1];
					}
					else
					{
						t = Spreadsheets[index - 1];
					}
				}
				delete Spreadsheets[index];
				Spreadsheets.erase(Spreadsheets.begin() + index);
				if (Spreadsheets.size() == 0)
				{
					std::cout << "Az osszes tabla bezarodott. Kilepes a programbol..." << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(3));
					exitBool = true;
				}

			}
			else
			{
				std::cout << "Nem letezik a megadott indexu tabla!" << std::endl;
			}
		}
		else
		{
			std::cout << "Nem letezo parancsot adott meg!" << std::endl;
		}
		std::cout << std::endl;
	} while (!exitBool);
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
	std::vector<Spreadsheet*> Spreadsheets;
	if (argc > 1)
	{
		std::smatch match;
		const std::regex filename(R"((?:\b([\w]+).csv\b))");
		std::string name = argv[1];
		if (std::regex_match(name, match, filename)) {
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
			Spreadsheets.push_back(new Spreadsheet(name, sep, match.str(1)));
			Input(Spreadsheets);
		}
		else
		{
			std::cout << "Hibas parametert adtal meg argumentumnkent! Alap tablazat betoltese..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			Spreadsheets.push_back(new Spreadsheet("Table"));
			Input(Spreadsheets);
		}
	}
	else
	{
		Spreadsheets.push_back(new Spreadsheet("Table"));
		Input(Spreadsheets);
	}
	for (size_t i = 0; i < Spreadsheets.size(); ++i) {
		delete Spreadsheets[i];
	}
	Spreadsheets.clear();
	return 0;
}
