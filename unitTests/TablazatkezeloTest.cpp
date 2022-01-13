
#include "../Tablazat.h"
#include <regex>
#include <gtest/gtest.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

class AggrTest : public ::testing::Test
{
protected:
	Spreadsheet* currentTable;
	std::vector<Spreadsheet*> Spreadsheets;
	void SetUp() override
	{
		currentTable = new Spreadsheet("Table");
		Spreadsheets.push_back(currentTable);
		currentTable->addRow(2);
		currentTable->addColumn(2);
		currentTable->editCell(0, 0, "2");
		currentTable->editCell(1, 0, "2");

		Spreadsheets.push_back(new Spreadsheet("TestTable"));
		Spreadsheets[Spreadsheets.size() - 1]->addRow(1);
		Spreadsheets[Spreadsheets.size() - 1]->editCell(0, 0, "a");
		Spreadsheets[Spreadsheets.size() - 1]->editCell(0, 1, "2b");

		Spreadsheets.push_back(new Spreadsheet("ThisIsTable3"));
		Spreadsheets[Spreadsheets.size() - 1]->addRow(4);
		Spreadsheets[Spreadsheets.size() - 1]->addColumn(1);
		Spreadsheets[Spreadsheets.size() - 1]->editCell(0, 0, "00");
		Spreadsheets[Spreadsheets.size() - 1]->editCell(0, 1, "01");
		Spreadsheets[Spreadsheets.size() - 1]->editCell(0, 4, "04");
		Spreadsheets[Spreadsheets.size() - 1]->editCell(1, 3, "13");

		Spreadsheets.push_back(new Spreadsheet("MOSZE"));
		Spreadsheets[Spreadsheets.size() - 1]->editCell(0, 0, "empty");
	}

	void TearDown() override
	{
		for (size_t i = 0; i < Spreadsheets.size(); ++i)
		{
			delete Spreadsheets[i];
		}
		Spreadsheets.clear();
	}

public:
	void check(std::vector<std::vector<std::string>> expected)
	{
		ASSERT_EQ(expected.size(), currentTable->getRowCounter())
			<< "A sorok szama elter ! Elvart : " << expected.size()
			<< " , kapott : " << currentTable->getRowCounter();
		ASSERT_EQ(expected[0].size(), currentTable->getColumnCounter())
			<< "Az oszlopok szama elter ! Elvart : " << expected[0].size()
			<< " , kapott : " << currentTable->getColumnCounter();
		for (unsigned row = 0; row < expected.size(); row++)
		{
			for (unsigned col = 0; col < expected[row].size(); col++)
			{
				EXPECT_EQ(expected[row][col], currentTable->get(row, col));
			}
		}
	}
};
TEST_F(AggrTest, TestSum)
{
	currentTable->editCell(2, 0, "=SUM(A1:B1)");
	currentTable->updateAggregateTable();
	std::vector<std::vector<std::string>> expected = {
		{"2", "2", "4.000000"},
		{"", "", ""},
		{"", "", ""} };

	check(expected);
}
TEST_F(AggrTest, TestCycle)
{

	currentTable->editCell(2, 0, "=SUM(A1:C1)"); //CYLCLE próba
	currentTable->updateAggregateTable();
	std::vector<std::vector<std::string>> expected = {
		{"2", "2", "#CYCLE!"},
		{"", "", ""},
		{"", "", ""} };

	check(expected);
}

TEST_F(AggrTest, TestOverRange)
{
	currentTable->editCell(2, 0, "=SUM(A5:C1)"); //NAME? próba
	currentTable->updateAggregateTable();
	std::vector<std::vector<std::string>> expected = {
		{"2", "2", "#NAME?"},
		{"", "", ""},
		{"", "", ""} };

	check(expected);
}
TEST_F(AggrTest, TestAVG)
{
	currentTable->editCell(1, 0, "6");
	currentTable->editCell(2, 0, "=AVG(A1:B1)");
	currentTable->updateAggregateTable();
	std::vector<std::vector<std::string>> expected = {
		{"2", "6", "4.000000"},
		{"", "", ""},
		{"", "", ""} };

	check(expected);
}
TEST_F(AggrTest, TestMAX)
{
	currentTable->editCell(1, 0, "5");
	currentTable->editCell(2, 0, "=MAX(A1:B1)");
	currentTable->updateAggregateTable();
	std::vector<std::vector<std::string>> expected = {
		{"2", "5", "5.000000"},
		{"", "", ""},
		{"", "", ""} };

	check(expected);
}
TEST_F(AggrTest, TestMIN)
{
	currentTable->editCell(1, 0, "5");
	currentTable->editCell(2, 0, "=MIN(A1:B1)");
	currentTable->updateAggregateTable();
	std::vector<std::vector<std::string>> expected = {
		{"2", "5", "2.000000"},
		{"", "", ""},
		{"", "", ""} };

	check(expected);
}

TEST_F(AggrTest, TestMultiTables)
{
	currentTable = Spreadsheets[1];
	std::vector<std::vector<std::string>> expected = {
	{"a"},
	{"2b"} };
	check(expected);

	currentTable = Spreadsheets[2];
	expected = {
	{"00",""},
	{"01",""},
	{"",""},
	{"","13"},
	{"04",""} };
	check(expected);


	currentTable = Spreadsheets[1];
	expected = {
	{"a"},
	{"2b"} };
	check(expected);

	currentTable = Spreadsheets[3];
	expected = {
	{"empty"} };
	check(expected);
}

TEST_F(AggrTest, TestMultiTableNames)
{
	int expectedsize = 4;
	ASSERT_EQ(expectedsize, Spreadsheets.size());
	std::string expected[4] = { "Table", "TestTable", "ThisIsTable3", "MOSZE" };
	for (size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); i++)
	{
		EXPECT_EQ(expected[i], Spreadsheets[i]->getName());
	}
}
int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}