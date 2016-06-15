#include<stdio.h>
#include<stdlib.h>
#include "treez.h"
#include<string.h>


/*
autor Tomasz Jureczko
projekt : LZ78
Funkcje wlasciwe - kompresji i dekompresji metoda LZ78
*/


void dekompresja(char * nazwapliku, char * theoutfile, int verbose, int metodaslownika)
{
	struct theTreeelem *slownik;
	slownik = NULL;
	slownik = (theTreeelem*)malloc(sizeof(theTreeelem));
	slownik->kod = 0;
	InitializeTreeEkem(slownik);
	int freeelem;	

	if (verbose == 1)
		printf("Otwarcie  plików\n");
	struct theTreeelem *checkit; //element tymczasowy
	struct theTreeelem *listaelementow[256] = { NULL };
	

	int odczytane;
	int licznik;
	unsigned char nr;
	unsigned char *tekstfromtree = 0;

	//zmienne s³u¿¹ce do wyliczenia "stopnia" kompresji
	unsigned long int bytesreaden = 0;
	unsigned long int byteswritten = 0;


	//plik do  odczytu
	FILE *plik = fopen(nazwapliku, "rb");    // otworzenie pliku do odczytu koniecznie binarnie
	if (!plik)
	{
		printf("Brak pliku wejsciowego %s", nazwapliku);
		return; //wyskok - blad;
	}

	//plik do zapisu
	FILE *outfile = fopen(theoutfile, "wb");//plik wynikowy
	if (!outfile)
	{
		printf("Brak mozliwosci zapisu pliku wyjsciowego %s\n", theoutfile);
		fclose(plik);//ten juz otwarty
		return; //wyskok - blad;
	}

	printf("Metoda s³ownikowa %d", metodaslownika);
	listaelementow[0] = slownik;
	odczytane = fread(&nr, 1, 1, plik);
	bytesreaden++;
	while (odczytane > 0)
	{
		freeelem = findFirstFree(listaelementow);//znajdz pierwszy wolny element s³ownika

		if (freeelem == -1)
		{
			if (verbose == 1)
				printf("Czyszczenie slownika\n");
			
			if (metodaslownika == 0)
			{
				clearTree(slownik);
				for (licznik = 1; licznik < 256; licznik++)
				{
					listaelementow[licznik] = NULL;
				}
				freeelem = findFirstFree(listaelementow);
			}
			else
			{
				clearUnusedElements(slownik, listaelementow, metodaslownika);//testy wskazuja ze uzycie tutaj jedynki daje najlepsze rezultaty
				freeelem = findFirstFree(listaelementow);
			}			
		}


		if (nr == 0)
		{
			odczytane = fread(&nr, 1, 1, plik);
			if (odczytane == 0)
			{ 
				printf("Blad struktury pliku 1 \n");
				fclose(plik);
				fclose(outfile);
				return;
			}

			bytesreaden++;
			fwrite(&nr, 1, 1, outfile);
			byteswritten++;

			checkit = getKod(slownik, &nr, freeelem);
			if (verbose == 1)
			{
				printf("'%c'%d \n", nr, freeelem);
			}
			listaelementow[checkit->kod] = checkit;
		}
		else
		{
			checkit = listaelementow[nr];
			if (checkit==NULL)
			{
				printf("Blad struktury pliku 1 \n");
				fclose(plik);
				fclose(outfile);
				return;
			}
			tekstfromtree = textFromTree(checkit, 1);
			byteswritten += strlen((char*)tekstfromtree);
			if (verbose == 1)
			{
				printf("%d\ %s \t", nr, tekstfromtree);
			}
			fwrite(&tekstfromtree[0], strlen((char*)tekstfromtree), 1, outfile);
				
			odczytane = fread(&nr, 1, 1, plik);
			if (odczytane == 0)
			{
				printf("Blad struktury pliku 1 \n");
				fclose(plik);
				fclose(outfile);
				return;
			}
			bytesreaden++;
			checkit = getKod(checkit, &nr, freeelem );
			if (verbose == 1)			
			{
				printf("'%c' %d \n", nr, freeelem);
			}

			listaelementow[checkit->kod] = checkit;
			//checkit->theparent->IloscUzyc++;
			fwrite(&nr, 1, 1, outfile);
			byteswritten++;
			//free(tekstfromtree);
			free(tekstfromtree);
		}
		odczytane = fread(&nr, 1, 1, plik);
		if (odczytane > 0)
		{
			bytesreaden++;
		}
	}
	fclose(plik);
	fclose(outfile);

	clearTree(slownik);
	free(slownik);
	printf("Znakow odczytanych %d , znakow zapisanych %d ", bytesreaden, byteswritten);
	printf("\nWspolczynnik kompresji %f ", (float)bytesreaden / (float)byteswritten);

	return;
}


