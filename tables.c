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
**	Defines for all units of the project.
**
** -------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include "tables.h"


typeData * getVariable(typeNodePtr * table, string * name, char existFlag) {
	// 1. if we call it from top-down and want to assign
	// just assign expressiom / func result from temp var name
	// 2. if we call from expression - NULL is semantic error
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
		newData.type = _NONE;
		if (treeInsert(table, *name, newData)==ALLOC_FAIL) return NULL;
		return &(*treeSearch(table, *name))->data;
	}
}

int varCounter = 0;
string tempVariableName () {
	// integer -> char *
	char str[MAX_TEMP_VARS];
	sprintf(str, "%d", varCounter);
	// create string
	string tempName;
	strInit(&tempName);
	// char * -> string
	int i = 0;
	while (str[i]!='\0') {
		strAddChar(&tempName, str[i]);
		i++;
	} 
	// will be Freeed by dispose
	if (DEBUG_FLAG) printf("Created variable with name %s\n", tempName.str);
	// add counter
	varCounter++;
	return tempName;
}

typeData * getLiteral(typeNodePtr * table, int dataType, string * atribute) {
	typeData newData;
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

		// case TYPE_FUNCTION:
	}
	string newName = tempVariableName();
	if (treeInsert(table, newName, newData)==ALLOC_FAIL) return NULL;
	return &(*treeSearch(table, newName))->data;
}

typeData * getEmpty(typeNodePtr * table) {
	typeData newData;
	newData.type = _NONE;
	string newName = tempVariableName();
	if (treeInsert(table, newName, newData)==ALLOC_FAIL) return NULL;
	return &(*treeSearch(table, newName))->data;
}

typeNodePtr createTable() {
	typeNodePtr newTable;
	treeInit(&newTable);	
	return newTable;
}






