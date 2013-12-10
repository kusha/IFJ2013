/* -- IFJ project 2013 ------------------------------------------------------
**
**	Interpreter of IFJ2013 language
**	4.11.2013 - 15.12.2013
**
**	Team 13 (b/3/I):
**
**	Bank Tomáš			<xbankt00@stud.fit.vutbr.cz>
**	Birger Mark			<xbirge00@stud.fit.vutbr.cz>
**	Botka Roland		<xbotka00@stud.fit.vutbr.cz>
**	Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
**	Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
**
**	Binary Search Tree implementation. ...
**
** -------------------------------------------------------------------------*/


/* -- Includes part --------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ial.h"


/* -- Functions for BST ----------------------------------------------------*/

void treeInit (typeNodePtr *Root) {
	(*Root) = NULL;
}	

typeNodePtr * treeSearch (typeNodePtr * Root, string Key) {
	if ((*Root) != NULL) {
		int compareResult = strCompare(&(*Root)->key, &Key);
		if (compareResult == 0) {
			return Root;
		} else {
			if (compareResult > 0) {
				return treeSearch (&(*Root)->left, Key);
			} else {
				return treeSearch (&(*Root)->right, Key);
			}
		}
	} else {
		return NULL;										//not founded
	}
} 


int treeInsert (typeNodePtr * Root, string Key, typeData Data)	{	
	if ((*Root) == NULL) {											// new node
		if (((*Root)=malloc(sizeof(struct typeNode)))==NULL) return ALLOC_FAIL;
		if (strCopy(&(*Root)->key, &Key)==STR_ERROR) return ALLOC_FAIL;
		(*Root)->data = Data;
		(*Root)->left = NULL;
		(*Root)->right = NULL;
	} else {
		int compareResult = strCompare(&(*Root)->key, &Key);
		if (compareResult > 0) {
			return treeInsert (&(*Root)->left, Key, Data);
		} else if (compareResult < 0) {
			return treeInsert (&(*Root)->right, Key, Data);
		} else {
			(*Root)->data = Data;
		}
	}
	return SUCCESS;
}

void treeDispose (typeNodePtr *Root) {	
	if ((*Root) != NULL) {
		treeDispose(&(*Root)->left);
		treeDispose(&(*Root)->right);
		free(*Root);
		(*Root) = NULL; //empty tree after dispose
	}
}


/* -- Debug function for BST printing --------------------------------------*/

void treePrint (typeNodePtr *Root) {	
	if ((*Root) != NULL) {
		printf("'%s' -> [%i , ", (*Root)->key.str,(*Root)->data.type);
		char val;
		switch ((*Root)->data.type) {
			case _NULL:
				printf("NULL");
				break;

			case _LOGICAL:
				val = (*Root)->data.valueOf.type_LOGICAL;
				if (val==0) {
					printf("false");
				} else {
					printf("true");
				}
				break;

			case _INTEGER:
				printf("%i", (*Root)->data.valueOf.type_INTEGER);
				break;

			case _DOUBLE:
				printf("%f", (*Root)->data.valueOf.type_DOUBLE);
				break;

			case _STRING:
				printf("%s", strGetContent(&(*Root)->data.valueOf.type_STRING));
				break;
				
		}
		printf("]\n");
		treePrint(&(*Root)->left);
		treePrint(&(*Root)->right);
	}
}


/* -- Shell sort -----------------------------------------------------------*/

char *sort_string(char *string) {						//shell sort serazeni znaku retezce podle ordinalni hodnoty od nejmensi po nejvetsi
	int n, step, i, j;
	char tmp;										//pomocna pro razeni
	n = strlen(string);								//zjistime delku stringu -> n
	step = n / 2;
	while(step > 0){									//opakuj dokud bude krok 1 a vetsi
		for(i = step; i < n - step; i++){
			j = i - step + 1;
			while((j >= 0) && (string[j] > string[j + step])){
				tmp = string[j + step];					//nasledujici 3 radky prohozeni hodnot jako u Bubble
				string[j + step] = string[j];
				string[j] = tmp;
				j = j - step;							//snizeni indexu o krok
			}
		}
		step = step / 2;								//opet puleni kroku
	}
	return string;
}


/* -- Boyer-Moorův ---------------------------------------------------------*/

int max(int a, int b)  //	pomocna funkce pro vyhledani vetsi hodnoty
{
  return (a > b)? a: b; 
}							

void nesouhlasZnakFce( char *str, int size, int badchar[zadnyZeZnaku])	//	Funkce pro BMA , zpracuje pole nesouhlasnych znaku
{
	int i;

	for (i = 0; i < zadnyZeZnaku; i++)									// Vsechny vyskyty inicializovat na -1
		badchar[i] = -1;

	for (i = 0; i < size; i++)											// Vyplni aktualni hodnotu posledniho vyskytu znaku
		badchar[(int) str[i]] = i;
}

int find_string(char *txt, char *pat)									// BMA vyhledavaci algoritmus
{
	int m = strlen(pat);
	int n = strlen(txt);

	int badchar[zadnyZeZnaku];

	nesouhlasZnakFce(pat, m, badchar);									//	Vyplni pole spatnych znaku volanim funkce nesouhlasZnakFce() pro dany vzorek

	int s = 0;															// s je posun vzorku k textu
	while(s <= (n - m))
	{
		int j = m-1;

		/* Redukovani indexu <j> ze vzorku (pattern)
		 * DOKUD se znaky vzoru a textu shoduji po posunuti <s> */
		while(j >= 0 && pat[j] == txt[s+j])
			j--;

		/* Pokud se vzorek nachazi v soucasnem posunu, tak index j
		 * se zmeni v -1 po dovrseni smycky */
		if (j < 0){
			return s;														//pokud jsem tedy nasel prvni vyskyt, koncim a vracim index
			
			/* Posun vzoru tak, aby dalsi znak v textu byl zarovnan s jeho poslednim vyskytem ve vzoru.
			 * Podminka s + m < n je nutna, pro pripad, kdy se vzor vyskytuje na konci textu */
			s += (s+m < n)? m-badchar[(unsigned char)txt[s+m]] : 1;
		}

		else{
			/* Posun vzoru tak, aby spatny znak v textu byl zarovnan s jeho poslednim vyskytem ve vzoru.
			 * Fce max se pouzije, aby sme opravdu dostali co "nejvyhodnejsi" posun == nejvetsi.
			 * Muzeme dostat negativni posun v pripade, ze posledni vyskyt spatneho znaku ve vzoru je na prave strane soucasneho znaku. */
			s += max(1, j - badchar[(unsigned char)txt[s+j]]);
		}
	}
	return -1;															//kdyz nedostanu prvni vyskyt viz vyse, vratim -1
}

/* -------------------------------------------------------------------------*/
