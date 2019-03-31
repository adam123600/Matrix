#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "matrix.h"
//#include <time.h>


//#define DEBUG

/////////////////////////////////////////////////
// funkcje prywatne
void TransMatrix( double** pTab, int nDim );
void ComplMatrix( double** pTabD, double** pTab, int nDim );
void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim );
////////////////////////


int CreateMatrix( double*** pTab, int nSize ) // pobierzmy sobie rozmiar z wejscia w mainie,, wymiar jeden bo wyznacznik mozna liczyc tylko z macierzy kwadratowej
{

	*pTab = (double**)malloc( sizeof( double ) * nSize );
	if( pTab )
	{
		double** ppTab = *pTab;
		for( int i = 0; i < nSize; i++ )
		{
			*ppTab = (double*)malloc( sizeof( double ) * nSize );
			if( *ppTab )
				memset( *ppTab++, 0, sizeof( double ) * nSize );
			else return 0;
			
		}
	}
	else return 0;

	return 1;
}

void DeleteMatrix( double*** pTab, int nSize )
{
	double** ppTab = *pTab;

	for( int i = 0; i < nSize; i++ )
	{
		free( *ppTab++ );
	}
	free( *pTab );

	*pTab = NULL;
	//ppTab = NULL;
}

void InverseMatrix( double** pInv, double** pTab, int nSize, double det )	// odwraca tablice, czyli A ^ ( -1 )
{
	
	ComplMatrix( pInv, pTab, nSize );// tworzy macierz doplenien

#ifdef DEBUG
	printf( "\nComplMatrix, Macierz Dopelnien:\n" );
	PrintMatrix( pInv, nSize );
	printf( "\n" );
#endif


	TransMatrix( pInv, nSize ); // transponuje te macierz

#ifdef DEBUG
	printf( "\nTransMatrix, Transponowanie:\n" );
	PrintMatrix( pInv, nSize );
	printf( "\n" );
#endif


	for( int i = 0; i < nSize; i++ )
	{
		double* ppInv = *pInv++;
		for( int j = 0; j < nSize; j++ )
		{
			*ppInv++ /= det; // dzieli kazdy element z pInv przez det i ponownie wrzuca do pInv
		}
	}





}

double Det( double** pTab, int nSize ) // oblicza wyznaniczk laplasem wzgledem 0 kolumny i wiersza, przy 2x2 liczmy na krzyz, przy 2x2 mozna uzywac indexowania
{
	if( nSize == 1 ) return **pTab;
	if( nSize == 2 ) return **pTab * pTab[1][1] - pTab[1][0] * pTab[0][1];

	double det = 0; // wartosc wyznacznika

	double** pomocniczaTab = NULL; // macierz do wpisywania macierzy po skresleniu wiersza i kolumny czyli jak w 6x6 skreslam to ta macierz bedzie 5x5
	if( !CreateMatrix( &pomocniczaTab, nSize - 1 ) )
	{
		perror( "ERROR, BLAD ALOKACJI PAMIECI- DET" );
		return 0;
	}

	int minus = 1;  // 1 do potegi wiersz + kolumna /// bedzie przestawiane z 1 na -1	
	double* ppTab = *pTab; // pomocniczy do poruszania sie po kolumnach

	for( int i = 0; i < nSize; i++, minus = -minus )
	{
		Complement( pomocniczaTab, pTab, 0, i, nSize );
		det += *( ppTab++ ) * minus * Det( pomocniczaTab, nSize - 1 );

#ifdef DEBUG
		printf( "\nComplement, Macierz po wycieciu:\n" );
		PrintMatrix( pomocniczaTab, nSize - 1 );
		printf( "\n" );
#endif
	}
	DeleteMatrix( &pomocniczaTab, nSize - 1 );
	return det;
}

void LayoutEqu( double** pInv, double* pB, double* pRes, int nSize )
//rozwiazuje uklad rownan, pB - wyrazy wolne, pInv tablica odwrocona!!!, pRes tablica z wynikami
{
	for( int i = 0; i < nSize; i++ )
	{
		double* ppInv = *pInv++; // wsk do odwroconej aby zejsc wiersz w dol
		double* ppB = pB; // do powrotu na poczatek wyrazow wolnych

		for( int j = 0; j < nSize; j++ )
			*pRes += *ppInv++ * *ppB++; // mnozenie odwroconej przez wolne i wstawiaj do pRes

		pRes++;
	}

}

