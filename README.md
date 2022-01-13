[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=453525&assignment_repo_type=GroupAssignmentRepo)
## Modern szoftverfejlesztési eszközök 2021

### 0. Feladat: Ennek a felsorolásnak a felülírása a következő adatokkal:

1. Zöllei Alex, zolleialex
2. Varga Szabolcs, vaszabii
3. Szikonya Attila, szikonyaattila
4. Bogdán Ádám, mikroelek

### 1. Feladat (határidő a PR nyitására: 10.06.)

A parancssoros táblázatkezelő program alábbi funkcióit kell implementálni egy **külön branchen**, majd nyitni belőle egy PR-t (Pull Requestet) a main branchre. Itt reviewernek fel kell kérni @oliverosz oktatót, aki majd vagy értékeli, vagy meghív egy másik oktatót értékelésre.

Elvárt funkciók:

* Tetszőleges méretű, egysoros szöveget tartalmazó cellákból álló táblázat tárolása
  * A program indításakor 1x1 méretű, 1 üres cellával
* Megjelenítés:
  * Indításkor és minden művelet után legyen megjelenítve (stdout-ra kiírva) a táblázat
  * A sorok legyenek számozva, az oszlopok nagy betűkkel jelölve (nem kell 26-nál több oszlopot kezelni)
  * Egy sor oszlopait 1-1 tabulátor karakter válassza el egymástól (hosszabb szövegeknél el fog csúszni, de most még ezzel nem kell foglalkozni)
* Műveletek (stdin-ről beolvasva):
  * `edit XY string` írja be a megadott stringet az XY koordinátájú cellába (pl. A2), a stringben lehetnek szóközök, de sortörés nem
  * `add N rows/cols` adjon hozzá N db új, üres cellákból álló sort/oszlopot a táblázat végéhez
  * `delete X/Y` törölje az X oszlopot vagy Y sort (ha betű, akkor oszlop, ha szám, akkor sor)
  * `insert N rows/cols before/after X/Y` mint az `add`, csak megadott pozícióba szúrjon be
  * `exit` lépjen ki a program
  * Bármelyik műveletnél hibás koordináta vagy paraméter esetén írjon hibaüzenetet (nem kell minden lehetséges hibára felkészíteni, de azért legyen input ellenőrzés)

### További feladatok

A további feladatok a template repó Readme-jében fognak megjelenni: https://github.com/SZE-MoSzE/MoSzE-Template#readme

A feladatok megjelenésekor készülni fog 1-1 PR, így aki értesítést szeretne kapni róla, az kövesse be (watch) a template repót a megfelelő notification beállításokkal.

A feladatokkal kapcsolatos kérdéseket is a template repóban, issue létrehozásával lehet feltenni.
Saját kódra vonatkozó kérdéseket pedig a csapat repójának Feedback PR-jában vagy az adott feladathoz már létrehozott PR-ban.
