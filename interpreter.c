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
	
	Interpreter of 3AK instructions unit
*/

#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"

/* -- Debug for printing insttructions -------------------------------------------------*/

char * printInstr (int priority) {
	switch (priority){
		case I_STOP:		return "I_STOP		";
		case I_GOTO:		return "I_GOTO		";
		case I_GOTO_IF:		return "I_GOTO_IF	";
		case I_ASSIGN:		return "I_ASSIGN	";
		case I_MULTIPLY: 	return "I_MULTIPLY	";
		case I_DIVIDE: 		return "I_DIVIDE	";
		case I_PLUS: 		return "I_PLUS		";
		case I_MINUS: 		return "I_MINUS		";
		case I_CONCATEN: 	return "I_CONCATEN	";
		case I_C_LESS: 		return "I_C_LESS	";
		case I_C_MORE: 		return "I_C_MORE	";
		case I_C_LESS_EQ: 	return "I_C_LESS_EQ	";
		case I_C_MORE_EQ: 	return "I_C_MORE_EQ	";
		case I_C_IS: 		return "I_C_IS		";
		case I_C_IS_NOT: 	return "I_C_IS_NOT	";
		case I_CONVERT: 	return "I_CONVERT	";
		case I_READ: 		return "I_READ		";
		case I_WRITE: 		return "I_WRITE		";
		case I_STR_LEN: 	return "I_STR_LEN	";
		case I_SUB_STR: 	return "I_SUB_STR	";
		case I_FIND_STR: 	return "I_FIND_STR	";
		case I_SORT_STR: 	return "I_SORT_STR	";
		default:			return "I_UNDEFINED ";
	}
}

void printOperand (typeData * pointer) {
	int val;
	if (pointer==NULL){
		printf("-----");
		return;
	}
	switch ((*pointer).type) {
		case _NONE:
			//printf("<NONE>");
			printf("%p",pointer);
			break;

		case _NULL:
			printf("NULL");
			break;

		case _LOGICAL:
			val = (*pointer).valueOf.type_LOGICAL;
			if (val==0) {
				printf("false");
			} else {
				printf("true");
			}
			break;

		case _INTEGER:
			printf("%i", (*pointer).valueOf.type_INTEGER);
			break;

		case _DOUBLE:
			printf("%6f", (*pointer).valueOf.type_DOUBLE);
			break;

		case _STRING:
			printf("%s", strGetContent(&(*pointer).valueOf.type_STRING));
			break;

		case _FUNCTION:
			if ((*pointer).instruction == NULL) {
				printf("NULL");	//error
			} else {
				printf("%p", (*pointer).instruction);
			}
			break;
			
	}
}

/* ----------------------------------------------------------------------*/

#define DATA_TYPE(ADDRESS) (ADDRESS)->type
//#define VALUE(TYPE) currentInstr->addressTwo->valueOf.type_(TYPE)

void * typeFind(typeData* type){
switch (type->type){
	case _NULL:
		return &(type->valueOf.type_NULL);
	case _LOGICAL:
		return &(type->valueOf.type_LOGICAL);
	case _INTEGER:
		return &(type->valueOf.type_INTEGER);
	case _DOUBLE:
		return &(type->valueOf.type_DOUBLE);
	case _STRING:
		return &(type->valueOf.type_STRING);
	/*case _FUNCTION:
		return type->valueOf.type_FUNCTION;	
	*/
	default:
		return 12;
	}
}


