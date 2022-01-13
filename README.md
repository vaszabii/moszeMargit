[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=453525&assignment_repo_type=GroupAssignmentRepo)
## Spreadsheet - parancssoros táblázatkezelő program

###  A projektet @vaszabii és @zolleialex készítette a Modern Szoftverfejlesztési eszközök c. tárgyra. A projekt során használt technológiák:C++, Google tests, Docker, Doxygen, Git/GitHub, Cmake.

Feladatkiírás: https://github.com/SZE-MoSzE/MoSzE-Template

Program használata, műveletek listája:

* edit XY string : a megadott string beirasa az XY koordinataju cellaba (pl. A2)
* add N rows/cols :  N db uj, ures cellakbol allo sort/oszlopot ad hozza a tablazat vegehez
* delete X/Y : X oszlop vagy Y sor torlese(ha betu, akkor oszlop, ha szam, akkor sor)
* insert N rows/cols before/after X/Y : A megadott pozicioba szur be N darab ures oszlopot/sort
* save fajlnev.csv -sep kar: Megadott nevu CSV fajlba kiirja az aktualis tablazatot. Az ertekek elvalasztasa alapertelmezetten pontosvesszovel tortenik, de az opcionalis [-sep kar] paranccsal megvaltoztathato (pl. save tabla.csv -sep ,)
* align XY left/right vagy align XY:MN left/right : Beallitja, hogy balra/jobbra igazitva legyen a cella(k) tartalma
* clear MN:XY : A megadott cellak tartalmanak torlese range muvelettel
* swap MN XY : A megadott ket cella tartalmanak felcserelese
* sort by X/Y asc/desc : A tablazat sorait a megadott oszlop szerint (vagy oszlopait a megadott sor szerint) novekvo/csokkeno sorrendbe rendezi (alapertelmezetten novekvo)
* barchart XY:MN filename : A megadott tartomanybol eloallit egy csoportositott oszlopdiagramot abrazolo SVG abrat HTML-be agyazva

Talbazatok kezelesere vonatkozo muveletek: 
* new sheet name : letrehoz egy uj tablazatot a megadott nevvel es beallitja aktivnak
* switch N : beallitja az N indexu tablazatot aktivnak
* open filename [-sep ,] : a megadott fajlbol betolti a tablazatot
* close N : bezarja az N indexu tablazatot
* rename N newname : atnevezi az N indexu tablazatot a megadott uj nevre
* exit : kilepes a programbol
