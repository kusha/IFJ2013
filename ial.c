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
		(*Root)->key = Key;
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
