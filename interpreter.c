/* -- IFJ project 2013 -------------------------------------------------------
**	
**	Interpreter of IFJ2013 language
**	4.11.2013 - 15.12.2013
**	
**	Team 13 (b/3/I):
**   
** +Bank Tomáš			<xbankt00@stud.fit.vutbr.cz>
**  Birger Mark			<xbirge00@stud.fit.vutbr.cz>
** +Botka Roland		<xbotka00@stud.fit.vutbr.cz>
**  Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
**  Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
**	
**	Interpreter of 3AK instructions header file
** -------------------------------------------------------------------------*/

/* -- Macro definitions ----------------------------------------------------*/
#define DATA_TYPE(ADDRESS) (ADDRESS)->type


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "interpreter.h"


/* -- Debug for printing instructions --------------------------------------*/

char * printInstr (int priority) {
	switch (priority) {
		case I_STOP:		return "I_STOP		";
		case I_GOTO:		return "I_GOTO		";
		case I_GOTO_IF:		return "I_GOTO_IF	";
		case I_ASSIGN:		return "I_ASSIGN	";
		case I_MULTIPLY: 	return "I_MULTIPLY	";
		case I_DIVIDE: 		return "I_DIVIDE	";
		case I_PLUS: 		return "I_PLUS		";
		case I_MINUS: 		return "I_MINUS		";
		case I_CONCATEN: 	return "I_CONCATEN	";
		case I_C_LESS: 		return "I_C_LESS	";
		case I_C_MORE: 		return "I_C_MORE	";
		case I_C_LESS_EQ: 	return "I_C_LESS_EQ	";
		case I_C_MORE_EQ: 	return "I_C_MORE_EQ	";
		case I_C_IS: 		return "I_C_IS		";
		case I_C_IS_NOT: 	return "I_C_IS_NOT	";
		case I_CONVERT: 	return "I_CONVERT	";
		case I_READ: 		return "I_READ		";
		case I_WRITE: 		return "I_WRITE		";
		case I_STR_LEN: 	return "I_STR_LEN	";
		case I_SUB_STR: 	return "I_SUB_STR	";
		case I_FIND_STR: 	return "I_FIND_STR	";
		case I_SORT_STR: 	return "I_SORT_STR	";
		case I_RETURN: 		return "I_RETURN	";
		case I_CALL: 		return "I_CALL		";
		default:			return "I_UNDEFINED ";
	}
}

void printOperand (typeData * pointer) {
	int val;
	if (pointer == NULL) {
		printf("-----");
		return;
	}
	switch ((*pointer).type) {
		case _NONE:
			printf("<NONE>");
			break;

		case _NULL:
			printf("NULL");
			break;

		case _LOGICAL:
			val = (*pointer).valueOf.type_LOGICAL;
			if (val==0) {
				printf("false");
			} else {
				printf("true");
			}
			break;

		case _INTEGER:
			printf("%i", (*pointer).valueOf.type_INTEGER);
			break;

		case _DOUBLE:
			printf("%6f", (*pointer).valueOf.type_DOUBLE);
			break;

		case _STRING:
			printf("%s", strGetContent(&(*pointer).valueOf.type_STRING));
			break;

		case _FUNCTION:
			if ((*pointer).instruction == NULL) {
				printf("NULL");	//error
			} else {
				printf("%p", (*pointer).instruction);
			}
			break;
	}
}

/* -------------------------------------------------------------------------*/

/* -- Get Value function ---------------------------------------------------*/

void * getValue(typeData* type) {
	switch (type->type) {
		case _NULL:
			return &(type->valueOf.type_NULL);
		case _LOGICAL:
			return &(type->valueOf.type_LOGICAL);
		case _INTEGER:
			return &(type->valueOf.type_INTEGER);
		case _DOUBLE:
			return &(type->valueOf.type_DOUBLE);
		case _STRING:
			return &(type->valueOf.type_STRING);
		default:
			return NULL;
		}
}

/* -------------------------------------------------------------------------*/


/* --- Get Deep function ---------------------------------------------------*/

typeData * getDeep(typeData * first, int deep) {
	if (DEBUG_FLAG) 
		printf(">>> ");
	if (first == NULL) {
		if (DEBUG_FLAG) 
			printf("\n");
		return NULL;
	} else {
		typeData * saver = first;
		int currentDeep = 0;
		typeData * currentPos = first;
		if (DEBUG_FLAG) printf(CMAG"%p"CRED" > "CNRM,(void*)currentPos);
		while (currentDeep< deep) {
			if (currentPos->deeper == NULL) {
				typeData * tmp = malloc(sizeof(typeData));
				*tmp = *saver;
				tmp->deeper = NULL;
				currentPos->deeper = tmp;
			} else {
				currentPos = currentPos->deeper;
				currentDeep += 1;
				if (DEBUG_FLAG) 
					printf(CMAG"%p"CRED" > "CNRM,(void*)currentPos);
			}
		}
		if (DEBUG_FLAG) 
			printf("\n");
		return currentPos;
	}
}

/* -------------------------------------------------------------------------*/

/* --- Convert Data function -----------------------------------------------*/

