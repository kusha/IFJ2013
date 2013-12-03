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
**	Tables header file. All functions generates nodes in BST.
**
** -------------------------------------------------------------------------*/


#ifndef __TABLES_H__
#define __TABLES_H__


#include "common.h"		// for common defines
#include "str.h"		// for string datatype
#include "ial.h"		// for binary search tree

#define MAY_NOT_EXIST 		0	// variables declration/assign
#define SHOULD_EXIST 		1	// variables in expressions
#define SHOULD_NOT_EXIST 	2	// first only declaration (functions)

#define MAX_TEMP_VARS		16	// maximum number of temp vars (10^16)

typeData * getVariable(typeNodePtr * table, string * name, char existFlag);		// generates var
typeData * getLiteral(typeNodePtr * table, int dataType, string * atribute);	// generates literal
typeData * getEmpty(typeNodePtr * table);										// generates temp
typeNodePtr createTable(); //returns new table

#endif
