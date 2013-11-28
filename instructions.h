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
	
	Header file of instruction list unit
*/

#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "common.h"
#include "ial.h"

// list of all instructions

#define I_STOP 			0 // exit		----	----	----

#define I_GOTO 			1 // ->			----	where	----
#define I_GOTO_IF 		2 // ?->		true?	where	----

#define I_ASSIGN		10 // =			where	from	----
#define I_PLUS			11 // +			where	oper1	oper2
#define I_MINUS			12 // -			where	oper1	oper2
#define I_MULTIPLY		13 // *			where	oper1	oper2
#define I_DIVIDE		14 // /			where	oper1	oper2
#define I_CONCATEN		15 // .			where	oper1	oper2

#define I_C_IS			20 // ===		result	oper1	oper2
#define I_C_IS_NOT		21 // !==		result	oper1	oper2
#define I_C_LESS		22 // <			result	oper1	oper2
#define I_C_LESS_EQ		23 // <=		result	oper1	oper2
#define I_C_MORE		24 // >			result	oper1	oper2
#define I_C_MORE_EQ		25 // >=		result	oper1	oper2

/*

#define I_WRITE
#define I_READ

*/


// structure of 3AK instructions list

typedef struct {
	int instrCode;
	void* addressOne;
	void* addressTwo;
	void* addressThree;
} typeInstruction;

typedef struct listItem {
	typeInstruction instr;
	struct listItem* next;
} typeListItem;

typedef struct {
	struct listItem* first;
	struct listItem* active;
	struct listItem* last;
} typeList;

// instructions list errors

#define ALLOC_FAIL -1

// prototypes of instruction list unit

void listInit(typeList *list);
void listDispose(typeList *list);
int listAdd(typeList *list, typeInstruction instr);
void listFirst(typeList *list);
void listNext(typeList *list);
typeInstruction * getCurrent(typeList *list);
//goto instruction


#endif
