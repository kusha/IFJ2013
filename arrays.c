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
**	Array for input params of functions. TODO is unlimited array.
**
** -------------------------------------------------------------------------*/


/* -- Includes part --------------------------------------------------------*/

#include "common.h"
#include "arrays.h"


/* -- Functions for array of type Data pointers ----------------------------*/


void arrayClear( typeInputArray * array ) {
	array->size = -1;
}

void arrayAdd( typeInputArray * array, typeData * content) {
	array->size++;
	array->arr[array->size]=content;
}

int arraysMerge( typeInputArray * array1, typeInputArray * array2) {
	if (array1->size == array2->size) {
		int i;
		for (i=0; i<=array1->size; i++) {
			array1->arr[i]=array2->arr[i];
		}
		return SUCCESS;
	}
	return MERGE_FAIL;
}