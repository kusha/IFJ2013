#ifndef __TABLES_H__
#define __TABLES_H__

#include "common.h"
#include "str.h"
#include "ial.h"

// existFlag
#define MAY_NOT_EXIST 	0
#define SHOULD_EXIST 	1


// datatype defines in ial.h

#define MAX_TEMP_VARS	16	// 10^16 temp vars

// prototypes
typeData * getVariable(string * name, char existFlag);
typeData * getLiteral(int dataType, string * atribute);
typeData * getEmpty();

#endif