void PrintMatrix( double** pTab, int nSize )
{
	/*
	for( int i = 0; i < nSize; i++ )
	{
		for( int j = 0; j < nSize; j++ )
			printf( "%.0lf ", *( *( pTab + i ) + j ) );
		printf( "\n" );
	}
	*/
	double* ppTab = *pTab; // pomocniczy do przeskakiwania po **

	for( int i = 0; i < nSize; i++ )
	{
		ppTab = *pTab++; // przesuniecie na kolumnie o jeden w prawo
		for( int j = 0; j < nSize; j++ )
		{
			printf( "%.3lf ", *ppTab++ ); // przesuwanie po kolumnie o jeden w dol
		}
		printf( "\n" );
	}

}

void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim ) // tworzy macierz po wycieciu wiersza i kolumny 
{
	for( int i = 0; i < nDim; i++ )
	{
		double* ppTabI = *pTabI++; // pomocniczy aby miec mozliwosc zejscia 
		if( i == nRow ) // jesli skreslany wiersz == i to continue 
			continue;

		double* ppTabO = *pTabO++; // pomocniczy aby miec mozliwosc zejscia

		for( int j = 0; j < nDim; j++, ppTabI++ )
		{
			if( j == nCol ) // gdy skreslana kolumna == j to continue 
				continue;
			*ppTabO++ = *ppTabI;
		}
	}
}

void ComplMatrix( double** pTabD, double** pTab, int nDim ) // liczy macierz dopelnien
{
	int minus = 1; // do zmieniania z dodatniej na ujemna

	double** ppTab = pTab;

	double** pomocniczaM = NULL; // macierz sluzaca aby wpisac w nia po skresleniu wiersza i kolumny przez funkcje Complement
	if( !CreateMatrix( &pomocniczaM, nDim - 1 ) )
	{
		perror( "ERROR, BLAD ALOKACJI PAMIECI- COMPLMATRIX" );
		return;
	}

	for( int i = 0; i < nDim; i++ ) 
	{
		*ppTab++;
		int minus = ( nDim % 2 )? -1 : 1; // do zmieniania z dodatniej na ujemna
		//int minus  = 1;
		double* ppTabD = *pTabD++;
	
		for( int j = 0; j < nDim; j++) 
		{
			Complement( pomocniczaM, pTab, i, j, nDim );
			*ppTabD++ = minus * Det( pomocniczaM, nDim - 1 );
			minus = -minus;
		}
		//if (!(nDim % 2)) minus *= -1;
	}
	DeleteMatrix( &pomocniczaM, nDim - 1 );
}

/*
void TransMatrix( double** pTab, int nDim )
{
	for( int i = 0; i < nDim-1; i++ )
	{
		for( int j = i+1; j < nDim; j++ )
		{
			double pomocnicza = pTab[i][j]; // do zapamietania aby nie tracic zamiennej 
			pTab[i][j] = pTab[j][i];
			pTab[j][i] = pomocnicza;
		}
	}
}
*/

void TransMatrix( double** pTab, int nDim )
{
	for( int i = 0; i < nDim - 1; i++ )
	{
		double* v = *( pTab + i ) + i + 1;
		//double* v = *pTab++ + i + 1;

		for( int j = i + 1; j < nDim ; j++ )
		{
			double tmp = *v;
			*v++ = pTab[j][i];
			pTab[j][i] = tmp;
		}
	}
}

/*
//// Generator do generowania losowych liczb do macierzy;
void GeneratorLosowy( double** pTab, int nSize )
{
	srand( time( NULL ) );
	double* ppTab = *pTab;

	for( int i = 0; i < nSize; i++ )
	{
		ppTab = *pTab++;
		for( int j = 0; j < nSize; j++ )
		{
			*( ppTab++ ) = rand() % 10;
		}
	}
}

*/