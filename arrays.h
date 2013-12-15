/* -- IFJ project 2013 -------------------------------------------------------
**
**	Interpreter of IFJ2013 language
**	4.11.2013 - 15.12.2013
**
**	Team 13 (b/3/I):
**
**	Bank Tomáš			<xbankt00@stud.fit.vutbr.cz>
** +Birger Mark			<xbirge00@stud.fit.vutbr.cz>
**	Botka Roland		<xbotka00@stud.fit.vutbr.cz>
**	Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
**	Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
**
**	Array of type data header file.
**
** -------------------------------------------------------------------------*/


#ifndef __ARRAYS_H__
#define __ARRAYS_H__


/* -- Includes part --------------------------------------------------------*/
#include "ial.h"			
#include "string.h"

/* -- Macro definitions ----------------------------------------------------*/
#define MERGE_FAIL -1
#define MAX_CALLS 100000

/* -- Array types and functions --------------------------------------------*/			
typedef struct inputCalls {		
	void * 			instr[MAX_CALLS];
	typeInputArray 	callParams[MAX_CALLS];
	string 			funcName[MAX_CALLS];
	typeData *		returnPtr[MAX_CALLS];
	int size;			
} typeCallsArray;

void arrayClear( typeInputArray * array );
void arrayAdd( typeInputArray * array, typeData * content);
typeData * arrayGet( typeInputArray * array, int idx);

void arrayCallsClear( typeCallsArray * array );
void arrayCallsAdd( typeCallsArray * array, void * instr, 
	typeInputArray callParams, string * funcName, typeData * returnPtr);

#endif
