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
	
	Instruction list unit
*/

#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "instructions.h"

void listInit(typeList *list) {
	list->first = NULL;
	list->active = NULL;
	list->last = NULL;
}

void listDispose(typeList *list) {
	typeListItem *helper;
	while (list->first != NULL) {
		helper = list->first;
		list->first = list->first->next;
		free(helper);
	}
}

int listAdd(typeList *list, typeInstruction instr) {
	typeListItem *item;
	if ((item = malloc(sizeof(typeListItem)))==NULL) {
		return ALLOC_FAIL;
	}
	item->instr = instr;
	item->next = NULL;
	if (list->first == NULL) {
		list->first = item;
		list->last = item;
	} else {
		list->last->next = item;
	}
	return SUCCESS;
}

void listFirst(typeList *list) {
	list->active = list->first;
}

void listNext(typeList *list) {
	if (list->active != NULL ) {
		list->active = list->active->next;
	}
}

// goto instruction needed!!!
// void listGoto(...) {
// 	...
// }

// DEBUG FUNCTIONS
// void listPrint(typeList *list) {
// 	listFirst(list);
// 	printf("instructions list: ");
// 	while (list->active != NULL) {
// 		printf("%i ",list->active->instr.instrCode);
// 		listNext(list);
// 	}
// 	printf("\n");
// }

// int main() {
// 	typeList instructionList;

// 	typeInstruction instruction;
// 	instruction.instrCode = 0;
// 	instruction.addressOne = NULL;
// 	instruction.addressTwo = NULL;
// 	instruction.addressThree = NULL;

// 	listInit(&instructionList);

// 	listPrint(&instructionList);
// 	listAdd(&instructionList, instruction);
// 	listPrint(&instructionList);
// 	listAdd(&instructionList, instruction);
// 	listPrint(&instructionList);
// 	listAdd(&instructionList, instruction);
// 	listPrint(&instructionList);

// 	listDispose(&instructionList);
// }