int convertData (typeData * dataOne, typeData * dataTwo, int type) {
	if (type != _NONE) {
		if (type == _LOGICAL) {
			if (DATA_TYPE(dataTwo) == _NULL) { 
				dataOne->valueOf.type_LOGICAL = false;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _LOGICAL) {
				dataOne->valueOf.type_LOGICAL = dataTwo->valueOf.type_LOGICAL;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _INTEGER) {
				if (dataTwo->valueOf.type_INTEGER == 0)
					dataOne->valueOf.type_LOGICAL = false;
				else
					dataOne->valueOf.type_LOGICAL = true;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _DOUBLE) {
				if (dataTwo->valueOf.type_DOUBLE == 0.0)	
					dataOne->valueOf.type_LOGICAL = false;
				else
					dataOne->valueOf.type_LOGICAL = true;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _STRING) {
				if (dataTwo->valueOf.type_STRING.length == 0)	
					dataOne->valueOf.type_LOGICAL = false;
				else
					dataOne->valueOf.type_LOGICAL = true;
				return 0;
			} else {
				return -1;
			}
		} else if (type == _INTEGER) {
			if (DATA_TYPE(dataTwo) == _NULL) { 
				dataOne->valueOf.type_INTEGER = 0;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _LOGICAL) { 
				if (dataTwo->valueOf.type_LOGICAL == true) {
					dataOne->valueOf.type_INTEGER = 1;
				} else {
					dataOne->valueOf.type_INTEGER = 0;
				}
				return 0;
			} else if (DATA_TYPE(dataTwo) == _INTEGER) { 
				dataOne->valueOf.type_INTEGER = dataTwo->valueOf.type_INTEGER;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _DOUBLE) {  
				int x;
				x=(int) dataTwo->valueOf.type_DOUBLE;
				dataOne->valueOf.type_INTEGER = x;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _STRING) {
				char c;
				string *str = malloc(sizeof(string));
				strInit(str);
				int k = 0;
				while (dataTwo->valueOf.type_STRING.length >= k) {
					c = dataTwo->valueOf.type_STRING.str[k];
					if (!(isspace(c))) {
						if (isdigit(c)) {
							strAddChar(str, c);
						} else {
							break;
						} 
					}
					k++; 
				}
				if (str->length != 0) {
					int x;
					x = atoi (str->str);
					dataOne->valueOf.type_INTEGER = x;
				} else {
					dataOne->valueOf.type_INTEGER = 0;
				}
				strFree(str);
				return 0;
			} else {
				return -1;
			} 
		} else if (type == _DOUBLE) {
			if (DATA_TYPE(dataTwo) == _NULL) { 
				dataOne->valueOf.type_DOUBLE = 0.0;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _LOGICAL) {
				if (dataTwo->valueOf.type_LOGICAL == true) {
					dataOne->valueOf.type_DOUBLE = 1.0;
				} else {
					dataOne->valueOf.type_DOUBLE = 0.0;
				}
				return 0;
			} else if (DATA_TYPE(dataTwo) == _INTEGER) {
				double x;
				x=(double) dataTwo->valueOf.type_INTEGER;
				dataOne->valueOf.type_DOUBLE = x;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _DOUBLE) {
				dataOne->valueOf.type_DOUBLE = dataTwo->valueOf.type_DOUBLE;
				return 0;
			} else if (DATA_TYPE(dataTwo) == _STRING) {
				char c;
				string *s = malloc(sizeof(string));
				strInit(s);
				int num = 0, exp = 0, bot = 0, k = 0, isok = 1;
				if (!(dataTwo->valueOf.type_STRING.length == 0)) {
					while (dataTwo->valueOf.type_STRING.length >= k) {
						c = dataTwo->valueOf.type_STRING.str[k];
						if (!isspace(c)) {
							if ((c == 'e' || c == 'E') 
								&& exp == 0 && num == 1) {
								strAddChar(s, c);
								isok = 0;
								exp = 1;
							} else if ((c == '-' || c == '+') 
								&& exp == 1 && num == 1) {
								strAddChar(s, c);
								isok = 0;
							} else if (c == '.' && bot == 0 
								&& num != 0) {
								strAddChar(s, c);
								bot = 1;
							} else if (isdigit(c)) {
								strAddChar(s, c);
								num = 1;
								if (exp == 1)
									isok = 1;
							} else {
								break;
							} 
						}
						k++; 
					}
					if (!isok) 
						return S_TYPE_ERROR;
					if (!num) 
						return S_TYPE_ERROR;
				}
				if (s->length != 0) {
					double x;
					x = atof (s->str);
					dataOne->valueOf.type_DOUBLE = x;
				} else {
					dataOne->valueOf.type_DOUBLE = 0.0;
				}
					strFree(s);
					return 0;
			} else {
				return -1;
			}  

		} else if (type == _STRING) {
			string newStr;
			strInit(&newStr);
			strClear(&newStr);
			char result[50];
			if (DATA_TYPE(dataTwo) == _NULL) {
				strInit(&dataOne->valueOf.type_STRING);
				strCopy(&dataOne->valueOf.type_STRING, &newStr);
				return 0;
			} else if (DATA_TYPE(dataTwo) == _LOGICAL) {
				if (dataTwo->valueOf.type_LOGICAL == 1) {
					strAddChar(&newStr, '1');
					dataOne->valueOf.type_STRING.length = 1;
				}
				strInit(&dataOne->valueOf.type_STRING);
				strCopy(&dataOne->valueOf.type_STRING, &newStr);
				return 0;
			} else if (DATA_TYPE(dataTwo) == _INTEGER) {
				sprintf(result, "%d", dataTwo->valueOf.type_INTEGER);
				int i = 0;
				while (result[i] != '\0') {
					strAddChar(&newStr, result[i]);
					i++;
				}
				strInit(&dataOne->valueOf.type_STRING);
				strCopy(&dataOne->valueOf.type_STRING, &newStr);
				return 0;
			} else if (DATA_TYPE(dataTwo) == _DOUBLE) {
				sprintf(result, "%g", dataTwo->valueOf.type_DOUBLE);
				int i = 0;
				while (result[i] != '\0') {
					strAddChar(&newStr, result[i]);
					i++;
				}
				strInit(&dataOne->valueOf.type_STRING);
				strCopy(&dataOne->valueOf.type_STRING, &newStr);
				return 0;
			} else if (DATA_TYPE(dataTwo) == _STRING) {
				if (dataOne != dataTwo) {
					strInit(&dataOne->valueOf.type_STRING);
					strCopy(&dataOne->valueOf.type_STRING, 
					&dataTwo->valueOf.type_STRING);
				}
				return 0;
			} else{
				return -1;
			}
		}
	} else {
		return -1;
	}
	return -1;
}

