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
#include "BarchartMaker.h"
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <sstream>
using namespace std;
Spreadsheet::Spreadsheet(std::string tableName) {
	addRow();
	spreadsheetname = tableName;
}

Spreadsheet::Spreadsheet(std::string filename, char separator, std::string tableName) {
	std::ifstream myfile;
	myfile.open(filename);
	if (myfile.is_open())
	{
		std::string rowStr, cellValue;
		while (std::getline(myfile, rowStr)) {
			std::vector<std::string> rowValues = {};
			size_t pos = 0;
			while ((pos = rowStr.find(separator)) != std::string::npos) { // Sztring split
				cellValue = rowStr.substr(0, pos);
				cellValue.erase(cellValue.find_last_not_of(" \t\n\r\f\v") + 1); // Whitespacek levágása a cellák végéről
				rowValues.push_back(cellValue);
				rowStr.erase(0, pos + 1);
			}
			rowValues.push_back(rowStr);
			table.push_back(rowValues);
		}
		size_t maxRowLength = 0;
		bool notEqual = false;
		for (size_t i = 0; i < table.size(); i++) // Leghosszabb sor meghatározása
		{
			if (maxRowLength < table[i].size())
			{
				maxRowLength = table[i].size();
				notEqual = true;
			}
		}
		if (notEqual) // Ha nem mindegyik sor egyenlő akkor üres sorokkal töltüjk fel a kisebb sorokat amíg nem lesznek egyenlőek
		{
			size_t diff = 0;
			for (size_t i = 0; i < table.size(); i++)
			{
				if (maxRowLength > table[i].size())
				{
					diff = maxRowLength - table[i].size();
					for (size_t j = 0; j < diff; j++)
					{
						table[i].push_back("");
					}
				}
			}
		}
		columnCounter = table[0].size();
		rowCounter = table.size();
		myfile.close();
		std::vector<std::vector<std::string>> tempTableAlign;
		std::vector<std::vector<std::pair<std::string, bool>>> tempTableAggregate;
		for (size_t i = 0; i < rowCounter; i++)
		{
			std::vector<std::string> tempAlignVector;
			std::vector < std::pair<std::string, bool>> tempAggregateVector;
			for (size_t j = 0; j < columnCounter; j++)
			{
				tempAlignVector.push_back("left");
				tempAggregateVector.push_back({ emptyCell, false });
			}
			tempTableAlign.push_back(tempAlignVector);
			tempTableAggregate.push_back(tempAggregateVector);
		}
		tableAlign = tempTableAlign;
		tableAggregate = tempTableAggregate;
		spreadsheetname = tableName;
	}
	else
	{
		std::string answerStr;
		std::cout << "Nem sikerult megnyitni a fajlt. Szeretne betolteni az alap, ures tablazat? (Y/N) ";
		std::getline(std::cin, answerStr);
		char answer = toupper(answerStr[0]);
		if (answer == 'Y')
		{
			std::cout << "Alap tablazat betoltese....";
			std::this_thread::sleep_for(std::chrono::seconds(5));
			addRow();
			spreadsheetname = "Table";
		}
		else
		{
			throw std::runtime_error("Nem sikerult megnyitni a fajlt.");
		}
	}
}

void Spreadsheet::addRow(int amount, int y) {
	std::vector<std::string> tempVector;
	std::vector<std::string> tempAlignVector;
	std::vector<std::pair<std::string, bool>> tempAggregateVector;
	for (size_t i = 0; i < columnCounter; i++)
	{
		tempVector.push_back(emptyCell);
		tempAlignVector.push_back("left");
		tempAggregateVector.push_back({ emptyCell, false });
	}
	for (int i = 0; i < amount; i++)
	{
		if (y == -1)
		{
			table.push_back(tempVector);
			tableAlign.push_back(tempAlignVector);
			tableAggregate.push_back(tempAggregateVector);
		}
		else
		{
			table.insert(table.begin() + y, tempVector);
			tableAlign.insert(tableAlign.begin() + y, tempAlignVector);
			tableAggregate.insert(tableAggregate.begin() + y, tempAggregateVector);
		}
	}
	rowCounter = table.size();
}

