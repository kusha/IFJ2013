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
** +Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
** +Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
**
**	Binary Search Tree implementation.
**
** -------------------------------------------------------------------------*/


#ifndef __IAL_H__
#define __IAL_H__


#include "common.h"			//for common defines
#include "str.h"			//string datatypes in keys & data

#define _NONE		-1	//code of non type (task page )
#define _NULL		0	//code of type null
#define _LOGICAL	1	//code of type boolean
#define _INTEGER	2	//code of type integer
#define _DOUBLE		3	//code of type double
#define _STRING		4	//code of type string
#define _FUNCTION	5	//code for functions table

/* -- Debug function for BST printing --------------------------------------*/

#define MAX_PARAMS 100	//number of maximum params for function
typedef struct inputArray {			//input array structure from arrays.h
	struct Data * arr[MAX_PARAMS];
	int size;			
} typeInputArray;

/* -- Data of interpreter structure ----------------------------------------*/

typedef struct Data{						//datatype for data of node
	char type;								//type of current data (more above)
	union {						//dynamic types: all types in one
		const void *	type_NULL;			//const value of type null
		char			type_LOGICAL;		//value of type boolean
		int 			type_INTEGER;		//value of type integer
		double 			type_DOUBLE;		//value of type double
		string			type_STRING;		//value of type string (really structure)
	} valueOf;
	void * instruction;
	typeInputArray		inputData;		//array of pointers to typeData (input)
	struct Data *		deeper;				//recursive variable
} typeData;									//datatype declaration (without struct)

/* -- Binary search tree struct + functions ---------------------------------*/

typedef struct typeNode{		//structure of tree node
	string key;					//ranging key of node
	typeData data;				//data of current node
	struct typeNode *left;		//pointer to left node
	struct typeNode *right;		//pointer to right node
} *typeNodePtr;

void treeInit (typeNodePtr *Root);								//inicialize new tree
typeNodePtr * treeSearch (typeNodePtr * Root, string Key);		//return node ptr/null
int treeInsert (typeNodePtr * Root, string Key, typeData Data);	//insert new node
void treeDispose (typeNodePtr *Root);							//dispose all tree
void treePrint (typeNodePtr *Root);								//debug tree print function


/* -- Shell sort -----------------------------------------------------------*/
char *sort_string(char *string);	//shell sort

/* -- Booyer-Moore search --------------------------------------------------*/
# define zadnyZeZnaku 256 	// Boyer-Moore
int max(int a, int b);		//Boyer-Moore - pomocna funkce pro vyhledani vetsi hodnoty
//Boyer-Moore - Funkce pro BMA , zpracuje pole nesouhlasnych znaku
void nesouhlasZnakFce(char *str, int size, int badchar[zadnyZeZnaku]);
int find_string(char *txt, char *pat);	//Boyer-Moore - BMA vyhledavaci algoritmus


#endif
