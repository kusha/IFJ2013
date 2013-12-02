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
**	Include main file dependencies.
**
** -------------------------------------------------------------------------*/


#ifndef __MAIN_H__
#define __MAIN_H__


#include "common.h"			// for common defines
#include "lexer.h"			// for file delegation
#include "parser.h"			// for parsing start
#include "interpreter.h"	// for interpretation start

#define HALT(CODE) /* -- Prints exit status code and returns value --*/	\
	if (DEBUG_FLAG) fprintf(stderr,"Exit with status code %i.\n",CODE);	\
	return CODE;


#endif
