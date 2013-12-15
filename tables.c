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
**	Defines for all units of the project.
**
** -------------------------------------------------------------------------*/

/* -- Includes part --------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "tables.h"

int varCounter = 0;

/* -- Get Variable function ------------------------------------------------*/
typeData * getVariable(typeNodePtr * table, string * name, char existFlag) {
	typeNodePtr * searchNode = treeSearch(table, *name); 			
	if (searchNode == NULL && existFlag == SHOULD_EXIST) {
		if (DEBUG_FLAG) printf("Can't return unexist variable\n");
			return NULL;
	} else if (searchNode != NULL && existFlag == SHOULD_NOT_EXIST) {
		if (DEBUG_FLAG) printf("Node exist, but already declared.\n");
			return NULL;
	}
	if (searchNode != NULL) {
		return &(*searchNode)->data;
	} else {
		typeData newData;
		newData.deeper = NULL;
		newData.type = _NONE;
		if (treeInsert(table, *name, newData)==ALLOC_FAIL) 
			return NULL;
		return &(*treeSearch(table, *name))->data;
	}
}

/* -- Temp Variable Name function ------------------------------------------*/
string tempVariableName () {
	char str[MAX_TEMP_VARS];
	sprintf(str, "%d", varCounter);
	string tempName;
	strInit(&tempName);
	int i = 0;
	while (str[i]!='\0') {
		strAddChar(&tempName, str[i]);
		i++;
	} 
	if (DEBUG_FLAG) 
		printf("Created variable with name %s\n", tempName.str);
	varCounter++;
	return tempName;
}

/* -- Get Literal function -------------------------------------------------*/
typeData * getLiteral(typeNodePtr * table, int dataType, string * atribute) {
	typeData newData;
	newData.deeper = NULL;
	newData.type = dataType;
	switch (dataType) {
		case _NULL:
			newData.valueOf.type_NULL = NULL;
			break;

		case _LOGICAL:
			if (strCompareConst(atribute, "true")) {
				newData.valueOf.type_LOGICAL = 1;
			} else if (strCompareConst(atribute, "false")) {
				newData.valueOf.type_LOGICAL = 0;
			}
			break;

		case _INTEGER:
			if (sscanf(strGetContent(atribute), "%d", \
				&newData.valueOf.type_INTEGER)!=1)
				return NULL;
			break;

		case _DOUBLE:
			if (sscanf(strGetContent(atribute), "%lf", \
				&newData.valueOf.type_DOUBLE)!=1)
				return NULL;
			break;

		case _STRING:
			strInit(&newData.valueOf.type_STRING);
			strCopy(&newData.valueOf.type_STRING, atribute);
			break;

	}
	string newName = tempVariableName();
	if (treeInsert(table, newName, newData)==ALLOC_FAIL) 
		return NULL;
	return &(*treeSearch(table, newName))->data;
}

/* -- Get Empty function ---------------------------------------------------*/
typeData * getEmpty(typeNodePtr * table) {
	typeData newData;
	newData.deeper = NULL;
	newData.type = _NONE;
	string newName = tempVariableName();
	if (treeInsert(table, newName, newData)==ALLOC_FAIL) 
		return NULL;
	return &(*treeSearch(table, newName))->data;
}

/* -- Create Table Function ------------------------------------------------*/
typeNodePtr createTable() {
	typeNodePtr newTable;
	treeInit(&newTable);	
	return newTable;
}
