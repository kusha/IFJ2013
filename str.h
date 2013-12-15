/* -- IFJ project 2013 ------------------------------------------------------
**
**	Interpreter of IFJ2013 language
**	4.11.2013 - 15.12.2013
**
**	Team 13 (b/3/I):
**
**	Bank Tomáš			<xbankt00@stud.fit.vutbr.cz>
** +Birger Mark			<xbirge00@stud.fit.vutbr.cz>
**	Botka Roland		<xbotka00@stud.fit.vutbr.cz>
**	Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
** +Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
**
**	String support functions.
**
** -------------------------------------------------------------------------*/


#ifndef __STR_H__
#define __STR_H__


#include "common.h"

#define STR_SUCCESS		0 //successfull operation
#define STR_ERROR		1 //allocation error/another
#define STR_ALLOC_DELTA	8 //allocation block size

typedef struct {
	char* str;		//string \0 at the end
	int length;		//len of the string
	int allocated;	//allocated memory size
} string;

int strInit(string *s);
void strFree(string *s);
void strClear(string *s);
int strAddChar(string *s, char c);
int strCompareConst(string *s1, char* s2);
int strCompare(string *s1, string *s2);
int strCopy(string *s1, string *s2);
char * strGetContent (string *s);


#endif
