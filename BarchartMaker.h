/**
* \class BarchartMaker
*
* \author Zöllei Alex, Varga Szabolcs, Szikonya Attila, Bogdán Ádám
*
* \date 2021/11/19
*
* \brief BarchartMaker class in BarchartMaker.h
*
* Ez az osztály hivatott a csoportosított diagramm SVG létrehozására, ami tartalmazza az SVG elkészítésért felelős függvényeket és funkciókat.
*
* Created on 2021/11/19
*/

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream> 

class BarchartMaker {
private:
	std::vector<std::string> xAxis;///< Az oszlop diagramm x tengelye
	std::vector<std::string> groupElements;///< Az oszlop diagramm csoportjai
	std::vector<std::vector<std::string>> values;///< Az oszlop diagramm értékei
	std::string filename;///< A html kimenet fájlneve

public:
	//! Paraméterezett konstruktor
	/*!
	\param fromRange a táblázatbol kapott tartomány
	\param filename a html kimenet fájlneve
	*/
	BarchartMaker(std::vector<std::vector<std::string>> fromRange, std::string filename);
	//! A HTML kimenet fejlécének 
	/*!
	\return a html fejléc
	*/
	std::stringstream headString();
	//! A csoportosított oszlopdiagram elkészítése
	/*!
	*/
	void createSVG();
	

};