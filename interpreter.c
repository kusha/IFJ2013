/*
	IFJ project 2013
	Interpreter of IFJ2013 language
	4.11.2013 - 15.12.2013
	
	Team 13 (b/3/I):

	Bank Tomáš			<xbankt00@stud.fit.vutbr.cz>
	Birger Mark			<xbirge00@stud.fit.vutbr.cz>
	Botka Roland		<xbotka00@stud.fit.vutbr.cz>
	Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
	Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
	
	Interpreter of 3AK instructions unit
*/

#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"


char * printInstr (int priority) {
	switch (priority){
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
		default:			return "I_UNDEFINED ";
	}
}

void printOperand (typeData * pointer) {
	int val;
	if (pointer==NULL){
		printf("<UNDEF>");
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
			printf("%f", (*pointer).valueOf.type_DOUBLE);
			break;

		case _STRING:
			printf("%s", strGetContent(&(*pointer).valueOf.type_STRING));
			break;
			
	}
}

int interpreterStart(/*tables for vars (global), table for functions*/typeList *instrList) {

	if (DEBUG_FLAG) printf("Interpeter start\n");

	/*

	// actualVarsTable = global / function
	
	REAL INTERPRETER SKETCH

	listFirst(instrList);
	typeInstruction currentInstr;
	while (1) {
		currentInstr = getCurrent(instrList);
		switch (currentInstr->instrCode) {
			case I_INSTR:
				//something
				break;

			case I_INSTR:
				//something
				break;

		}
		listNext(instrList);
	}

	*/

	if (DEBUG_FLAG) {
		printf("Interpeter debug mode\n");
		printf("Start print instructions\n");
		listFirst(instrList);
		while (instrList->active != NULL) {
			printf(CGRN"%s\t", printInstr(instrList->active->instr.instrCode));
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

	if (DEBUG_FLAG) printf("Interpeter finish\n");
	
	return SUCCESS;
}
