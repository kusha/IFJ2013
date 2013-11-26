#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "ial.h"

char Search(tNodePtr Root, string Key){
	if (Root!=NULL){
		int strCompaResult=strCompare(&Root->key,&Key);
		if (strCompaResult==0) return 1;
		else if (strCompaResult>0)
			Search(Root->lptr,Key);
		else 	
			Search(Root->rptr,Key);
	}
	else return 0;
}


int Insert(tNodePtr* Root, string Key, tData Data){
	if (*Root==NULL){
		if (((*Root)=malloc(sizeof(struct tNode)))==NULL) return ALLOC_FAIL;
		(*Root)->key=Key;
		(*Root)->key.str=malloc(sizeof(char)*Key.length);
		strcpy((*Root)->key.str,Key.str);
		(*Root)->data=Data;
		(*Root)->lptr=NULL;
		(*Root)->rptr=NULL;	
	}
	else {
		int strCompaResult=strCompare(&((*Root)->key),&Key);
		if (strCompaResult<0) Insert(&((*Root)->lptr),Key,Data);
		else if (strCompaResult>0) Insert(&((*Root)->rptr),Key,Data);
		else (*Root)->data=Data;
	}
	return SUCCESS;
}

	
	
void Dispose(tNodePtr Root){
	if (Root != NULL){
		Dispose(Root->lptr);
		Dispose(Root->rptr);
		free(Root);
	}
}	
