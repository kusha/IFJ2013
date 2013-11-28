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
	
*/

#include <stdio.h> //getc
#include <ctype.h> //isspace
#include "lexer.h"

#define DEBUG_FLAG 0

// S is STATE
#define S_START			0
#define S_IDENTIFIER	1
#define S_VARIABLE_D	2
#define S_VARIABLE_ID	3
#define S_INTEGER		4
#define S_DOUBLE		5
#define S_EXPONENT_E	6
#define S_EXPONENT		7
#define S_STRING		8
#define S_ESCAPE		9
#define S_SYMBOL_CODE1	10
#define S_SYMBOL_CODE2	11
#define S_EQUAL			12
#define S_CMP_IS		13
#define S_CMP_NOT1		14
#define S_CMP_NOT2		15
#define S_LESS			16
#define S_PHP1			17
#define S_PHP2			18
#define S_PHP3			19
#define S_MORE			20
#define S_DIVIDE		21
#define S_MLINE_CMNT	22
#define S_LINE_CMNT		23
#define S_MLINE_ESC		24


FILE *source;

void delegateSourceFile(FILE *f) {
	source = f;
}

int checkKeywords(string *attribute) {
	if (strCompareConst(attribute, "else")) {
		return KEYWORD_ELSE;
	} else if (strCompareConst(attribute, "function")) {
		return KEYWORD_FUNCTION;
	} else if (strCompareConst(attribute, "if")) {
		return KEYWORD_IF;
	} else if (strCompareConst(attribute, "return")) {
		return KEYWORD_RETURN;
	} else if (strCompareConst(attribute, "while")) {
		return KEYWORD_WHILE;
	} else if (strCompareConst(attribute, "false")) {
		return LITERAL_LOGICAL;
	} else if (strCompareConst(attribute, "null")) {
		return LITERAL_NULL;
	} else if (strCompareConst(attribute, "true")) {
		return LITERAL_LOGICAL;
	} else {				//maybe there will be std functions
		return IDENTIFIER;
	}
}

char hex;

int troubleLine = 1;
int troubleColumn = 1;
int troubleCharacter;

