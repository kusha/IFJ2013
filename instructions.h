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

// list of all instructions

#define I_INSTR 0
// ...

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
//goto instruction
