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
	
	Lexer header file - define token names, structures and 2 functions:
		delegate file pointer from main to lexer
		generate token function for parser

	uses in main.c and in parser.c
*/

#ifndef __LEXER_H__
#define __LEXER_H__

#include "common.h"
#include "str.h"

//lexer datatypes and structures section

//common resources
extern int troubleLine;
extern int troubleColumn;
extern int troubleCharacter;

//must use macros for token names
//bcs in parser switch it'll be harder to use enum
#define PHP					0  // <?php
#define END					1  // EOF

#define IDENTIFIER			10 // (0..9|a..z|A..Z|_)+
#define IDENTIFIER_VARIABLE	11 // $ + IDENTIFIER

#define KEYWORD_ELSE		20 // else
#define KEYWORD_FUNCTION	21 // function
#define KEYWORD_IF			22 // if
#define KEYWORD_RETURN		23 // return
#define KEYWORD_WHILE		24 // while

#define LITERAL_NULL		30 // null
#define LITERAL_LOGICAL		31 // (false|true)
#define LITERAL_INETEGER	32 // 0..9+
#define LITERAL_DOUBLE		33 // 12.43 12e10 12.32E-3
#define LITERAL_STRING		34 // "\"Ahoj\n\$vete\x21'"

#define COMMA				40 // , (od Tomase)
#define SEMICOLON			41 // ; (od Tomase)
#define LEFT_BRACKET		42 // (
#define RIGHT_BRACKET		43 // )
#define LEFT_CURLY_BRACKET	44 // {
#define RIGHT_CURLY_BRACKET	45 // }

#define OPERATION_ASSIGN	50 // =
#define OPERATION_PLUS		51 // +
#define OPERATION_MINUS		52 // -
#define OPERATION_MULTIPLY	53 // *
#define OPERATION_DIVIDE	54 // /
#define OPERATION_CONCATEN	55 // .

#define COMPARE_IS			60 // ===
#define COMPARE_IS_NOT		61 // !==
#define COMPARE_LESS		62 // <
#define COMPARE_LESS_EQ		63 // <=
#define COMPARE_MORE		64 // >
#define COMPARE_MORE_EQ		65 // >=

#define LEXER_ERROR			-1 // lexer error code for parser
#define LEXER_START_ERROR	-2

//functions prototypes section
void delegateSourceFile(FILE *f);
int getToken(string *attribute);


#endif