/* -------------------------------------------------------------------------*/

/* -- Interpreter Start - main function of interpreter ---------------------*/

int interpreterStart(typeList *instrList) {
	if (DEBUG_FLAG) 
		printf("Interpeter start\n");

	if (DEBUG_FLAG) {
		printf("Interpeter debug mode\n");
		printf("Start print instructions\n");
		listFirst(instrList);
		while (instrList->active != NULL) {
			printf(CBLU"%p\t\t", (void*)instrList->active);
			printf("%s\t", printInstr(instrList->active->instr.instrCode));
			printOperand(instrList->active->instr.addressOne);
			printf("\t");
			printOperand(instrList->active->instr.addressTwo);
			printf("\t");
			printOperand(instrList->active->instr.addressThree);
			printf("\n"CNRM);
			listNext(instrList);
		}
		printf("Finish print instructions\n");
	}

	int SubStr;
	int SubStrFlag = 0;
	int gotoFlag = 0;
	
	tStackTable deepStack;
	stackTableInit ( &deepStack );	
	int recursiveDeep = 0;
	int callFlag = 0;
	int retFlag = 0;

	int nowCleanFlag = 0;
	int nextCleanFlag = 0;
	typeListItem * gotoWay = NULL;

	listFirst(instrList);
	typeInstruction realInstruction;
	typeInstruction* currentInstr;
	
	while (1) {
		if (DEBUG_FLAG) 
			printf(CRED"\nCurrent deep: %i\n"CNRM, recursiveDeep);
		if (DEBUG_FLAG) 
			printf(CRED"Call (only first change) flag: %i\n"CNRM, callFlag);
		if (DEBUG_FLAG) 
			printf(CRED"Return (not first change) flag: %i\n"CNRM, retFlag);

		currentInstr = getCurrent(instrList);
		realInstruction = *currentInstr;
		if (currentInstr->instrCode == I_ASSIGN) {
			realInstruction.addressOne = 
				getDeep(currentInstr->addressOne, recursiveDeep);
		} else if (currentInstr->instrCode == I_RETURN) {
			realInstruction.addressOne = getDeep(currentInstr->addressOne, 0);
		} else {
			realInstruction.addressOne = 
				getDeep(currentInstr->addressOne, recursiveDeep);
		}
		if (currentInstr->instrCode == I_ASSIGN) {
			realInstruction.addressTwo = 
				getDeep(currentInstr->addressTwo, 
					recursiveDeep-callFlag+retFlag);
		} else if (currentInstr->instrCode == I_CALL) {
			realInstruction.addressTwo = getDeep(currentInstr->addressTwo, 0);
		} else {
			realInstruction.addressTwo = 
				getDeep(currentInstr->addressTwo, recursiveDeep);
		}
		realInstruction.addressThree = 
			getDeep(currentInstr->addressThree, recursiveDeep);

		currentInstr = &realInstruction;

		if (DEBUG_FLAG) 
			printf(CYEL"ADRESSES:\t\t\t\t%p %p %p\n"CNRM, 
				currentInstr->addressOne,currentInstr->addressTwo,
				currentInstr->addressThree);

			nowCleanFlag = nextCleanFlag;

		if (DEBUG_FLAG) 
			printf(CRED"\nNow clean status: %i\n"CNRM, nowCleanFlag);
		if (DEBUG_FLAG) 
			printf(CRED"\nNext clean status: %i\n"CNRM, nextCleanFlag);

		switch (currentInstr->instrCode) {
		
			case I_IDLE:
				break;
			
			case I_STOP:
				break;
				
			/* -- Instruction CALL -----------------------------------------*/
			case I_CALL:
				callFlag = 1;
				stackTablePush (&deepStack, currentInstr->addressOne, 
					currentInstr->addressTwo );
				break;

			/* -- Instruction RETURN ---------------------------------------*/
			case I_RETURN: {
				typeData* returnInstr = NULL;
				typeData* returnData = NULL;
				if (stackTableTop (&deepStack, &returnInstr, &returnData)
					== STACK_EMPTY) {
					typeInstruction newInstruction;
					newInstruction.instrCode = I_STOP;
					newInstruction.addressOne = NULL;
					newInstruction.addressTwo = NULL;
					newInstruction.addressThree = NULL;
					int res;
					if ((res=listAddNext(instrList, newInstruction))!=SUCCESS){
						REPORT("Can't add instructions after actual")
						return res;
					}
				} else {
					typeInstruction newInstruction1;
					newInstruction1.instrCode = I_ASSIGN;
					newInstruction1.addressOne = returnData;
					newInstruction1.addressTwo = currentInstr->addressOne;
					newInstruction1.addressThree = NULL;
					typeInstruction newInstruction2;
					newInstruction2.instrCode = I_GOTO;
					newInstruction2.addressOne = NULL;
					newInstruction2.addressTwo = returnInstr;
					newInstruction2.addressThree = NULL;
					int res;
					if ((res=listAddNext(instrList,newInstruction2))!=SUCCESS){
						REPORT("Can't add instructions after actual")
						return res;
					}
					if ((res=listAddNext(instrList,newInstruction1))!=SUCCESS){
						REPORT("Can't add instructions after actual")
						return res;
					}
					stackTablePop ( &deepStack );
					REPORT("INSTR GENRATED")
				}
				retFlag = 1;
				nextCleanFlag = 1;
				break; 
			}

			/* -- Instruction GOTO ----------------------------------------*/
			case I_GOTO:
				if (DEBUG_FLAG) {
					printf(CGRN"%p\t\t", (void*)currentInstr);
					printf("%s\t", printInstr(currentInstr->instrCode));
					printOperand(currentInstr->addressOne);
					printf("\t");
					printOperand(currentInstr->addressTwo);
					printf("\t");
					printOperand(currentInstr->addressThree);
					printf("\t");
					printf("\n"CNRM);
				}
				if (callFlag == 1) 
					callFlag = 0;
				if (retFlag == 1) 
					retFlag = 0;
				if (nextCleanFlag == 1) 
					nextCleanFlag = 0;
				gotoFlag = 1;
				gotoWay = currentInstr->addressTwo->instruction;
				break;

			/* -- Instruction GOTO IF --------------------------------------*/
			case I_GOTO_IF:
				if (DEBUG_FLAG) {
					printf(CGRN"%p\t\t", (void*)currentInstr);
					printf("%s\t", printInstr(currentInstr->instrCode));
					printOperand(currentInstr->addressOne);
					printf("\t");
					printOperand(currentInstr->addressTwo);
					printf("\t");
					printOperand(currentInstr->addressThree);
					printf("\t");
					printf("\n"CNRM);
				}
				gotoFlag = 1;
				if (currentInstr->addressOne->type == _LOGICAL) {
					if (currentInstr->addressOne->valueOf.type_LOGICAL)
						gotoWay = currentInstr->addressTwo->instruction;
					else
						gotoWay = currentInstr->addressThree->instruction;
				} else {
					return S_EXPRESS_ERROR;
				} 
				break;
				
			/* -- Instruction ASSIGN ---------------------------------------*/
			case I_ASSIGN:
				if ((DATA_TYPE(currentInstr->addressTwo)) == _NULL) {
			 		currentInstr->addressOne->type = _NULL;
					void * p = getValue(currentInstr->addressTwo);
					currentInstr->addressOne->valueOf.type_NULL = p;	
				} else if ((DATA_TYPE(currentInstr->addressTwo)) == _LOGICAL){
					currentInstr->addressOne->type = _LOGICAL;
					void * p = getValue(currentInstr->addressTwo);
					char *i = p;
					currentInstr->addressOne->valueOf.type_LOGICAL = *i;	
				} else if ((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER){
					currentInstr->addressOne->type = _INTEGER;
					void * p = getValue(currentInstr->addressTwo);
					int *i = p;
					currentInstr->addressOne->valueOf.type_INTEGER = *i;
				} else if ((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE){
			 		currentInstr->addressOne->type = _DOUBLE;
					void * p = getValue(currentInstr->addressTwo);
					double *i = p;
					currentInstr->addressOne->valueOf.type_DOUBLE = *i;	
			 	} else if ((DATA_TYPE(currentInstr->addressTwo)) == _STRING){
			 		currentInstr->addressOne->type = _STRING;
					void * p = getValue(currentInstr->addressTwo);
					string *i = p;
					currentInstr->addressOne->valueOf.type_STRING = *i;	
				} else {
					return S_EXPRESS_ERROR; 
				}
				break;
				
			/* -- Instruction PLUS -----------------------------------------*/
			case I_PLUS:
				if (((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER)
				&&((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) {
					currentInstr->addressOne->type = _INTEGER;
					void * p = getValue(currentInstr->addressTwo);
					void * q = getValue(currentInstr->addressThree);   
					int *i = p;
					int *j = q;
					currentInstr->addressOne->valueOf.type_INTEGER = (*i)+(*j);
				}else if ((DATA_TYPE(currentInstr->addressTwo) == _DOUBLE
				&& DATA_TYPE(currentInstr->addressThree) == _INTEGER) 
				|| (DATA_TYPE(currentInstr->addressTwo) == _INTEGER
				&& DATA_TYPE(currentInstr->addressThree) == _DOUBLE)
				|| (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE
				&& DATA_TYPE(currentInstr->addressThree) == _DOUBLE)) {
					currentInstr->addressOne->type = _DOUBLE;
					void * p = getValue(currentInstr->addressTwo);
					void * q = getValue(currentInstr->addressThree);
					double result = 0.0;
					if (((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
					&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)){
						double* i = p;
						double* j = q;
						result = (*i)+(*j);
					} else if (((DATA_TYPE(currentInstr->addressTwo))==_DOUBLE)
					&& ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) {
						double* i = p;
						int* j = q;
						result = (*i)+(*j);
					} else if (((DATA_TYPE(currentInstr->addressTwo))==_INTEGER)
					&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)) {
						int* i = p;
						double* j = q;
						result = (*i)+(*j);
					}
					currentInstr->addressOne->valueOf.type_DOUBLE = result;								
				} else {
					return S_EXPRESS_ERROR; 
				}
				break;	
				 
			/* -- Instruction MINUS ----------------------------------------*/
			case I_MINUS:
				if (((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER)
				&&((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) {
					currentInstr->addressOne->type = _INTEGER;
					void * p = getValue(currentInstr->addressTwo);
					void * q = getValue(currentInstr->addressThree);   
					int *i = p;
					int *j = q;
					currentInstr->addressOne->valueOf.type_INTEGER = (*i)-(*j);				
				} else if ((DATA_TYPE(currentInstr->addressTwo) == _DOUBLE 
				&& DATA_TYPE(currentInstr->addressThree)== _INTEGER)
				|| (DATA_TYPE(currentInstr->addressTwo) == _INTEGER
				&& DATA_TYPE(currentInstr->addressThree)== _DOUBLE)
				|| (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE 
				&& DATA_TYPE(currentInstr->addressThree)== _DOUBLE)) {
					currentInstr->addressOne->type = _DOUBLE;
					void * p = getValue(currentInstr->addressTwo);
					void * q = getValue(currentInstr->addressThree);
					double result = 0.0;
					if (((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
					&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)) {
						double* i = p;
						double* j = q;
						result = (*i)-(*j);
					} else if (((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
					&& ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) {
						double* i = p;
						int* j = q;
						result = (*i)-(*j);
					} else if (((DATA_TYPE(currentInstr->addressTwo))== _INTEGER) 
					&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)) {
						int* i = p;
						double* j = q;
						result = (*i)-(*j);
					}
					currentInstr->addressOne->valueOf.type_DOUBLE = result;								
				} else {
					return S_EXPRESS_ERROR; 
				}
				break;
				
			/* -- Instruction MULTIPLY -------------------------------------*/
			case I_MULTIPLY:
				if (((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER)
				&&((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) {
					currentInstr->addressOne->type = _INTEGER;
					void * p = getValue(currentInstr->addressTwo);
					void * q = getValue(currentInstr->addressThree);   
					int *i = p;
					int *j = q;
					currentInstr->addressOne->valueOf.type_INTEGER = (*i)*(*j);					
				} else if ((DATA_TYPE(currentInstr->addressTwo) == _DOUBLE
				&& DATA_TYPE(currentInstr->addressThree)== _INTEGER) 
				|| (DATA_TYPE(currentInstr->addressTwo) == _INTEGER
				&& DATA_TYPE(currentInstr->addressThree)== _DOUBLE)
				|| (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE
				&& DATA_TYPE(currentInstr->addressThree)== _DOUBLE)) {
					currentInstr->addressOne->type = _DOUBLE;
					void * p = getValue(currentInstr->addressTwo);
					void * q = getValue(currentInstr->addressThree);
					double result = 0.0;
					if (((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
					&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)) {
						double* i = p;
						double* j = q;
						result = (*i)*(*j);
					} else if (((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
					&& ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) {
						double* i = p;
						int* j = q;
						result = (*i)*(*j);
					} else if (((DATA_TYPE(currentInstr->addressTwo))== _INTEGER) 
					&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)) {
						int* i = p;
						double* j = q;
						result = (*i)*(*j);
					}
					currentInstr->addressOne->valueOf.type_DOUBLE = result;								
				} else {
					return S_EXPRESS_ERROR; 
				}
				break;
				
			/* -- Instruction DIVIDE ---------------------------------------*/
			case I_DIVIDE:
				if ((DATA_TYPE(currentInstr->addressTwo) == _INTEGER 
				&& DATA_TYPE(currentInstr->addressThree) == _INTEGER)
				|| (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE
				&& DATA_TYPE(currentInstr->addressThree) == _INTEGER) 
				|| (DATA_TYPE(currentInstr->addressTwo) == _INTEGER
				&& DATA_TYPE(currentInstr->addressThree) == _DOUBLE)
				|| (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE
				&& DATA_TYPE(currentInstr->addressThree) == _DOUBLE)) {
					currentInstr->addressOne->type = _DOUBLE;
					void * p = getValue(currentInstr->addressTwo);
					void * q = getValue(currentInstr->addressThree);
					double result = 0.0;
					if (DATA_TYPE(currentInstr->addressTwo) == _INTEGER
					&& DATA_TYPE(currentInstr->addressThree) == _INTEGER) {
					 	int* i = p;
					 	int* j = q;
					 	double x, y;
					 	x = (double) *i;
					 	y = (double) *j;
					 	if (y == 0) {
							return S_ZERODIV_ERROR;             
						} else {
							result = (x)/(y);
					 	}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE 
					&& DATA_TYPE(currentInstr->addressThree) == _DOUBLE) {
						double* i = p;
						double* j = q;
						if (*j == 0) {
							return S_ZERODIV_ERROR;             
						} else {
							result = (*i)/(*j);
					 	}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE 
					&& DATA_TYPE(currentInstr->addressThree) == _INTEGER) {
						double* i = p;
						int* j = q;
						if (*j == 0) {
							return S_ZERODIV_ERROR;             
						} else {
							result = (*i)/(*j);
					 	}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _INTEGER 
					&& DATA_TYPE(currentInstr->addressThree) == _DOUBLE) {
						int* i = p;
						double* j = q;
						if (*j == 0) {
							return S_ZERODIV_ERROR;             
						} else {
							result = (*i)/(*j);
					 	}
					}
					currentInstr->addressOne->valueOf.type_DOUBLE = result;
				} else {
					return S_EXPRESS_ERROR; 
				}	  	  
				break;
				
			/* -- Instruction CONCATEN -------------------------------------*/
			case I_CONCATEN:
				if (DATA_TYPE(currentInstr->addressTwo)== _STRING) {
					if (DATA_TYPE(currentInstr->addressThree) != _STRING) {
						if (convertData(currentInstr->addressThree, 
						currentInstr->addressThree, _STRING) != 0) {
							return S_TYPE_ERROR;
						}
					}
					currentInstr->addressOne->type = _STRING;
					void * p = getValue(currentInstr->addressTwo);
					void * q = getValue(currentInstr->addressThree);
					string *i = p;
					string *j = q;
					string result;
					strInit(&result);
					int k = 0;
					strCopy(&result,i);
					while (j->str[k] != '\0') {
						strAddChar(&result,(j->str[k]));
						k++;
					} 
					strInit(&currentInstr->addressOne->valueOf.type_STRING);
					strCopy(&currentInstr->addressOne->valueOf.type_STRING,&result);
				} else {
					return S_EXPRESS_ERROR;
				}
				break;	
						
			/* -- Instruction COMPARE IS -----------------------------------*/
			case I_C_IS:	
				if (DATA_TYPE(currentInstr->addressTwo) 
				== DATA_TYPE(currentInstr->addressThree)) {
					currentInstr->addressOne->type = _LOGICAL;
					void *p = getValue(currentInstr->addressTwo);
					void *q = getValue(currentInstr->addressThree);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL) { 
						currentInstr->addressOne->valueOf.type_LOGICAL = 1;
					} else if (DATA_TYPE(currentInstr->addressTwo) == _LOGICAL) {
						char *i = p;
						char *j = q;
						if ((*i) == (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _INTEGER) {
						int *i = p;
						int *j = q;
						if ((*i) == (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE) {
						double *i = p;
						double *j = q;
						if ((*i) == (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _STRING) {
						string *i = p;
						string *j = q;
						if ((strCompare(i,j)) == 0) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					}
				} else {
					currentInstr->addressOne->type = _LOGICAL;
					currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				}
				break;
			
			/* -- Instruction COMPARE IS NOT -------------------------------*/	
			case I_C_IS_NOT:
				if (DATA_TYPE(currentInstr->addressTwo) 
				== DATA_TYPE(currentInstr->addressThree)) {
					currentInstr->addressOne->type = _LOGICAL;
					void *p = getValue(currentInstr->addressTwo);
					void *q = getValue(currentInstr->addressThree);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
					} else if (DATA_TYPE(currentInstr->addressTwo) == _LOGICAL) {
						char *i = p;
						char *j = q;				
						if ((*i) != (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _INTEGER) {
						int *i = p;
						int *j = q;
						if ((*i) != (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE) {
						double *i = p;
						double *j = q;
						if ((*i) != (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _STRING) {
						string *i = p;
						string *j = q;
						if ((strCompare(i,j)) != 0) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					}
				} else {
					currentInstr->addressOne->type = _LOGICAL;
					currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				}
				break;
				
			/* -- Instruction COMARE LESS  ---------------------------------*/			
			case I_C_LESS:
				if (DATA_TYPE(currentInstr->addressTwo) 
				== DATA_TYPE(currentInstr->addressThree)) {
					currentInstr->addressOne->type = _LOGICAL;
					void *p = getValue(currentInstr->addressTwo);
					void *q = getValue(currentInstr->addressThree);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL) { 
						currentInstr->addressOne->valueOf.type_LOGICAL = 0;
					} else if (DATA_TYPE(currentInstr->addressTwo) == _LOGICAL){
				  		char *i = p;
				  		char *j = q;				
						if ((*i) < (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 
						} else {
				  			currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  		}
				  	} else if (DATA_TYPE(currentInstr->addressTwo) == _INTEGER){
				  		int *i = p;
				  		int *j = q;				
						if ((*i) < (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else { 
				  			currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  		}
				  	} else if (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE){
				  		double *i = p;
				  		double *j = q;
						if ((*i) < (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
				  			currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  		}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _STRING){
						string *i = p;
				  		string *j = q;
						if ((strCompare(i,j)) < 0) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 
						} else {
				  			currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  		}
				  	}
				} else {
					return S_EXPRESS_ERROR;
				}
				break;
				
			/* -- Instruction COMPARE LESS EQUAL ---------------------------*/
			case I_C_LESS_EQ:
				if (DATA_TYPE(currentInstr->addressTwo) 
				== DATA_TYPE(currentInstr->addressThree)) {
					currentInstr->addressOne->type = _LOGICAL;
					void *p = getValue(currentInstr->addressTwo);
					void *q = getValue(currentInstr->addressThree);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL) { 
						currentInstr->addressOne->valueOf.type_LOGICAL = 1;
					} else if (DATA_TYPE(currentInstr->addressTwo) == _LOGICAL) {
						char *i = p;
						char *j = q;
						if ((*i) <= (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _INTEGER) {
						int *i = p;
						int *j = q;
						if ((*i) <= (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE) {
						double *i = p;
						double *j = q;
						if ((*i) <= (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _STRING) {
						string *i = p;
						string *j = q;
						if ((strCompare(i,j)) <= 0) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					}
				} else {
					return S_EXPRESS_ERROR;
				}
				break;
				
			/* -- Instruction COMARE MORE ----------------------------------*/			
			case I_C_MORE:
				if (DATA_TYPE(currentInstr->addressTwo) 
				== DATA_TYPE(currentInstr->addressThree)) {
					currentInstr->addressOne->type = _LOGICAL;
					void *p = getValue(currentInstr->addressTwo);
					void *q = getValue(currentInstr->addressThree);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL) { 
						currentInstr->addressOne->valueOf.type_LOGICAL = 1;
					} else if (DATA_TYPE(currentInstr->addressTwo) == _LOGICAL) {
						char *i = p;
						char *j = q;
						if ((*i) > (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _INTEGER) {
						int *i = p;
						int *j = q;
						if ((*i) > (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE) {
						double *i = p;
						double *j = q;
						if ((*i) > (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _STRING) {
						string *i = p;
						string *j = q;
						if ((strCompare(i,j)) > 0) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					}
				} else {
					return S_EXPRESS_ERROR;
				}
				break;
				
			/* -- Instruction COMPARE EQUAL --------------------------------*/
			case I_C_MORE_EQ:
				if (DATA_TYPE(currentInstr->addressTwo) 
				== DATA_TYPE(currentInstr->addressThree)) {
					currentInstr->addressOne->type = _LOGICAL;
					void *p = getValue(currentInstr->addressTwo);
					void *q = getValue(currentInstr->addressThree);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL) { 
						currentInstr->addressOne->valueOf.type_LOGICAL = 1;
					} else if (DATA_TYPE(currentInstr->addressTwo) == _LOGICAL) {
						char *i = p;
						char *j = q;
						if ((*i) >= (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _INTEGER) {
						int *i = p;
						int *j = q;				
						if ((*i) >= (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _DOUBLE) {
						double *i = p;
						double *j = q;
						if ((*i) >= (*j)) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					} else if (DATA_TYPE(currentInstr->addressTwo) == _STRING) {
						string *i = p;
						string *j = q;
						if ((strCompare(i,j)) >= 0) { 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
						} else {
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
						}
					}
				} else {
					return S_EXPRESS_ERROR;
				}
				break;
				
			/* -- Instruction CONVERT --------------------------------------*/
			case I_CONVERT:
				if (convertData(currentInstr->addressOne, currentInstr->addressTwo,
				currentInstr->addressThree->valueOf.type_INTEGER) != 0) {
					return S_TYPE_ERROR;
				} else {
					currentInstr->addressOne->type = 
						currentInstr->addressThree->valueOf.type_INTEGER;
				}
				break;
				
			/* -- Instruction READ -----------------------------------------*/
			case I_READ: {
				int c;
				string result;
				strInit(&result);
				while (((c=getchar()) != '\n') && (c != EOF)) {
					strAddChar(&result, c);
				}
				strInit(&currentInstr->addressOne->valueOf.type_STRING);
				if (strCopy(&currentInstr->addressOne->valueOf.type_STRING, &result)
				!= STR_SUCCESS) return INTERNAL_ERROR;
					currentInstr->addressOne->type=_STRING;				
				break; 
				}
				
			/* -- Instruction WRITE ----------------------------------------*/
			case I_WRITE: {
				typeData * tmp = malloc(sizeof(typeData));
				if (convertData(tmp, currentInstr->addressOne, _STRING) != 0)
					return S_TYPE_ERROR;
				printf("%s",tmp->valueOf.type_STRING.str); 
				break; 
				}
				
			/* -- Instruction STRLEN ---------------------------------------*/
			case I_STR_LEN:
				if (DATA_TYPE(currentInstr->addressTwo)==_STRING) {
					currentInstr->addressOne->type = _INTEGER;
					currentInstr->addressOne->valueOf.type_INTEGER = 
						currentInstr->addressTwo->valueOf.type_STRING.length;
				} else {
					typeData * tmp= malloc(sizeof(typeData));
					if (convertData(tmp, currentInstr->addressTwo, _STRING)!= 0)
						return S_TYPE_ERROR;
					currentInstr->addressOne->type = _INTEGER;
					currentInstr->addressOne->valueOf.type_INTEGER =
						tmp->valueOf.type_STRING.length;
				}		
				break;
				
			/* -- Instruction SUBSTRING ------------------------------------*/
			case I_SUB_STR: {		
				typeData * tmp = malloc(sizeof(typeData));
				if (convertData(tmp, currentInstr->addressThree, _INTEGER) != 0)
					return S_TYPE_ERROR;
				if (SubStrFlag == 0) {
					SubStr = tmp->valueOf.type_INTEGER;
					SubStrFlag = 1;
				} else {
					typeData * tmp1 = malloc(sizeof(typeData));
					if (convertData(tmp1, currentInstr->addressTwo, _STRING) != 0)
						return S_TYPE_ERROR;
					if (SubStr >= 0 
						&& tmp->valueOf.type_INTEGER >= 0 
						&& tmp->valueOf.type_INTEGER >= SubStr
						&& SubStr < tmp1->valueOf.type_STRING.length 
						&& tmp->valueOf.type_INTEGER <= 
						tmp1->valueOf.type_STRING.length) { 
						string *str = malloc(sizeof(string));
						strInit(str);
						char c; 
						int j = tmp->valueOf.type_INTEGER;
						j--;
						for(int i = SubStr; i <= j; i++) {
							c = tmp1->valueOf.type_STRING.str[i];
							strAddChar(str, c);
						}
						currentInstr->addressOne->type = _STRING;
						currentInstr->addressOne->valueOf.type_STRING = *str;
						SubStrFlag = 0;
					} else {
						return S_OTHER_ERROR;
					}
				}
				break; 
				}
			/* -- Instruction FING STRING ----------------------------------*/
			case I_FIND_STR:{		
				typeData * tmp1 = malloc(sizeof(typeData));
				typeData * tmp2 = malloc(sizeof(typeData));
				if (convertData(tmp1, currentInstr->addressTwo, _STRING) != 0)
					return S_TYPE_ERROR;
				if (convertData(tmp2, currentInstr->addressThree, _STRING) != 0)
					return S_TYPE_ERROR;	
 				currentInstr->addressOne->type = _INTEGER ;
				currentInstr->addressOne->valueOf.type_INTEGER = 
					find_string(tmp1->valueOf.type_STRING.str,
					tmp2->valueOf.type_STRING.str);
				break; 
				}
				
			/* -- Instruction SORT STRING ----------------------------------*/
			case I_SORT_STR: {
				typeData * tmp= malloc(sizeof(typeData));
				if (convertData(tmp, currentInstr->addressTwo, _STRING) != 0)
					return S_TYPE_ERROR;
				currentInstr->addressOne->type = _STRING;
				char * processedString =
					sort_string(tmp->valueOf.type_STRING.str);
				string str;
				strInit(&str);
				int i=0;
				while (processedString[i]!='\0') {
					strAddChar(&str, processedString[i]);
					i++;
				}
				strInit(&currentInstr->addressOne->valueOf.type_STRING);
				if (strCopy(&currentInstr->addressOne->valueOf.type_STRING, &str)
				!= STR_SUCCESS)
					return INTERNAL_ERROR;		
				break; 
				}
		/* -- END OF SWITCH ------------------------------------------------*/
		}
		/* -----------------------------------------------------------------*/
		
		recursiveDeep = stackTableCount (&deepStack) + 1;

		if (!gotoFlag) {
			if (DEBUG_FLAG) {
				printf(CGRN"%p\t\t", (void*)currentInstr);
				printf("%s\t", printInstr(currentInstr->instrCode));
				printOperand(currentInstr->addressOne);
				printf("\t");
				printOperand(currentInstr->addressTwo);
				printf("\t");
				printOperand(currentInstr->addressThree);
				printf("\t");
				printf("\n"CNRM);
			}
		}
		if (currentInstr->instrCode == I_STOP)
			break;
		if (nowCleanFlag) {
			REPORT("Removing")
			listRemove(instrList);
		}
		if (gotoFlag) {
			listGoto(instrList,gotoWay);
			gotoFlag = 0;
		}
		listNext(instrList);
		
	/* -- END OF WHILE -----------------------------------------------------*/
	}
	/* ---------------------------------------------------------------------*/
	
	if (DEBUG_FLAG) 
		printf("Interpeter finish\n");
	
	return SUCCESS;
}
/* -- END OF INTERPRETER ---------------------------------------------------*/
