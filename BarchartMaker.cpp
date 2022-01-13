/**
*
* \author Zöllei Alex, Varga Szabolcs, Szikonya Attila, Bogdán Ádám
*
* \date 2021/11/19
*
* Ez a barchartaker.cpp fájl írja a a BarchartMaker osztály függvényeit. 
*
* Created on 2021/11/19
*/
#include "BarchartMaker.h"
#include <fstream>
using namespace std;

BarchartMaker::BarchartMaker(std::vector<std::vector<std::string>> fromRange, std::string file) {
	

	for (size_t i = 0; i < fromRange.size(); i++)
	{
		std::vector<string> chartrow;
		bool pushed = false;
		for (size_t j = 0; j < fromRange[i].size(); j++)
		{
			if (i==0 && j != 0)
			{
				groupElements.push_back(fromRange[i][j]);
			}
			else if(j==0 && i != 0)
			{
				xAxis.push_back(fromRange[i][j]);
			}
			else if (j != 0 && i != 0)
			{
				chartrow.push_back(fromRange[i][j]);
				pushed = true;
			}
		}
		if (pushed)
		{
			values.push_back(chartrow);
		}

	}
	//Próbára

	/*xAxis = { "A", "B", "C" };
	groupElements = { "alma", "körte", "cseresznye", "citrom" };
	values = {
		{"2", "5", "6", "12"},
		{"60", "2", "3", "98"},
		{"10", "2", "7", "69"}
	};*/
	filename = file;

}



std::stringstream BarchartMaker::headString()
{
	std::stringstream h;
	h << "<head><title>SVG chart</title>"
		<< "<script type = \"text/javascript\" src = \"https://www.gstatic.com/charts/loader.js\">"
		<< "</script><script type = \"text/javascript\">google.charts.load('current', {packages: ['corechart']});</script>"
		<< "</head>";
	return h;

}


void BarchartMaker::createSVG() {
	ofstream fout;
	fout.open(filename + ".html");
	if (fout.fail())
	{
		cout << "Nem sikerült megnyitni a fájlt";
	}
	std::stringstream head = headString(); //Head és stílusok beállítása
	fout << head.str();
	//body kiíratása
	fout << " <body><div id = \"container\" style = \"width: 550px; height: 400px; margin: 0 auto\"></div>";
	fout << "<script language = \"JavaScript\">function drawChart() {";
	fout << "";
	fout << "var data = google.visualization.arrayToDataTable([";
	fout << "[''";
	for (size_t i = 0; i < groupElements.size(); i++)
	{
		fout << ", '" << groupElements[i] << "'";
	}
	fout << "]";
	for (size_t i = 0; i < xAxis.size(); i++)
	{
		fout << ", ['" << xAxis[i] << "' ";
		for (size_t j = 0; j < values[i].size(); j++)
		{
			fout << ", " << values[i][j];
		}
		fout << "]";
	}
	fout << "]);";
	fout << "var options = { vAxes: {0: {  baseline:  0 }}};";
	fout << "var chart = new google.visualization.ColumnChart(document.getElementById('container'));";
	fout << "chart.draw(data, options);}";
	fout << "google.charts.setOnLoadCallback(drawChart);</script> </body></html>";
	fout.close();


}