//-c -f lz78.cpp -o lz78.lz78
void kompresja(char * nazwapliku, char * theoutfile, int verbose, int metodaslownika)
{
	struct theTreeelem *slownik;
	slownik = NULL;
	slownik = (theTreeelem*)malloc(sizeof(theTreeelem));
	slownik->kod = 0;
	
	InitializeTreeEkem(slownik);

	struct theTreeelem *listaelementow[256] = { NULL };
	listaelementow[0] = slownik;

	//zmienne s³u¿¹ce do wyliczenia "stopnia" kompresji
	unsigned long int bytesreaden = 0;
	unsigned long int byteswritten = 0;

	struct theTreeelem *checkit; //element tymczasowy

	int odczytane;
	int licznik;
	unsigned char nr;
	unsigned char prevchar;
	int freeelem;	

	if (verbose == 1)
		printf("Otwarcie  plików\n");
	
	//plik do odczytu
	FILE *plik = fopen(nazwapliku, "rb");    // otworzenie pliku do odczytu
	if (!plik)
	{
		printf("Brak pliku wejsciowego %s\n", nazwapliku);
		return; //wyskok - blad;
	}

	//plik do zapisu
	FILE *outfile = fopen(theoutfile, "wb");//plik wynikowy koniecznie binarnie
	if (!outfile)
	{ 
		printf("Brak mozliwosci zapisu pliku wyjsciowego %s\n",theoutfile);
		fclose(plik);
		return; 
	}
	printf("Metoda slownikowa %d\n", metodaslownika);

	odczytane = fread(&nr, 1, 1, plik);
	while (odczytane > 0)
	{
		bytesreaden++;
		freeelem = findFirstFree(listaelementow);


		if (freeelem == -1)
		{
			if (verbose == 1)
				printf("Czyszczenie slownika\n");
			/*else
				printf("\n");
				*/

			if (metodaslownika == 0)
			{
				clearTree(slownik);
				for (licznik = 1; licznik < 256; licznik++)
				{
					listaelementow[licznik] = NULL;
				}
				freeelem = findFirstFree(listaelementow);

			}
			else
			{
				clearUnusedElements(slownik, listaelementow, metodaslownika);//testy wskazuja ze uzycie tutaj jedynki daje najlepsze rezultaty
				freeelem = findFirstFree(listaelementow);
			}
			
		}

		//sprawdzenie znaku w slowniku
		checkit = getKod(slownik, &nr, freeelem);
		
		while (checkit->kod != freeelem)
		{
			prevchar = nr;
			//prev = checkit;
			odczytane = fread(&nr, 1, 1, plik);
			if (verbose == 1)
				printf("'%c'", nr);

			if (odczytane)
			{
				bytesreaden++;
				checkit = getKod(checkit, &nr, freeelem);
			}
			else
			{
				nr = prevchar;//w zasadzie zbêdne - fread zostawia znak jesli nie odczyta , ale for sure
				break;
				//checkit zostaje jaki by³
			}
		}
		if (verbose == 1)
		{
			printf("\t %d\n", freeelem);
		}
		fwrite(&checkit->theparent->kod, 1, 1, outfile);
		byteswritten++;
		fwrite(&nr, 1, 1, outfile);
		byteswritten++;


		listaelementow[checkit->kod] = checkit;

		odczytane = fread(&nr, 1, 1, plik);
		if (verbose == 1)
			printf("'%c'", nr);

	}
	fclose(plik);
	fclose(outfile);

	clearTree(slownik);
	free(slownik);

	printf("Znakow odczytanych %d , znakow zapisanych %d ", bytesreaden, byteswritten);
	printf("\nWspolczynnik kompresji %f ", (float)byteswritten / (float)bytesreaden);

	return;
}