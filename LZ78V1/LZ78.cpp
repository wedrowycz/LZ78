#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include "compress.h"

/*
autor Tomasz Jureczko
projekt : LZ78
modul glowny projektu - zawiera odczyt parametrow programu , kontrole skladni oraz wywolanie funkcji
I takie tam
*/


void PrintSkladnia()
{	//wypisanie skladni wywolania
	printf("Program LZ78 - kompresja pliku metoda LZ78 \n");
	printf("Kopyrajt TJ\n");
	printf("Skladnia dla kompresji : lz78 -f <plikwejsciowy> [-o <plik_wyjsciowy>] -c\n");	
	printf("Skladnia dla dekompresji : lz78 -f <plik_wejsciowy> [-o <plik_wyjsciowy>] -d\n");	
	printf("-h lub bez parametrow wyswietla niniejsza pomoc, \n jezeli wystapi parametr -h kompresja i dekompresja nie sa przeprowadzane\n");
	printf("-v (verbose) wyœwietla log dzialania \n ");
	printf("-rn - alternatywna metoda budowy slownika polegajaca na usuwaniu co jakis czas elementow najrzadziej uzywanych - n\n kolejno 1,2,3 oznacza glebokosc czyszczenia slownika ");
}


int main(int argc, char**argv)
{
	char * inputf = NULL;
	char * outputf = NULL;
	char * hparaml=(char*)malloc(4);//help
	char * vparam = (char*)malloc(4);//verbose
	char  kierunek = ' ' ;//c lub d - kompresja lub dekompresja
	char * rparam = (char*)malloc(4);//-r - reclaim , odzyskaj "nieuzywane elementy"
	int rdepth = 0;
	char * komenda;


	komenda = (char*)malloc(255);

	int i; 

	//je¿eli nie podano parametrów
	if (argc < 1)
	{
		PrintSkladnia();
		return 0;
	}

	//sprawdzenie parametrów 
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			komenda = argv[i];
			if (komenda[1] == 'h')
			{
				strcpy(hparaml , "TAK");
				komenda = "";
			}
			else if (komenda[1] == 'v')
			{
				strcpy(vparam , "TAK");
				komenda = "";
			}
			else if ((komenda[1] == 'r') || (komenda[1] == 'R'))
			{   //parametr metody slownikowej
				strcpy( rparam , "TAK");
				if (strlen(komenda) > 1)
				{
					rdepth = atoi(&komenda[2]);
				}

				komenda = "";

			}
			else if ((komenda[1] == 'c') || (komenda[1] == 'C'))
			{
				if (kierunek != ' ')
				{
					printf("Blad skladni - plik nie moze byc jednoczesnie kodowany i dekodowany");
					PrintSkladnia();
					return 0;
				}
				kierunek = 'c';
				komenda = "";
			}
			else if ((komenda[1] == 'd') || (komenda[1] == 'D'))
			{
				if (kierunek != ' ')
				{
					printf("Blad skladni - plik nie moze byc jednoczesnie kodowany i dekodowany");
					PrintSkladnia();
					return 0;
				}

				kierunek = 'd';
				komenda = "";
			}
		}
		else
		{
			if((komenda[1] == 'f')||(komenda[1]=='F'))
			{
				inputf = (char*)malloc(strlen(argv[i]) + 1);
				strcpy( inputf , argv[i]) ;
				komenda = "";
			}
			else if((komenda[1] == 'o')||(komenda[1]=='O'))
			{
				outputf = (char*)malloc(strlen(argv[i]) + 1);
				strcpy(outputf , argv[i] );
				komenda = "";
			}

		}

	}

	if ( ( strcmp(hparaml , "TAK")==0) || (kierunek == ' ') || (inputf == NULL ) || (outputf == NULL))
	{
		PrintSkladnia();
		return 0;
	}
		
	
	if (kierunek == 'c')
	{
		kompresja(inputf, outputf, (strcmp(vparam, "TAK")==0? 1 : 0), rdepth); // -c -f lz78.cpp -o lz78.lz78 -r1 -v
	}
	if (kierunek == 'd')
	{
		dekompresja(inputf, outputf, (strcmp(vparam, "TAK") == 0 ? 1 : 0), rdepth);
	}

	free(hparaml);
	free(vparam);
	free(rparam);
	free(inputf);
	free(outputf);
	return 0;
}