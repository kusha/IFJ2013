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
**	Instruction list unit. One way list.
**
** -------------------------------------------------------------------------*/


/* -- Includes part --------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "instructions.h"


/* -- Basic functions for one way list -------------------------------------*/

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
		list->last = item;
	}
	return SUCCESS;
}

int listAddNext(typeList *list, typeInstruction instr) {
	if (list->active != NULL ) {
		typeListItem *item;
		if ((item = malloc(sizeof(typeListItem)))==NULL) {
			return ALLOC_FAIL;
		}
		item->instr = instr;
		item->next = list->active->next;
		list->active->next = item;
		if (list->active == list->last) {
			list->last = item;
		}
	} else {
		REPORT("No actual for listAddNext")
		return INTERNAL_ERROR;
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

void listRemove(typeList *list) {
	typeListItem *prev = list->first;
	while (prev->next != list->active) {
		prev = prev->next;
	}
	prev->next = list->active->next;
	typeListItem *copy = list->active;
	list->active = prev;
	free(copy);
}


/* -- Additional functions for current implementation -----------------------*/

void listGoto(typeList *list, void *instr) {
	list->active = (typeListItem *) instr;
}

typeInstruction * getCurrent(typeList *list) {
	if (list->active != NULL ) {
		return &(list->active->instr);
	} else {
		if (DEBUG_FLAG) printf("There is no active instruction for response!\n");
		return NULL;
	}
}

typeListItem * getPtrToCurrent(typeList *list) {
	return list->last;
}

typeListItem * getPtrToActive(typeList *list) {
	return list->active;
}