void Spreadsheet::addColumn(int amount, int x) {

	for (int i = 0; i < amount; i++)
	{
		for (size_t k = 0; k < rowCounter; k++)
		{
			if (x == -1)
			{
				table[k].push_back(emptyCell);
				tableAlign[k].push_back("left");
				tableAggregate[k].push_back({ emptyCell, false });
			}
			else
			{
				table[k].insert(table[k].begin() + x, emptyCell);
				tableAlign[k].insert(tableAlign[k].begin() + x, "left");
				tableAggregate[k].insert(tableAggregate[k].begin() + x, { emptyCell, false });
			}
		}
	}
	columnCounter = table[0].size();
}

void Spreadsheet::editCell(int x, int y, std::string text) {
	table[y][x] = text;
	tableAggregate[y][x].first = emptyCell;
}
void Spreadsheet::clearCell(int x, int y) {
	table[y][x] = emptyCell;
	tableAggregate[y][x].first = emptyCell;
}
void Spreadsheet::changeAlign(int x, int y, std::string align) {
	tableAlign[y][x] = align;
}
void Spreadsheet::swap(int x, int y, int m, int n) {
	std::string assistant = table[n][m];
	table[n][m] = table[y][x];
	table[y][x] = assistant;
	std::string assistantAggregate = tableAggregate[n][m].first;
	tableAggregate[n][m].first = tableAggregate[y][x].first;
	tableAggregate[y][x].first = assistantAggregate;
}

void Spreadsheet::rangeAlign(int x, int y, int m, int n, std::string align) {
	for (;y <= n; y++)
	{
		int a = x;
		for (;a <= m; a++)
		{
			changeAlign(a, y, align);
		}
	}
}
void Spreadsheet::rangeClear(int x, int y, int m, int n) {
	for (; y <= n; y++)
	{
		int a = x;
		for (; a <= m; a++)
		{
			clearCell(a, y);
		}
	}
}
void Spreadsheet::removeRow(int y) {
	table.erase(table.begin() + y);
	table.shrink_to_fit();
	tableAlign.erase(tableAlign.begin() + y);
	tableAlign.shrink_to_fit();
	tableAggregate.erase(tableAggregate.begin() + y);
	tableAggregate.shrink_to_fit();
	rowCounter = table.size();
}

void Spreadsheet::removeColumn(int x) {
	for (size_t i = 0; i < rowCounter; i++)
	{
		table[i].erase(table[i].begin() + x);
		table[i].shrink_to_fit();
		tableAlign[i].erase(tableAlign[i].begin() + x);
		tableAlign[i].shrink_to_fit();
		tableAggregate[i].erase(tableAggregate[i].begin() + x);
		tableAggregate[i].shrink_to_fit();
	}
	columnCounter = table[0].size();
}

