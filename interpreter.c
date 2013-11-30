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
			printf("%i\t%i\t%i\t%i\n", \
				instrList->active->instr.instrCode, \
				instrList->active->instr.addressOne, \
				instrList->active->instr.addressTwo, \
				instrList->active->instr.addressThree);
			listNext(instrList);
		}
		printf("Finish print instructions\n");
	}

	if (DEBUG_FLAG) printf("Interpeter finish\n");
	
	return SUCCESS;
}
