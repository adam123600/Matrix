//#pragma once

int CreateMatrix( double*** pTab, int nSize ); // !!!!!!!funkcja zwraca czy wystapil blad czy nie, 0 error 1 ok,
//w pelni dynamiczna dwuwymiarowa tablica
// ZAWSZE GDY ALOKUJEMY PAMIEC TO JA ZERUJEMY, wyzerowanie memset

void DeleteMatrix( double*** pTab, int nSize ); // zwalnia pamiac, po zwolnieniu wskaznik ma byc wskaznikiem pustym NULL


void InverseMatrix( double** pInv, double** pTab, int nSize, double det ); 
// odwraca tablice, czyli A ^ ( -1 )


double Det( double** pTab, int nSize ); 
// rozwiniecie la plasa, rozwijamy wzgledem zerowego wiersza i zerowej kolumny
// bedzie to funkcja rekurencyjna , gdy dojdziemy 2x2 liczymy na krzyz,
// 3x3 nie wkorzystujemy sarrusa, w 2x2 to jedne miejsce gdzie mozna wykorzystac indeksowanie tablicy, inne miejsca nie mozna uzyc indeksowania,
// podwojnego indexowania poza 2x2 nie wykorzystujemy, uzywamy tylko wskaznikow!


void LayoutEqu( double** pInv, double* pB, double* pRes, int nSize ); 
// rozwiazuje uklad rownan, pB - wyrazy wolne, pInv tablica odwrocna, pRes tablica z wynikami


void PrintMatrix( double** pTab, int nSize ); // wyswietla macierz po przeksztalceniu ( moze kazdym - nie wiem jeszcze ) 

//void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim );

//ma wejsciowe : pTabI, rozmiar nDim, wyciac nRow i nCol
//wyjsciowe : pTabO
//	nalezy zastosowac tutaj contiune dwa razy bo bedzie petla w petli


//	void ComplMatrix( double** pTabD, double** pTab, int nDim );
//oblicza macierz complementarna czyli macierz dopelnien// 
//wejscie : pTab, nDim
//	wyjscie : pTabD


//	void TransMatrix( double** pTab, int nDim );
//transponowanie macierzy
//dzialam na tablicy pTab i na niej samej transponuje
//tutaj trzeba uzyc indeksowania

//void GeneratorLosowy( double** pTab, int nSize ); // losuje liczby do macierzy ( dla mnie do testowania )