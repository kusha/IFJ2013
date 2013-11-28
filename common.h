#ifndef __COMMON_H__
#define __COMMON_H__

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

#endif
