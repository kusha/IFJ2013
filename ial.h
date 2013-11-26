#include "str.h"

#define ALLOC_FAIL -1;
#define SUCCESS 0;

typedef struct Data{
	char Type;
	char Value;
}tData;

typedef struct tNode{
	string key;
	tData data;
	struct tNode *lptr;
	struct tNode *rptr;
} *tNodePtr;

char Search(tNodePtr Root, string Key);
int Insert(tNodePtr* Root, string Key, tData Data);
void Dispose(tNodePtr Root);
