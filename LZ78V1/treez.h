#include <stdio.h>

/*
autor Tomasz Jureczko
projekt : LZ78
Funkcje i struktury obslugi drzewa
*/


struct theTreeelem
{
	unsigned char kod;
	unsigned char znak;
	unsigned int IloscUzyc;
	struct theTreeelem *theslownik[256];
	struct theTreeelem *theparent;
};


struct theTreeelem *InitializeTree(char firstletter);
struct theTreeelem *getKod(struct theTreeelem* head, unsigned char *teksttoencode, int maxkod);
void  clearTree(struct theTreeelem* head);
void  InitializeTreeEkem(struct theTreeelem* head);
void writeTreeToFile(struct theTreeelem* head, char * fileName);
unsigned char *textFromTree(struct theTreeelem* thePath, int depth);

int clearUnusedElements(struct theTreeelem *head, struct theTreeelem *listaelementow[256], int maxlevel);
int findFirstFree(struct theTreeelem *listaelementow[256]);