int getToken(string *attribute) {
	//in case of error reuturn LEXER_ERROR;
	int character; //int bcs of EOF
	int state = S_START;
	while (1) {
		character = getc(source);

		troubleCharacter = character;
		if (character=='\n') {
			troubleLine++;
			troubleColumn = 1;
		} else {
			troubleColumn++;
		}

		switch (state) {
			case S_START:
				if (isspace(character)) {
					state = S_START;
				} else if (isalpha(character)||character=='_') {
					strAddChar(attribute, character);
					state = S_IDENTIFIER;
				} else if (character=='$') {
					strAddChar(attribute, character); //is dollar at start of varid needed?
					state = S_VARIABLE_D;
				} else if (isdigit(character)) {
					strAddChar(attribute, character);
					state = S_INTEGER;
				} else if (character==',') {
					return COMMA;
				} else if (character==';') {
					return SEMICOLON;
				} else if (character=='(') {
					return LEFT_BRACKET;
				} else if (character==')') {
					return RIGHT_BRACKET;
				} else if (character=='{') {
					return LEFT_CURLY_BRACKET;
				} else if (character=='}') {
					return RIGHT_CURLY_BRACKET;
				} else if (character=='"') {
					state = S_STRING;
				} else if (character==EOF) {
					return END;
				} else if (character=='=') {
					state = S_EQUAL;
				} else if (character=='+') {
					return OPERATION_PLUS;
				} else if (character=='-') {
					return OPERATION_MINUS;
				} else if (character=='*') {
					return OPERATION_MULTIPLY;
				} else if (character=='.') {
					return OPERATION_CONCATEN;
				} else if (character=='!') {
					state = S_CMP_NOT1;
				} else if (character=='<') {
					state = S_LESS;
				} else if (character=='>') {
					state = S_MORE;
				} else if (character=='/') {
					state = S_DIVIDE;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;

			case S_IDENTIFIER:
				if (isalnum(character)||character=='_') {
					strAddChar(attribute, character);
				} else {
					ungetc(character, source);
					return checkKeywords(attribute);
				}
				break;

			case S_VARIABLE_D:
				if (isalpha(character)||character=='_') {
					strAddChar(attribute, character);
					state = S_VARIABLE_ID;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;

			case S_VARIABLE_ID:
				if (isalnum(character)||character=='_') {
					strAddChar(attribute, character);
				} else {
					ungetc(character, source);
					return IDENTIFIER_VARIABLE; //no checkword check in vars?
				}
				break;

			case S_INTEGER:
				if (isdigit(character)) {
					strAddChar(attribute, character);
				} else if (character=='e'||character=='E') {
					strAddChar(attribute, character); //what add at e/E for scanf?
					state = S_EXPONENT_E;
				} else if (character=='.') {
					strAddChar(attribute, character);
					state = S_DOUBLE;
				} else {
					ungetc(character, source);
					return LITERAL_INETEGER;
				}
				break;

			case S_DOUBLE:
				if (isdigit(character)) {
					strAddChar(attribute, character);
				} else if (character=='e'||character=='E') {
					strAddChar(attribute, character); //what add at e/E for scanf?
					state = S_EXPONENT_E;
				} else {
					ungetc(character, source);
					return LITERAL_DOUBLE;
				}
				break;

			case S_EXPONENT_E:
				if (character=='+'||character=='-'||isdigit(character))  {
					strAddChar(attribute, character);
					state = S_EXPONENT;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;
				
			case S_EXPONENT:
				if (isdigit(character)) {
					strAddChar(attribute, character);
				} else {
					ungetc(character, source);
					return LITERAL_DOUBLE;
				}
				break;
				
			case S_STRING:
				hex = 0x00;
				if (character=='"') {
					return LITERAL_STRING;
				} else if (character=='\\') {
					state = S_ESCAPE;
				} else if (character>31&&character!='$') {
					strAddChar(attribute, character);
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;
				
			case S_ESCAPE:
				if (character=='x') {
					state = S_SYMBOL_CODE1;
				} else if (character=='$') {
					strAddChar(attribute, '$');
					state = S_STRING;
				} else if (character=='n') {
					strAddChar(attribute, '\n');
					state = S_STRING;
				} else if (character=='t') {
					strAddChar(attribute, '\t');
					state = S_STRING;
				} else if (character=='\\') {
					strAddChar(attribute, '\\');
					state = S_STRING;
				} else if (character=='"') {
					strAddChar(attribute, '\"');
					state = S_STRING;
				} else if (character>31) {
					strAddChar(attribute, '\\');	//add \\ (esc for \) and character
					strAddChar(attribute, character);
					state = S_STRING;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;
				
			case S_SYMBOL_CODE1:
				if (isdigit(character)) {
					hex += (character - '0');
					state = S_SYMBOL_CODE2;
				} else if (character>='a'&&character<='f') {
					hex += (character - 'a' + 10);
					state = S_SYMBOL_CODE2;
				} else if (character>='A'&&character<='F') {
					hex += (character - 'A' + 10);
					state = S_SYMBOL_CODE2;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;

			case S_SYMBOL_CODE2:
				hex = hex << 4;
				if (isdigit(character)) {
					hex += (character - '0');
					strAddChar(attribute, hex);
					state = S_STRING;
				} else if (character>='a'&&character<='f') {
					hex += (character - 'a' + 10);
					strAddChar(attribute, hex);
					state = S_STRING;
				} else if (character>='A'&&character<='F') {
					hex += (character - 'A' + 10);
					strAddChar(attribute, hex);
					state = S_STRING;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;
				
			case S_EQUAL:
				if (character=='=') {
					state = S_CMP_IS;
				} else {
					ungetc(character, source);
					return OPERATION_ASSIGN;
				}
				break;
				
			case S_CMP_IS:
				if (character=='=') {
					return COMPARE_IS;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;
				
			case S_CMP_NOT1:
				if (character=='=') {
					state = S_CMP_NOT2;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;
				
			case S_CMP_NOT2:
				if (character=='=') {
					return COMPARE_IS_NOT;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;
				
			case S_LESS:
				if (character=='?') {
					state = S_PHP1;
				} else if (character=='=') {
					return COMPARE_LESS_EQ;
				} else {
					ungetc(character, source);
					return COMPARE_LESS;
				}
				break;
				
			case S_PHP1:
				if (character=='p') { //is <?php case sensitive?!
					state = S_PHP2;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;

			case S_PHP2:
				if (character=='h') {
					state = S_PHP3;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;
			
			case S_PHP3:
				if (character=='p') {
					return PHP;
				} else {
					if (DEBUG_FLAG)
						printf("LEXER_ERROR at %c [%i] with state=%i\n", \
							character,character,state);
					return LEXER_ERROR;
				}
				break;

			case S_MORE:
				if (character=='=') {
					return COMPARE_MORE_EQ;
				} else {
					ungetc(character, source);
					return COMPARE_MORE;
				}
				break;
				
			case S_DIVIDE:
				if (character=='/') {
					state = S_LINE_CMNT;
				} else if (character=='*') {
					state = S_MLINE_CMNT;
				} else {
					ungetc(character, source);
					return OPERATION_DIVIDE;
				}
				break;
				
			case S_MLINE_CMNT:
				if (character=='*')  {
					state = S_MLINE_ESC;
				} //no error check (what a hell in comments?)
				break;
				
			case S_MLINE_ESC:
				if (character=='/')  {
					state = S_START;
				} else {
					state = S_MLINE_CMNT;
				}
				break;
				
			case S_LINE_CMNT:
				if (character=='\n')  {
					state = S_START;
				} //no error check (what a hell in comments?)
				break;
		}
	}
	return SUCCESS;
}
