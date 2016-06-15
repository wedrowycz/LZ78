#include "treez.h"
#include <stdlib.h>
#include<string.h>

/*
autor Tomasz Jureczko
projekt : LZ78
Funkcje obslugi drzewa
*/


struct theTreeelem* InitializeTree(char firstletter)
{
	return NULL;
}

void  InitializeTreeEkem(struct theTreeelem* head)
{
	int cnt;
	for (cnt = 0; cnt < 256; cnt++)
	{
		head->theslownik[cnt] = NULL;
	}
}

struct theTreeelem* getKod(struct theTreeelem* head, unsigned char *teksttoencode, int maxkod)
{
	if (head->theslownik[teksttoencode[0]] == NULL)
	{
		head->theslownik[teksttoencode[0]] = (theTreeelem*)malloc(sizeof(theTreeelem));
		InitializeTreeEkem(head->theslownik[teksttoencode[0]]);
		head->theslownik[teksttoencode[0]]->kod = maxkod ;
		head->theslownik[teksttoencode[0]]->theparent = head;
		head->theslownik[teksttoencode[0]]->znak = teksttoencode[0];
		head->theslownik[teksttoencode[0]]->IloscUzyc = 1;
		return head->theslownik[teksttoencode[0]];
	}
	else
	{
		head->theslownik[teksttoencode[0]]->IloscUzyc++;
		return head->theslownik[teksttoencode[0]];
		//struct theTreeelem* nastepny;
		//nastepny = getKod(head->theslownik[teksttoencode[0]], &teksttoencode[1], maxkod);
		//if (nastepny == NULL)
		//{			
		//	return head;
		//}
		//else
		//{
		//	return nastepny;
		//}
	}
}

//czyszczenie drzewa - zarówno dla zakoñczenia programu jak i zerowania s³ownika
void  clearTree(struct theTreeelem* head)
{
	int cnt;
	for (cnt = 0; cnt < 256; cnt++)
	{
		if (head->theslownik[cnt] != NULL)
		{
			clearTree(head->theslownik[cnt]);
			free(head->theslownik[cnt]);

			head->theslownik[cnt] = NULL;
		}
	}
	
}

void writeTreeToFile(struct theTreeelem* head, char * fileName)
{

}


 unsigned char *textFromTree(struct theTreeelem* thePath, int depth)
{
	unsigned char* wynik;
	unsigned char cnt;
	if (thePath->theparent->kod == 0)
	{
		wynik = (unsigned char *)malloc(depth+1);		
		wynik[depth] = 0;
		for (cnt = 0; cnt < 256;cnt++)
		{ 
			if (thePath->theparent->theslownik[cnt] != NULL)
			{
				if (thePath->theparent->theslownik[cnt]->kod == thePath->kod)
				{
					wynik[0] = cnt;					
					thePath->IloscUzyc++;
					return wynik;					
				}
			}
		}
	}
	else
	{
		wynik = textFromTree(thePath->theparent, depth+1);
		for (cnt = 0; cnt < 256; cnt++)
		{
			if (thePath->theparent->theslownik[cnt] != NULL)
			{
				if (thePath->theparent->theslownik[cnt]->kod == thePath->kod)
				{
					wynik[strlen((char*)wynik)-depth] = cnt; //wynik + cnt;
					thePath->IloscUzyc++;
					return wynik;
				}
			}
		}
	}
	
}

 //funkcja eliminuje nieu¿ywane elementy ze struktury slownikowej, zwraca ilosc usunietych elementow
 int clearUnusedElements(struct theTreeelem *head, struct theTreeelem *listaelementow[256], int maxlevel)
 {
	 int licznik;
	 int liczbauzyc;
	 int wynik =0;
	 //kazdy przebieg zmniejsza liczbe uzyc "parenta", mozna zwolnic wiec elementy uzyte n razy stosujac ten sam algorytm
	 for (liczbauzyc = 1; liczbauzyc <= maxlevel; liczbauzyc++)
	 {
		 for (licznik = 1; licznik < 256; licznik++)
		 {
			 if ((listaelementow[licznik] != NULL) && (listaelementow[licznik]->IloscUzyc == 1))
			 {
				 listaelementow[licznik]->theparent->IloscUzyc--;
				 listaelementow[licznik]->theparent->theslownik[listaelementow[licznik]->znak] = NULL;
				 free(listaelementow[licznik]);
				 listaelementow[licznik] = NULL;
				 wynik++;
			 }
		 }
	 }
	 return wynik;
 }

 int findFirstFree(struct theTreeelem *listaelementow[256])
 {
	 int elem =-1;
	 for (elem = 1; elem < 256; elem ++)
	 {
		 if (listaelementow[elem] == NULL)
			 break;
	 }
	 return (elem < 256 ? elem : -1 );
 }