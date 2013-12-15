/* -- IFJ project 2013 -------------------------------------------------------
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
**	Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
**
**	Tables header file. All functions generates nodes in BST.
**
** -------------------------------------------------------------------------*/


#ifndef __TABLES_H__
#define __TABLES_H__


/* -- Includes part --------------------------------------------------------*/
#include "common.h"		
#include "str.h"		
#include "ial.h"	

/* -- Macro definitions ----------------------------------------------------*/
#define MAY_NOT_EXIST 		0
#define SHOULD_EXIST 		1	
#define SHOULD_NOT_EXIST 	2
#define MAX_TEMP_VARS		16	

/* -- Tables common functions ----------------------------------------------*/
typeData * getVariable(typeNodePtr * table, string * name, char existFlag);		
typeData * getLiteral(typeNodePtr * table, int dataType, string * atribute);	
typeData * getEmpty(typeNodePtr * table);					
typeNodePtr createTable();

#endif
