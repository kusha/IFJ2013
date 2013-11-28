#include <stdio.h>
#include <stdlib.h>
#include "tables.h"

typeNodePtr globalVars;			//global table of vars
typeNodePtr globalFunctions;	//global table of functions

typeData * getVariable(string * name, char existFlag) {
	// 1. if we call it from top-down and want to assign
	// just assign expressiom / func result from temp var name
	// 2. if we call from expression - NULL is semantic error
	typeNodePtr * searchNode = treeSearch(&globalVars, *name);
	if (searchNode == NULL && existFlag) {
		if (DEBUG_FLAG) printf("Can't return unexist variable\n");
		return NULL;
	} else {
		typeData newData;
		newData.type = _NULL;
		newData.valueOf.type_NULL = NULL;
		if (treeInsert(&globalVars, *name, newData)==ALLOC_FAIL) return NULL;
		return &(*treeSearch(&globalVars, *name))->data;
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

typeData * getLiteral(int dataType, string * atribute) {
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
	if (treeInsert(&globalVars, newName, newData)==ALLOC_FAIL) return NULL;
	return &(*treeSearch(&globalVars, newName))->data;
}

typeData * getEmpty() {
	typeData newData;
	newData.type = _NULL;
	newData.valueOf.type_NULL = NULL;
	// switch (dataType) {			//initial values
	// 	case _NULL:
			
	// 		break;

	// 	case _LOGICAL:
	// 		newData.valueOf.type_LOGICAL = 0;
	// 		break;

	// 	case _INTEGER:
	// 		newData.valueOf.type_INTEGER = 0;
	// 		break;

	// 	case _DOUBLE:
	// 		newData.valueOf.type_DOUBLE = 0.0;
	// 		break;

	// 	case _STRING:
	// 		strInit(&newData.valueOf.type_STRING);
	// 		break;

	// 	// case TYPE_FUNCTION:
	// }
	string newName = tempVariableName();
	if (treeInsert(&globalVars, newName, newData)==ALLOC_FAIL) return NULL;
	return &(*treeSearch(&globalVars, newName))->data;
}

// typeConversion (typeData * variable, )

// int main () {

// 	string myString1;
// 	strInit(&myString1);
// 	strAddChar(&myString1, '1');
// 	strAddChar(&myString1, '2');
// 	strAddChar(&myString1, 'e');
// 	strAddChar(&myString1, '-');

// 	string myString2;
// 	strInit(&myString2);
// 	strAddChar(&myString2, 'v');
// 	strAddChar(&myString2, 'd');
// 	strAddChar(&myString2, 's');

// 	// strAddChar(&myString2, 'a');
// 	// strAddChar(&myString2, 'k');
// 	// strAddChar(&myString2, 'a');

// 	treeInit (&globalVars);

// 	typeData * tempVariable1;
// 	tempVariable1 = getEmpty();

// 	typeData * tempVariable2;
// 	tempVariable2 = getLiteral(_DOUBLE, &myString1);

// 	// typeData * tempVariable3;
// 	// tempVariable3 = getVariable(string * name, char existFlag)

// 	typeData * tempVariable3;
// 	tempVariable3 = getVariable(&myString2, SHOULD_EXIST);
// 	tempVariable3 = getVariable(&myString2, MAY_NOT_EXIST);


// 	// typeData * tempVariable3;
// 	// tempVariable3 = getVariable(&myString2, 0);


// 	// if (tempVariable3 == NULL) {
// 	// 	printf("NULL\n");
// 	// }

// 	treePrint (&globalVars);

// 	treeDispose (&globalVars);
// }






