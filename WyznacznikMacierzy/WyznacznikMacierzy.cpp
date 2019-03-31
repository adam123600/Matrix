// WyznacznikMacierzy.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "matrix.h"
#include <stdio.h>
#include <iostream> // fabs

void ReadData( FILE* fin, double**pMatrix, double* b, int nDim ); // Funkcja czytajaca dane z txt

#define DEBUG

int main(int argc, char** argv)
{
	if( argc != 2 ) // do odpowiedniego wczytywania pliku ( jesli zle to nie dziala program )
	{
		printf( "Aby wlaczyc wpisz: WyznacznikMacierzy.exe <nazwa_pliku>\n" );
		return 0;
	}

	
	//////////////////////// WLACZENIE PLIKU //////////////////////
	FILE* plik = fopen( argv[1], "r" ); // wlaczenie pliku na odczyt, "r" - odczyt
	if( !plik )
	{
		perror( "ERROR, BLAD WLACZENIA PLIKU" );
		return 0;
	}
	///////////////////////////////////////////////////////////////////


	////////////// CZYTANIE ROZMIARU
	int nSize = 0; // zmienna do wczytania rozmiaru
	fscanf( plik, "%d", &nSize);
	if( nSize < 1 )
	{
		printf( "Niepoprawny rozmiar macierzy." );
		return 0;
	}
	///////////////////////////////////////////////////////////




	////////////// TWORZENIE MACIERZY
	double** Matrix = NULL; // macierz z danych
	if( !CreateMatrix( &Matrix, nSize ) )
	{
		perror( "ERROR, BLAD ALOKACJI PAMIECI DLA MATRIX- MACIERZ DANYCH" );
		return 0;
	}

	double* pB = (double*)malloc( sizeof( double )*nSize ); // tablica na wyrazy wolne
	if( !pB )
	{
		perror( "ERROR, BLAD ALOKACJI PAMIECI DLA PB- TABLICA NA WYRAZY WOLNE" );
		return 0;
	}
	memset( pB, 0, sizeof( double )*nSize );



	//////////////////////   CZYTANIE MACIERZY
	//////// ZROBIC FUNKCJE CZYTAJACA
/*	for( int i = 0; i < nSize; i++ )
	{
		double* pMatrix = *Matrix++;
		for( int j = 0; j < nSize; j++ )
		{
			fscanf( plik, "%lf", pMatrix++ );
		}
		fscanf( plik, "%lf", pB++ );
	}

	pB -= nSize; // Cofniecie wskaznikow na poczatek
	Matrix -= nSize;
*/

	ReadData( plik, Matrix, pB, nSize ); // CZYTANIE MACIERZY I WYRAZOW WOLNYCH


	double* Result = (double*)malloc( sizeof( double )*nSize );// TABLICA NA WYNIKI
	if( !Result )
	{
		perror( "ERROR, BLAD ALOKACJI PAMIECI DLA RESULT- TABLICA NA WYNIKI" );
		return 0;
	}
	memset( Result, 0, sizeof( double )*nSize );

	double det = Det( Matrix, nSize ); // det = obliczenie wyznacznika z Matrix
	if( fabs( det ) <= 1e-14 )
	{
		printf( "Wyznacznik rowny 0, nie da sie liczyc dalej." );
		return 0;
	}

	double** pInv = NULL; // macierz do odwrocenia 
	if( !CreateMatrix( &pInv, nSize ) )
	{
		perror( "ERROR, BLAD ALOKACJI PAMIECI DLA pInv- TABLICA ODWROCONA" );
		return 0;
	}


	InverseMatrix(pInv , Matrix, nSize, det ); // odwracanie macierzy

	LayoutEqu( pInv, pB, Result, nSize ); // rozwiazywanie ukladu rownan

	printf( "Wynik:\n" );
	for( int i = 0; i < nSize; i++ )
		printf( "%.3lf ", *Result++ );
	printf( "\n\n" );



#ifdef DEBUG

	printf( "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ " );
	printf( "\nWARUNKOWA KOMPILACJA\nWYDRUKI:\n" );
	// Macierz
	printf( "\nMacierz wejsciowa:\n" );
	PrintMatrix( Matrix, nSize );
	printf( "\n" );

	// Macierz Transponowana
	// FUNKCJA PRYWATNA!!!
//	printf( "Transponowana:\n" );
//	TransMatrix( Matrix, nSize );
//	PrintMatrix( Matrix, nSize );
	//printf( "\n\n" );

	// Rozmiar macierzy
	printf( "\nRozmiar macierzy: %d\n\n", nSize );


	// Macierz odwrotna
	printf( "Macierz odwrotna: \n" );
	PrintMatrix( pInv, nSize );

	// Wyrazy wolne
	double* ppB = pB;
	printf( "\n\nWyrazy wolne:\n" );
	for( int i = 0; i < nSize; i++ )
		printf( "%lf ", *ppB++ );
	printf( "\n\n" );


	// Wyznacznik
	printf( "\nWyznacznik:\n" );
	printf( "%.3lf\n\n", det );


	// Wyniki
	printf( "\nWyniki:\n" );
	Result -= nSize;
	for( int i = 0; i < nSize; i++ )
		printf( "%.3lf ", *Result++ );
	printf( "\n\n" );

	printf( "KONIEC WARUNKOWEJ KOMPILACJI\n\n" );
	printf( "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n\n" );
#endif


	DeleteMatrix( &Matrix, nSize );
	DeleteMatrix( &pInv, nSize );
	Matrix = NULL;
	pInv = NULL;

	Result -= nSize; // cofniecie sie do poczatku aby usunac
	
	free( Result );
	free( pB );
	Result = NULL;
	pB = NULL;
	

	fclose( plik );
	plik = NULL;

	return 0;
}

void ReadData( FILE* file, double** Matrix, double* pB, int nDim ) // Funkcja czytajaca dane z txt
// file - plik
// Matrix - Macierz wejsciowa
// pB- wyrazy wolne
// nDim- wymiar
{
	for( int i = 0; i < nDim; i++ )
	{
		double *pPom = *Matrix++;
		for( int j = 0; j < nDim; j++ )
		{
			fscanf( file, "%lf", pPom++ );
		}
		fscanf( file, "%lf", pB++ );
	}
}