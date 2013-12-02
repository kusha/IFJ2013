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
**	Header file of instruction list implementation.
**
** -------------------------------------------------------------------------*/


#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__


#include "common.h"	//for common defines
#include "ial.h"	//for typeData inside instructions


/* -- All instruction definition -------------------------------------------*/

/* -- Common instructions ---------------------------------*/
#define I_STOP 			0 	// exit		-----	-----	-----

/* -- Goto instrutcions -----------------------------------*/
#define I_GOTO 			1 	// ->		-----	where	-----
#define I_GOTO_IF 		2 	// ?->		true?	where	-----

/* -- Operation instructions ------------------------------*/
#define I_ASSIGN		10 	// =		where	from	-----
#define I_PLUS			11 	// +		where	oper1	oper2
#define I_MINUS			12 	// -		where	oper1	oper2
#define I_MULTIPLY		13 	// *		where	oper1	oper2
#define I_DIVIDE		14 	// /		where	oper1	oper2
#define I_CONCATEN		15 	// .		where	oper1	oper2

/* -- Logical instructions --------------------------------*/
#define I_C_IS			20 	// ===		result	oper1	oper2
#define I_C_IS_NOT		21 	// !==		result	oper1	oper2
#define I_C_LESS		22 	// <		result	oper1	oper2
#define I_C_LESS_EQ		23 	// <=		result	oper1	oper2
#define I_C_MORE		24 	// >		result	oper1	oper2
#define I_C_MORE_EQ		25 	// >=		result	oper1	oper2

/* -- Datatype convertation instruction -------------------*/
#define I_CONVERT		30 	// type ->	change	d_type	-----

/* -- IO instructions -------------------------------------*/
#define I_READ			40 	// scanf	target	-----	-----
#define I_WRITE			41 	// printf	input	-----	-----

/* -- Special instructions for strings --------------------*/
#define I_STR_LEN		50 	//			????	oper1	oper2
#define I_SUB_STR		51 	//			????	oper1	oper2
#define I_FIND_STR		52 	//			????	oper1	oper2
#define I_SORT_STR		53 	//			????	oper1	oper2

/* -------------------------------------------------------------------------*/

// structure of 3AK instructions list

typedef struct {				//structure of 3AK instruction
	int instrCode;				//code of 3AK instruction (more above)
	typeData * addressOne;		//pointer to first operand
	typeData * addressTwo;		//pointer to second operand
	typeData * addressThree;	//pointer to third operand
} typeInstruction;				//datatype declaration (without struct)

typedef struct listItem {	//structure for node of list
	typeInstruction instr;	//instruction
	struct listItem* next;	//pointer to next instruction
} typeListItem;				//datatype declaration (without struct)

typedef struct {				//structure for list of nodes
	struct listItem* first;		//pointer to first instruction in list
	struct listItem* active;	//pointer to current instructions
	struct listItem* last;		//pointer to last instruction in list
} typeList;						//datatype declaration (without struct)

void listInit(typeList *list);						//inicialize new instructions list
void listDispose(typeList *list);					//dispose all nodes of list
int listAdd(typeList *list, typeInstruction instr);	//add instruction to the end of list
void listFirst(typeList *list);						//set current instruction to start
void listNext(typeList *list);						//set current to the next
typeInstruction * getCurrent(typeList *list);		//return current instructions content
void listGoto(typeList *list, void *instr);			//goto instruction, set current
typeListItem * getPtrToCurrent(typeList *list);		//generate pointer to instruction


#endif
