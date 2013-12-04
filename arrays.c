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

#include <stdlib.h>
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

typeData * arrayGet( typeInputArray * array, int idx) {
	if (idx > array->size) {
		return NULL;
	}
	return array->arr[idx];
}
