// #include "instructions.h"
// #include "str.h"

#define _NULL		0
#define _LOGICAL	1
#define _INTEGER	2
#define _DOUBLE		3
#define _STRING		4
#define _FUNCTION	5	

#define ALLOC_FAIL -1
#define SUCCESS 0

typedef struct Data{
	char type;
	union {
		const void *	type_NULL;
		char			type_LOGICAL;
		int 			type_INTEGER;
		double 			type_DOUBLE;
		string			type_STRING;
	} valueOf;	
	union jump { 			//for TYPE_FUNCTION
		typeInstruction *	entryPoint;
		typeInstruction *	returnPoint;
	};
} typeData;

typedef struct typeNode{
	string key;
	typeData data;
	struct typeNode *left;
	struct typeNode *right;
} *typeNodePtr;

void treeInit (typeNodePtr *Root);
typeNodePtr * treeSearch (typeNodePtr * Root, string Key);
int treeInsert (typeNodePtr * Root, string Key, typeData Data);
void treeDispose (typeNodePtr *Root);
void treePrint (typeNodePtr *Root);