int interpreterStart(typeList *instrList) {

	if (DEBUG_FLAG) printf("Interpeter start\n");

	if (DEBUG_FLAG) {
		printf("Interpeter debug mode\n");
		printf("Start print instructions\n");
		listFirst(instrList);
		while (instrList->active != NULL) {
			printf(CBLU"%s\t", printInstr(instrList->active->instr.instrCode));
			printOperand(instrList->active->instr.addressOne);
			printf("\t");
			printOperand(instrList->active->instr.addressTwo);
			printf("\t");
			printOperand(instrList->active->instr.addressThree);
			printf("\n"CNRM);
			listNext(instrList);
		}
		printf("Finish print instructions\n");
	}
	int gotoFlag = 0;
	
	listFirst(instrList);
	typeInstruction* currentInstr;
	while (1) {
		currentInstr = getCurrent(instrList);
		// printf(CBLU"PROCESSING %s\n"CNRM, printInstr(instrList->active->instr.instrCode));
		switch (currentInstr->instrCode) {
			
			case I_STOP:
				//return 0;
				break;

			case I_GOTO:
				//DEBUG!!!
				printf(CGRN"%s\t", printInstr(instrList->active->instr.instrCode));
				printOperand(instrList->active->instr.addressOne);
				printf("\t");
				printOperand(instrList->active->instr.addressTwo);
				printf("\t");
				printOperand(instrList->active->instr.addressThree);
				printf("\n"CNRM);
				gotoFlag = 1;

				listGoto(instrList,currentInstr->addressTwo->instruction);
				break;
				
			case I_GOTO_IF:
				//DEBUG!!!
				printf(CGRN"%s\t", printInstr(instrList->active->instr.instrCode));
				printOperand(instrList->active->instr.addressOne);
				printf("\t");
				printOperand(instrList->active->instr.addressTwo);
				printf("\t");
				printOperand(instrList->active->instr.addressThree);
				printf("\n"CNRM);
				gotoFlag = 1;

				//void * p=typeFind(currentInstr->addressOne);
				if(currentInstr->addressOne->type == _LOGICAL) {
					if(currentInstr->addressOne->valueOf.type_LOGICAL)
						listGoto(instrList,currentInstr->addressTwo->instruction);
					else
						listGoto(instrList,currentInstr->addressThree->instruction);
				} else {
					return S_EXPRESS_ERROR;
				} 
				break;
				 		
			case I_ASSIGN:
				if((DATA_TYPE(currentInstr->addressTwo)) == _NULL){
			 		currentInstr->addressOne->type = _NULL;
					void * p=typeFind(currentInstr->addressTwo);
					currentInstr->addressOne->valueOf.type_NULL = p;	
			 	}
				else if((DATA_TYPE(currentInstr->addressTwo)) == _LOGICAL){
			 		currentInstr->addressOne->type = _LOGICAL;
					void * p=typeFind(currentInstr->addressTwo);
					char *i=p;
					currentInstr->addressOne->valueOf.type_LOGICAL = *i;	
			 	}
				else if((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER){
					currentInstr->addressOne->type = _INTEGER;
					void * p=typeFind(currentInstr->addressTwo);
					int *i=p;
					currentInstr->addressOne->valueOf.type_INTEGER = *i;	  
				}
			 	else if((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE){
			 		currentInstr->addressOne->type = _DOUBLE;
					void * p=typeFind(currentInstr->addressTwo);
					double *i=p;
					currentInstr->addressOne->valueOf.type_DOUBLE = *i;	
			 	}
			 	else if((DATA_TYPE(currentInstr->addressTwo)) == _STRING){
			 		currentInstr->addressOne->type = _STRING;
					void * p=typeFind(currentInstr->addressTwo);
					string *i=p;
					currentInstr->addressOne->valueOf.type_STRING = *i;	
			 	}
			 	else
			 		return 12;
				
				break;	
				
			case I_PLUS:
				if(((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER)&&((DATA_TYPE(currentInstr->addressThree))== _INTEGER)){
					currentInstr->addressOne->type = _INTEGER;
					void * p=typeFind(currentInstr->addressTwo);
					void * q=typeFind(currentInstr->addressThree);   
					int *i = p;
					int *j = q;
					currentInstr->addressOne->valueOf.type_INTEGER = (*i)+(*j);	  	
				}
				else if ((((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) 
						 || (((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE))
						 || (((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE))){
					currentInstr->addressOne->type = _DOUBLE;
					void * p=typeFind(currentInstr->addressTwo);
					void * q=typeFind(currentInstr->addressThree);
	        double result;
					if (((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)){
							double* i = p;
							double* j = q;
							result = (*i)+(*j);
					}
					else if	(((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)){
							double* i = p;
							int* j = q;
							result = (*i)+(*j);
							}
					else if	(((DATA_TYPE(currentInstr->addressTwo))== _INTEGER) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)){
							int* i = p;
							double* j = q;
							result = (*i)+(*j);
							}
					currentInstr->addressOne->valueOf.type_DOUBLE = result;								
				}	  
				break;	 
			
			case I_MINUS:
				if(((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER)&&((DATA_TYPE(currentInstr->addressThree))== _INTEGER)){
					currentInstr->addressOne->type = _INTEGER;
					void * p=typeFind(currentInstr->addressTwo);
					void * q=typeFind(currentInstr->addressThree);   
					int *i = p;
					int *j = q;
					currentInstr->addressOne->valueOf.type_INTEGER = (*i)-(*j);				
				}
			else if ((((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) 
						 || (((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE))
						 || (((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE))){
					currentInstr->addressOne->type = _DOUBLE;
					void * p=typeFind(currentInstr->addressTwo);
					void * q=typeFind(currentInstr->addressThree);
	        double result;
					if (((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)){
							double* i = p;
							double* j = q;
							result = (*i)-(*j);
					}
					else if	(((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)){
							double* i = p;
							int* j = q;
							result = (*i)-(*j);
							}
					else if	(((DATA_TYPE(currentInstr->addressTwo))== _INTEGER) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)){
							int* i = p;
							double* j = q;
							result = (*i)-(*j);
							}
					currentInstr->addressOne->valueOf.type_DOUBLE = result;								
				}	  
				break;
			
			case I_MULTIPLY:
				if(((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER)&&((DATA_TYPE(currentInstr->addressThree))== _INTEGER)){
					currentInstr->addressOne->type = _INTEGER;
					void * p=typeFind(currentInstr->addressTwo);
					void * q=typeFind(currentInstr->addressThree);   
					int *i = p;
					int *j = q;
					currentInstr->addressOne->valueOf.type_INTEGER = (*i)*(*j);					
				}
				else if ((((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) 
						 || (((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE))
						 || (((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE))){
					currentInstr->addressOne->type = _DOUBLE;
					void * p=typeFind(currentInstr->addressTwo);
					void * q=typeFind(currentInstr->addressThree);
	        double result;
					if (((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)){
							double* i = p;
							double* j = q;
							result = (*i)*(*j);
					}
					else if	(((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)){
							double* i = p;
							int* j = q;
							result = (*i)*(*j);
							}
					else if	(((DATA_TYPE(currentInstr->addressTwo))== _INTEGER) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)){
							int* i = p;
							double* j = q;
							result = (*i)*(*j);
							}
					currentInstr->addressOne->valueOf.type_DOUBLE = result;								
				}	    
				break;
			
			case I_DIVIDE:
				if (((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER)
						 && (((DATA_TYPE(currentInstr->addressThree))== _INTEGER))
						 || (((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)) 
						 || (((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE))
						 || (((DATA_TYPE(currentInstr->addressTwo)) == _DOUBLE) 
						 && ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE))){
					currentInstr->addressOne->type = _DOUBLE;
					void * p=typeFind(currentInstr->addressTwo);
					void * q=typeFind(currentInstr->addressThree);
	        double result;
	        
	        if (((DATA_TYPE(currentInstr->addressTwo)) == _INTEGER)&&
					 (((DATA_TYPE(currentInstr->addressThree))== _INTEGER))){
					 	int* i=p;
					 	int* j=q;
					 	result = (*i)/(*j);
					 		printf("%f\n\n",*i );
					 		printf("%f\n\n",*j );
					}
					else if (((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)){
							double* i = p;
							double* j = q;
							result = (*i)/(*j);
					}
					else if	(((DATA_TYPE(currentInstr->addressTwo))== _DOUBLE) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _INTEGER)){
							double* i = p;
							int* j = q;
							result = (*i)/(*j);
					}
					else if	(((DATA_TYPE(currentInstr->addressTwo))== _INTEGER) 
							&& ((DATA_TYPE(currentInstr->addressThree))== _DOUBLE)){
							int* i = p;
							double* j = q;
							result = (*i)/(*j);
					}
					currentInstr->addressOne->valueOf.type_DOUBLE = result;								
				}	  	  
				break;
			
			case I_CONCATEN:
				if (((DATA_TYPE(currentInstr->addressTwo))== _STRING) 
					&& ((DATA_TYPE(currentInstr->addressThree))== _STRING)){
					currentInstr->addressOne->type = _STRING;
					void * p=typeFind(currentInstr->addressTwo);
					void * q=typeFind(currentInstr->addressThree);
					string *i=p;
					string *j=q;
					string result;
					asprintf(&(result.str),"%s%s",(i->str),(j->str));
					currentInstr->addressOne->valueOf.type_STRING = result;						
				}
				break;
			
			
			case I_C_IS:	
				if((DATA_TYPE(currentInstr->addressTwo)) == (DATA_TYPE(currentInstr->addressThree))){
					currentInstr->addressOne->type = _LOGICAL;
					void *p=typeFind(currentInstr->addressTwo);
					void *q=typeFind(currentInstr->addressThree);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
							printf("NULL %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);
					} 							
				  else if(DATA_TYPE(currentInstr->addressTwo) == _LOGICAL){
				  	char *i=p;
				  	char *j=q;
				
						if((*i)==(*j)){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("LOG %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				  else if(DATA_TYPE(currentInstr->addressTwo) == _INTEGER){
				  	int *i=p;
				  	int *j=q;
				
						if((*i)==(*j)){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("INT %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				  else if(DATA_TYPE(currentInstr->addressTwo) == _DOUBLE){
				  	double *i=p;
				  	double *j=q;
				
						if((*i)==(*j)){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("DOUB %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				  else if(DATA_TYPE(currentInstr->addressTwo) == _STRING){
						string *i=p;
				  	string *j=q;
						if((strCompare(i,j))==0){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("STR %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				}
				else{
				currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				printf("konec %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				}
			break;
	
	/*		case I_C_IS_NOT:
		*/
			
			case I_C_LESS:
				if((DATA_TYPE(currentInstr->addressTwo)) == (DATA_TYPE(currentInstr->addressThree))){
					currentInstr->addressOne->type = _LOGICAL;
					void *p=typeFind(currentInstr->addressTwo);
					void *q=typeFind(currentInstr->addressThree);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 0;
							printf("NULL %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);
					} 							
				  else if(DATA_TYPE(currentInstr->addressTwo) == _LOGICAL){
				  	char *i=p;
				  	char *j=q;
				
						if((*i)<(*j)){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("LOG %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				  else if(DATA_TYPE(currentInstr->addressTwo) == _INTEGER){
				  	int *i=p;
				  	int *j=q;
				
						if((*i)<(*j)){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("INT %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				  else if(DATA_TYPE(currentInstr->addressTwo) == _DOUBLE){
				  	double *i=p;
				  	double *j=q;
				
						if((*i)<(*j)){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("DOUB %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				  else if(DATA_TYPE(currentInstr->addressTwo) == _STRING){
						string *i=p;
				  	string *j=q;
						if((strCompare(i,j))<0){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("STR %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				}
				else{
				currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				printf("konec %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				}
			break;
				
			case I_C_LESS_EQ:
				if((DATA_TYPE(currentInstr->addressTwo)) == (DATA_TYPE(currentInstr->addressThree))){
					currentInstr->addressOne->type = _LOGICAL;
					void *p=typeFind(currentInstr->addressTwo);
					void *q=typeFind(currentInstr->addressThree);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1;
							printf("NULL %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);
					} 							
				  else if(DATA_TYPE(currentInstr->addressTwo) == _LOGICAL){
				  	char *i=p;
				  	char *j=q;
				
						if((*i)<=(*j)){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("LOG %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				  else if(DATA_TYPE(currentInstr->addressTwo) == _INTEGER){
				  	int *i=p;
				  	int *j=q;
				
						if((*i)<=(*j)){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("INT %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				  else if(DATA_TYPE(currentInstr->addressTwo) == _DOUBLE){
				  	double *i=p;
				  	double *j=q;
				
						if((*i)<=(*j)){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("DOUB %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				  else if(DATA_TYPE(currentInstr->addressTwo) == _STRING){
						string *i=p;
				  	string *j=q;
						if((strCompare(i,j))<=0){ 
							currentInstr->addressOne->valueOf.type_LOGICAL = 1; 							
						}
						else{                                                                        
				  		currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				  	}
				  	printf("STR %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				  }
				}
				else{
				currentInstr->addressOne->valueOf.type_LOGICAL = 0;
				printf("konec %d\n\n",currentInstr->addressOne->valueOf.type_LOGICAL);

				}
			break;
/*			
			case I_C_MORE:
			
			
			case I_C_MORE_EQ:
				
*/			
			case I_CONVERT:
				if((DATA_TYPE(currentInstr->addressOne)) != (DATA_TYPE(currentInstr->addressTwo))){
					void *p=typeFind(currentInstr->addressOne);		  
					if (DATA_TYPE(currentInstr->addressTwo) == _NULL){ 
							currentInstr->addressOne->valueOf.type_NULL = NULL;
					} 							
					else if(DATA_TYPE(currentInstr->addressTwo)==_LOGICAL){
						char *i=p;
						currentInstr->addressOne->valueOf.type_LOGICAL = *i;
					}
					else if(DATA_TYPE(currentInstr->addressTwo)==_INTEGER){
						int *i=p;
						currentInstr->addressOne->valueOf.type_INTEGER = *i;
					}
					else if(DATA_TYPE(currentInstr->addressTwo)==_DOUBLE){
						double *i=p;
						currentInstr->addressOne->valueOf.type_DOUBLE = *i;
					}
					else if(DATA_TYPE(currentInstr->addressTwo)==_STRING){
						string *i=p;
						currentInstr->addressOne->valueOf.type_STRING = *i;
					}
				currentInstr->addressOne->type = currentInstr->addressTwo->type;
			  }
			break;
			
			
			case I_READ:
				if(1){
					char *str;
					scanf("%s",&str);
					currentInstr->addressOne->valueOf.type_STRING.str = str;				
					printf("%s",currentInstr->addressOne->valueOf.type_STRING);	
					}
				break;
			
			case I_WRITE:
				printf("asd"); 
			  	if (DATA_TYPE(currentInstr->addressTwo) == _NULL){ 
							currentInstr->addressOne->valueOf.type_NULL = NULL;
					} 							
					else if(DATA_TYPE(currentInstr->addressTwo)==_LOGICAL){
							printf("%c",currentInstr->addressOne->valueOf.type_LOGICAL);
					}
					else if(DATA_TYPE(currentInstr->addressTwo)==_INTEGER){
						printf("%d",currentInstr->addressOne->valueOf.type_INTEGER);
					}
					else if(DATA_TYPE(currentInstr->addressTwo)==_DOUBLE){
						printf("%f",currentInstr->addressOne->valueOf.type_DOUBLE);
					}
					else if(DATA_TYPE(currentInstr->addressTwo)==_STRING){
					printf("%s",currentInstr->addressOne->valueOf.type_STRING.str);
					}			
				break;
/*			
			case I_STR_LEN:
			
			case I_SUB_STR: 
			
			case I_FIND_STR: 
			
			case I_SORT_STR:
	*/		
		}

		if (!gotoFlag) {
			printf(CGRN"%s\t", printInstr(instrList->active->instr.instrCode));
			printOperand(instrList->active->instr.addressOne);
			printf("\t");
			printOperand(instrList->active->instr.addressTwo);
			printf("\t");
			printOperand(instrList->active->instr.addressThree);
			printf("\n"CNRM);
		} else {
			gotoFlag = 0;
		}

		if (currentInstr->instrCode == I_STOP) break;
		listNext(instrList);
	}


	// if (DEBUG_FLAG) {
	// 	printf("Interpeter debug mode\n");
	// 	printf("Start print instructions\n");
	// 	listFirst(instrList);
	// 	while (instrList->active != NULL) {
	// 		printf(CGRN"%s\t", printInstr(instrList->active->instr.instrCode));
	// 		printOperand(instrList->active->instr.addressOne);
	// 		printf("\t");
	// 		printOperand(instrList->active->instr.addressTwo);
	// 		printf("\t");
	// 		printOperand(instrList->active->instr.addressThree);
	// 		printf("\n"CNRM);
	// 		listNext(instrList);
	// 	}
	// 	printf("Finish print instructions\n");
	// }

	if (DEBUG_FLAG) printf("Interpeter finish\n");
	
	return SUCCESS;
}
