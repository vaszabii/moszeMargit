
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
    Spreadsheet *t;

    void SetUp() override
    {
        t = new Spreadsheet();
        t->addRow(2);
        t->addColumn(2);
        t->editCell(0, 0, "2");
        t->editCell(1, 0, "2");
    }

    void TearDown() override
    {
        delete t;
    }

public:
    void check(std::vector<std::vector<std::string>> expected)
    {
        for (unsigned row = 0; row < expected.size(); row++)
        {
            for (unsigned col = 0; col < expected[row].size(); col++)
            {
                EXPECT_EQ(expected[row][col], t->get(row, col));
            }
        }
    }
};

TEST_F(AggrTest, TestSum)
{
    t->editCell(2, 0, "=SUM(A1:B1)");
    t->updateAggregateTable();
    std::vector<std::vector<std::string>> expected = {
        {"2", "2", "4.000000"},
        {"", "", ""},
        {"", "", ""}};
    
    check(expected);
}
TEST_F(AggrTest, TestCycle)
{
    
    t->editCell(2, 0, "=SUM(A1:C1)"); //CYLCLE próba
    t->updateAggregateTable();
    std::vector<std::vector<std::string>> expected = {
        {"2", "2", "#CYCLE!"},
        {"", "", ""},
        {"", "", ""}};

    check(expected);
}

TEST_F(AggrTest, TestOverRange)
{    
    t->editCell(2, 0, "=SUM(A5:C1)"); //NAME? próba
    t->updateAggregateTable();
    std::vector<std::vector<std::string>> expected = {
        {"2", "2", "#NAME?"},
        {"", "", ""},
        {"", "", ""}};

    check(expected);
}
TEST_F(AggrTest, TestAVG)
{    
    t->editCell(1, 0, "6");
    t->editCell(2, 0, "=AVG(A1:B1)"); 
    t->updateAggregateTable();
    std::vector<std::vector<std::string>> expected = {
        {"2", "6", "4.000000"},
        {"", "", ""},
        {"", "", ""}};

    check(expected);
}
TEST_F(AggrTest, TestMAX)
{    
    t->editCell(1, 0, "5");
    t->editCell(2, 0, "=MAX(A1:B1)"); 
    t->updateAggregateTable();
    std::vector<std::vector<std::string>> expected = {
        {"2", "5", "5.000000"},
        {"", "", ""},
        {"", "", ""}};

    check(expected);
}
TEST_F(AggrTest, TestMIN)
{    
    t->editCell(1, 0, "5");
    t->editCell(2, 0, "=MIN(A1:B1)"); 
    t->updateAggregateTable();
    std::vector<std::vector<std::string>> expected = {
        {"2", "5", "2.000000"},
        {"", "", ""},
        {"", "", ""}};

    check(expected);
}
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}