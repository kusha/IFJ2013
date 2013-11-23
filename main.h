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
	
	Main header file - define common macros (like exit codes)

	uses probably in all program units
*/

#define DEBUG_FLAG		1

//exit codes:
#define SUCCESS 		0
#define LEXICAL_ERROR 	1
#define PARSER_ERROR 	2
//semantic errors
#define S_FUNC_ERROR 	3
#define S_PARAM_ERROR 	4
#define S_DECLAR_ERROR 	5
#define S_ZERODIV_ERROR	10
#define S_TYPE_ERROR 	11
#define S_EXPRESS_ERROR	12
#define S_OTHER_ERROR 	13
//interpret error
#define INTERNAL_ERROR 	99