bool Spreadsheet::cycle(int x, int y, int m, int n) {
	const std::regex sum(R"((?:=)(\bSUM\b|\bsum\b)(?:\()([a-zA-Z])([0-9]+)(?:\b:\b)([a-zA-Z])([0-9]+)(?:\)))");
	const std::regex avg(R"((?:=)(\bAVG\b|\bavg\b)(?:\()([a-zA-Z])([0-9]+)(?:\b:\b)([a-zA-Z])([0-9]+)(?:\)))");
	const std::regex min(R"((?:=)(\bMIN\b|\bmin\b)(?:\()([a-zA-Z])([0-9]+)(?:\b:\b)([a-zA-Z])([0-9]+)(?:\)))");
	const std::regex max(R"((?:=)(\bMAX\b|\bmax\b)(?:\()([a-zA-Z])([0-9]+)(?:\b:\b)([a-zA-Z])([0-9]+)(?:\)))");
	orderRangeParams(x, m);
	orderRangeParams(y, n);
	for (; y <= n; y++) {
		int a = x;
		for (; a <= m; a++) {
			std::smatch matchCycle;
			if (tableAggregate[y][a].second)
			{
				return true;
			}
			else
			{
				if (std::regex_match(table[y][a], matchCycle, sum) || std::regex_match(table[y][a], matchCycle, avg) || std::regex_match(table[y][a], matchCycle, min) || std::regex_match(table[y][a], matchCycle, max))
				{
					tableAggregate[y][a].second = true;
					if (cycle(xcharCheck(matchCycle.str(2)), ynumberCheck(matchCycle.str(3)), xcharCheck(matchCycle.str(4)), ynumberCheck(matchCycle.str(5))))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}


void Spreadsheet::updateAggregateTable() {
	//Aggregátor függvények regexei
	const std::regex sum(R"((?:=)(\bSUM\b|\bsum\b)(?:\()([a-zA-Z])([0-9]+)(?:\b:\b)([a-zA-Z])([0-9]+)(?:\)))");
	const std::regex avg(R"((?:=)(\bAVG\b|\bavg\b)(?:\()([a-zA-Z])([0-9]+)(?:\b:\b)([a-zA-Z])([0-9]+)(?:\)))");
	const std::regex min(R"((?:=)(\bMIN\b|\bmin\b)(?:\()([a-zA-Z])([0-9]+)(?:\b:\b)([a-zA-Z])([0-9]+)(?:\)))");
	const std::regex max(R"((?:=)(\bMAX\b|\bmax\b)(?:\()([a-zA-Z])([0-9]+)(?:\b:\b)([a-zA-Z])([0-9]+)(?:\)))");
	const std::regex is_it_pure_number(R"(^([+-]?(?:[\d]+\.?|[\d]*\.[\d]+))$)");
	std::string aggFv, aggValue;
	int x, y, m, n, counter, a;
	bool firstMin, isCycle;
	double calcValue, helper;
	for (size_t i = 0; i < rowCounter; i++) // Aggregátor fv-k eredményének megőrzése külön vektorban
	{
		for (size_t j = 0; j < columnCounter; j++)
		{
			std::smatch match;
			aggFv = table[i][j];
			if (std::regex_match(aggFv, match, sum) || std::regex_match(aggFv, match, avg) || std::regex_match(aggFv, match, min) || std::regex_match(aggFv, match, max)) //Aggregátor fv.-k vizsgálata
			{
				x = xcharCheck(match.str(2));
				y = ynumberCheck(match.str(3));
				m = xcharCheck(match.str(4));
				n = ynumberCheck(match.str(5));
				if (x > -1 && y > -1 && n > -1 && m > -1) {
					orderRangeParams(x, m);
					orderRangeParams(y, n);
					tableAggregate[i][j].second = true;
					calcValue = 0.0;
					counter = 0;
					firstMin = false;
					isCycle = false;
					while (y <= n && !isCycle)
					{
						a = x;
						while (a <= m && !isCycle)
						{
							try {
								std::smatch matchCycle;
								helper = 0.0;
								if (std::regex_match(table[y][a], matchCycle, sum) || std::regex_match(table[y][a], matchCycle, avg) || std::regex_match(table[y][a], matchCycle, min) || std::regex_match(table[y][a], matchCycle, max))
								{
									tableAggregate[y][a].second = true;
									if (cycle(xcharCheck(matchCycle.str(2)), ynumberCheck(matchCycle.str(3)), xcharCheck(matchCycle.str(4)), ynumberCheck(matchCycle.str(5))))
									{
										aggValue = "#CYCLE!";
										isCycle = true;
									}
									else
									{
										helper = std::stod(tableAggregate[y][a].first);
									}
								}
								else
								{
									if (std::regex_match(table[y][a], is_it_pure_number))
									{
										helper = std::stod(table[y][a]);
									}
									else
									{
										throw std::invalid_argument("Cant convert string.");
									}
								}
								if (!isCycle)
								{
									if (match.str(1) == "SUM" || match.str(1) == "sum") //SUM
									{
										calcValue += helper;
									}
									else if (match.str(1) == "AVG" || match.str(1) == "avg") //AVG
									{
										calcValue += helper;
										counter++;
									}
									else if (match.str(1) == "MIN" || match.str(1) == "min") //MIN
									{
										if (!firstMin) {
											calcValue = helper;
											firstMin = true;
										}
										else {
											if (helper < calcValue) {
												calcValue = helper;
											}
										}
									}
									else if (match.str(1) == "MAX" || match.str(1) == "max") //MAX
									{
										if (helper > calcValue) {
											calcValue = helper;
										}
									}
								}
							}
							catch (const std::invalid_argument&) {
								//Cella figyelmen kívül hagyása
							}
							catch (const std::out_of_range&) {
								//Cella figyelmen kívül hagyása
							}
							if (!isCycle)
							{
								if (match.str(1) == "AVG" || match.str(1) == "avg") {
									aggValue = std::to_string(calcValue / counter);
								}
								else
								{
									aggValue = std::to_string(calcValue);
								}
							}
							a++;
						}
						y++;
					}
					for (size_t k = 0; k < rowCounter; k++)
					{
						for (size_t h = 0; h < columnCounter; h++)
						{
							tableAggregate[k][h].second = false;
						}
					}
				}
				else
				{
					aggValue = "#NAME?";
				}
				tableAggregate[i][j].first = aggValue;
			}
		}
	}
}


void Spreadsheet::printTable() {
	updateAggregateTable();
	char columnSeparator = '|';
	char rowSeparator = '-';
	size_t* cellsMaxLength = new size_t[columnCounter];

	for (size_t i = 0; i < columnCounter; i++)
	{
		cellsMaxLength[i] = 0;
	}
	for (size_t i = 0; i < rowCounter; i++) // Oszlop szélesség meghatározása
	{
		for (size_t y = 0; y < columnCounter; y++)
		{
			size_t aggcellLength, cellLength;
			if (tableAggregate[i][y].first != "")
			{
				if (tableAggregate[i][y].first != "#NAME?" && tableAggregate[i][y].first != "#CYCLE!")
				{
					aggcellLength = tableAggregate[i][y].first.find_first_of('.') + 4;
				}
				else
				{
					aggcellLength = tableAggregate[i][y].first.length();
				}
				cellLength = 0;
			}
			else
			{
				cellLength = table[i][y].length();
				aggcellLength = 0;
			}

			if (table[i][y] == "" && cellsMaxLength[y] == 0)
			{
				cellsMaxLength[y] = 1;
			}
			else if (cellsMaxLength[y] < cellLength || cellsMaxLength[y] < aggcellLength)
			{
				if (cellsMaxLength[y] < aggcellLength)
				{
					cellsMaxLength[y] = aggcellLength;
				}
				else if (cellsMaxLength[y] < cellLength)
				{
					cellsMaxLength[y] = cellLength;
				}
			}
		}
	}
	int rowNumbers = (int)rowCounter + 1;
	size_t numberOfDigits = 0; // Sorszám oszlop szélességének meghatározása
	do {
		++numberOfDigits;
		rowNumbers /= 10;
	} while (rowNumbers);

	size_t rowSeparatorLength = numberOfDigits + 1;
	for (size_t i = 0; i < numberOfDigits; i++)
	{
		std::cout << " ";
	}
	std::cout << columnSeparator;
	for (size_t i = 0; i < columnCounter; i++) // Fejléc kiíratása
	{
		char letter = char('A' + i);
		std::cout << letter;
		size_t a = cellsMaxLength[i] - 1;
		for (size_t i = 0; i < a; i++)
		{
			std::cout << " ";
		}
		rowSeparatorLength = rowSeparatorLength + cellsMaxLength[i];
		rowSeparatorLength++;
		std::cout << columnSeparator;
	}
	std::cout << std::endl;
	for (size_t i = 0; i < rowSeparatorLength; i++) // Sorelválasztó 
	{
		std::cout << rowSeparator;
	}
	std::cout << std::endl;
	bool aggregate = false;
	for (size_t i = 0; i < rowCounter; i++) // Tartalom kiíratása
	{
		size_t rn = i + 1;
		size_t rnDigits = std::to_string(rn).length();
		rnDigits = numberOfDigits - rnDigits;
		std::cout << rn; // Sorszám a sor elejére
		for (size_t j = 0; j < rnDigits; j++)
		{
			std::cout << " ";
		}
		std::cout << columnSeparator;
		for (size_t y = 0; y < columnCounter; y++)
		{
			std::string content = " ";
			aggregate = false;
			size_t whitespaces = cellsMaxLength[y] - 1;
			if (table[i][y] != "")
			{
				if (tableAggregate[i][y].first != emptyCell)// Aggregáló táblából kiíratás
				{
					content = tableAggregate[i][y].first;
					aggregate = true;
					size_t aggcellLength;
					if (content != "#NAME?" && content != "#CYCLE!")
					{
						aggcellLength = content.find_first_of('.') + 4;
					}
					else
					{
						aggcellLength = content.length();
					}
					whitespaces = cellsMaxLength[y] - aggcellLength;
				}
				else
				{
					content = table[i][y];
					whitespaces = cellsMaxLength[y] - table[i][y].length();
				}
			}
			if (tableAlign[i][y] == "left")
			{
				if (aggregate && tableAggregate[i][y].first != "#NAME?" && tableAggregate[i][y].first != "#CYCLE!")
				{
					std::cout << std::fixed << std::setprecision(3) << std::stod(content);
				}
				else
				{
					std::cout << content;
				}
				for (size_t i = 0; i < whitespaces; i++)
				{
					std::cout << " ";
				}
			}
			else
			{
				for (size_t i = 0; i < whitespaces; i++)
				{
					std::cout << " ";
				}
				if (aggregate && tableAggregate[i][y].first != "#NAME?" && tableAggregate[i][y].first != "#CYCLE!")
				{
					std::cout << std::fixed << std::setprecision(3) << std::stod(content);
				}
				else
				{
					std::cout << content;
				}
			}

			std::cout << columnSeparator;
		}
		std::cout << std::endl;
		for (size_t i = 0; i < rowSeparatorLength; i++) // Sorelválasztó kiíratása
		{
			std::cout << "-";
		}
		std::cout << std::endl;
	}
	delete[] cellsMaxLength;
}


void Spreadsheet::writeTableToFile(std::string name, char separator) {
	std::ofstream myfile;
	myfile.open(name);
	if (myfile.is_open())
	{
		for (size_t i = 0; i < rowCounter; i++)
		{
			for (size_t j = 0; j < columnCounter; j++)
			{
				if (j == columnCounter - 1) // Ha az utolsó elem a sorba akkor nem kell szeparátor
				{
					myfile << table[i][j];
				}
				else
				{
					myfile << table[i][j] << separator;
				}
			}
			myfile << "\n";
		}
		myfile.close();
		std::cout << "Sikeresen megtortent a(z) " << name << " fajlba iras!" << std::endl;
	}
	else
	{
		std::cout << "Nem sikerult megnyitni a fajlt!" << std::endl;
	}
}

size_t Spreadsheet::getColumnCounter() {
	return columnCounter;
}

size_t Spreadsheet::getRowCounter() {
	return rowCounter;
}

//! Ez a függvény rendezi az oszlopokat növekvő sorrendbe
/*!
	\param table a táblázat
	\param by oszlopindex
*/
void sortByColumnAsc(std::vector<std::vector<std::string>>& table, int& by) {
	const std::regex is_it_number(R"(^([+-]?(?:[\d]+\.?|[\d]*\.[\d]+))$)");
	std::sort(table.begin(), table.end(), [&by, &is_it_number](const std::vector<std::string>& item1, const std::vector<std::string>& item2)
		{
			if (item1[by].length() != 0 && std::regex_match(item1[by], is_it_number)) {
				try {
					double num1 = std::stod(item1[by]);
					if (item2[by].length() != 0 && std::regex_match(item2[by], is_it_number)) {
						try {
							double num2 = std::stod(item2[by]);
							return num1 < num2;
						}
						catch (const std::exception&) {}
					}
					else {
						return false;
					}
				}
				catch (const std::exception&) {}
			}
			else {
				if (item2[by].length() != 0 && std::regex_match(item2[by], is_it_number)) {
					try {
						std::stod(item2[by]);
						return true;
					}
					catch (const std::exception&) {}
				}
				else {
					return item1[by] < item2[by];
				}
			}
			return item1[by] < item2[by];
		});
}

//! Ez a függvény rendezi az oszlopokat csökkenő sorrendbe
/*!
	\param table a táblázat
	\param by oszlopindex
*/
void sortByColumnDesc(std::vector<std::vector<std::string>>& table, int& by) {
	const std::regex is_it_number(R"(^([+-]?(?:[\d]+\.?|[\d]*\.[\d]+))$)");
	std::sort(table.begin(), table.end(), [&by, &is_it_number](const std::vector<std::string>& item1, const std::vector<std::string>& item2) {
		if (item1[by].length() != 0 && std::regex_match(item1[by], is_it_number)) {
			try {
				double num1 = std::stod(item1[by]);
				if (item2[by].length() != 0 && std::regex_match(item2[by], is_it_number)) {
					try {
						double num2 = std::stod(item2[by]);
						return num1 > num2;
					}
					catch (const std::exception&) {}
				}
				else {
					return true;
				}
			}
			catch (const std::exception&) {}
		}
		else {
			if (item2[by].length() != 0 && std::regex_match(item2[by], is_it_number)) {
				try {
					std::stod(item2[by]);
					return false;
				}
				catch (const std::exception&) {}
			}
			else {
				return item1[by] > item2[by];
			}
		}
		return item1[by] < item2[by];
		});
}

//! A mátrix transzponálása a sorok rendezése miatt
/*!
	\param table a táblázat
	\return a transzponált táblázat
*/
std::vector< std::vector<std::string> > transpose(std::vector< std::vector<std::string> >& table) {
	std::vector< std::vector<std::string>> transposed(table[0].size(), std::vector<std::string>());
	for (size_t i = 0; i < table.size(); ++i)
	{
		for (size_t j = 0; j < table[0].size(); ++j)
		{
			transposed[j].push_back(table[i][j]);
		}
	}
	return transposed;
}

void Spreadsheet::sortBy(int by, bool is_it_row, std::string direction) {
	for (size_t i = 0; i < rowCounter; i++)
	{
		for (size_t j = 0; j < columnCounter; j++)
		{
			tableAggregate[i][j].first = emptyCell;
		}
	}
	if (is_it_row)
	{
		table = transpose(table);
		if (direction == "asc")
		{
			sortByColumnAsc(table, by);
		}
		else
		{
			sortByColumnDesc(table, by);
		}
		table = transpose(table);
	}
	else
	{
		if (direction == "asc")
		{
			sortByColumnAsc(table, by);
		}
		else
		{
			sortByColumnDesc(table, by);
		}
	}
};

int Spreadsheet::xcharCheck(std::string xPar) {
	char x = xPar[0];
	x = toupper(x);
	int xNum = int(x) - char('A');
	if (xNum + 1 > (int)columnCounter)
	{
		return -1;
	}
	else
	{
		return xNum;
	}
}

int Spreadsheet::ynumberCheck(std::string yPar) {
	int y = 0;
	try {
		y = std::stoi(yPar);
	}
	catch (std::invalid_argument&) {
		return -2;
	}
	if (y > (int)rowCounter || y < 1)
	{
		return -1;
	}
	else
	{
		return y - 1;
	}
}

void Spreadsheet::orderRangeParams(int& a, int& b) {
	if (b < a)
	{
		int c = a;
		a = b;
		b = c;
	}
}

std::string Spreadsheet::getName() {
	return spreadsheetname;
}

void Spreadsheet::setName(std::string tableName) {
	spreadsheetname = tableName;
}

std::string Spreadsheet::get(size_t row, size_t column) {
	std::string content = "";
	if (row >= 0 && row <= table.size() && column >= 0 && column <= table[0].size())
	{
		if (tableAggregate[row][column].first != emptyCell)
		{
			content = tableAggregate[row][column].first;
		}
		else
		{
			content = table[row][column];
		}
	}
	return content;

}

std::stringstream headString() {
	std::stringstream h;
	h << "<head> <meta charset=\"utf-8\"><style>" <<
		" body {font: 10px sans-serif;}" <<
		".axis path," <<
		".axis line {fill: none; stroke: #000;shape-rendering: crispEdges;}" <<
		".bar { fill: steelblue;}" <<
		"</style></head>";
	return h;
}
void Spreadsheet::barChart(int x, int y, int m, int n, std::string filename) {
	const std::regex is_it_pure_number(R"(^([+-]?(?:[\d]+\.?|[\d]*\.[\d]+))$)");
	std::vector<std::vector<string>> chart;
	std::string cellValue;
	int j = y;
	for (; y <= n; y++) // Rangen bellüli értékek egy külön vektorba gyűjtése
	{
		std::vector<string> chartrow;
		int a = x;
		for (; a <= m; a++)
		{

			if (std::regex_match(table[y][a], is_it_pure_number) || j == y || a == x)
			{
				cellValue = table[y][a];
			}
			else
			{
				cellValue = "0";
			}

			chartrow.push_back(cellValue);
		}
		chart.push_back(chartrow);
	}


	BarchartMaker Barchart(chart, filename);
	Barchart.createSVG();
}


