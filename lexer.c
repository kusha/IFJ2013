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
#include <stdbool.h> // true
#include <string.h> //strcmp
#include <stdlib.h> //atoi
#include "main.h"
#include "lexer.h"

FILE *source;

int isKeyWord(char* word){
  if((strcmp("if", word) == 0)){
    return KEYWORD_IF;
  }else if((strcmp("else", word) == 0)){
    return KEYWORD_ELSE;
  }else if((strcmp("funciton", word) == 0)){
    return KEYWORD_FUNCTION;  
  }else if((strcmp("return", word) == 0)){  
    return KEYWORD_RETURN;  
  }else if((strcmp("while", word) == 0)){  
    return KEYWORD_WHILE;
  }else if((strcmp("true", word) == 0) || (strcmp("false", word) == 0)){  
    return LITERAL_LOGICAL;  
  }else if((strcmp("null", word) == 0)){  
    return LITERAL_NULL;
  }else{
    return IDENTIFIER;  
  }
   
}

void delegateSourceFile(FILE *f) {
	source = f;
}

int getToken(string *attribute) {
	//in case of error reuturn LEXER_ERROR;
	int c; //int bcs of EOF
	strClear(attribute);
	int state = S_START;
  int i = 0, backs = i, x = i, xx = i, star = i, linec = i, e = i, j = i, space = i; // pomocný
  char hexa[3];      // hexa číslo -- čtou se jen dva znaky(x00) -- má velikost 3 protože se tam ukládá i převedený číslo a to může být max 255(xFF) = 3 znaky
 	unsigned int s;    // převedeny hexa číslo na int            
  
  while(true){
    if((c = getc(source)) == EOF){
		  return END_OF_FILE;	
    }    
    switch(state){
      case S_START: 
        if (isspace(c)){
          state = S_START;                      // mezery se ignoruji
	      }else if(isalpha(c) || c == '_'){
          strAddChar(attribute, c);             
	        state = IDENTIFIER;                   // znaky
	      }else if(isdigit(c)){                   
          strAddChar(attribute, c);
	        state = LITERAL_INETEGER;             // čísla 
	      }else if(c == '$'){    
	        strAddChar(attribute, c);             
	        state = IDENTIFIER_VARIABLE;          // identifikator
        }else if(c == '"'){ 
          //strAddChar(attribute, c);
          state = LITERAL_STRING;               // string
        }else if(c == '='){ 
          strAddChar(attribute, c);
          state = OPERATION_ASSIGN;             // přiřazení nebo porovnání je rovno
        }else if (c == '+'){
	        strAddChar(attribute, c);
          return OPERATION_PLUS;                // plus
	      }else if (c == '-'){
          strAddChar(attribute, c);
          return OPERATION_MINUS;               // minus
        }else if(c == '/'){
          //strAddChar(attribute, c);
   	      state = OPERATION_DIVIDE; 	          // komentar nebo dělení
   	    }else if(c == '*'){
          strAddChar(attribute, c);             
   	      return OPERATION_MULTIPLY; 	          // krát
        }else if(c == '.'){
          strAddChar(attribute, c);
   	      return OPERATION_CONCATEN;            // zřetězení
        }else if(c == '!'){
          strAddChar(attribute, c);
   	      state = COMPARE_IS_NOT;               // vykřičník (porovnání IS_NOT)
        }else if(c == '<'){
          strAddChar(attribute, c);
   	      state = COMPARE_LESS;                 // porovnání je menší nebo začátek php (<?php)
        }else if(c == '>'){
          strAddChar(attribute, c);
   	      state = COMPARE_MORE;                 // porovnání je větší
        }else if(c == '{'){ 
          strAddChar(attribute, c);
          return LEFT_CURLY_BRACKET;            // levá složená závorka
        }else if(c == '}'){ 
          strAddChar(attribute, c);             
          return RIGHT_CURLY_BRACKET;           // pravá složená závorka
        }else if(c == '('){ 
          strAddChar(attribute, c);             
          return LEFT_BRACKET;                  // levá závorka
        }else if(c == ')'){ 
          strAddChar(attribute, c);             
          return RIGHT_BRACKET;                 // pravá závorka
	      }else if(c == ';'){
          strAddChar(attribute, c);
          return SEMICOLON;                     // středník
	      }else if(c == ','){
          strAddChar(attribute, c);
          return COMMA;                         // čárka	      
        }else{
          return LEXER_ERROR;
        }
        i++;
      break;
      
      case COMPARE_LESS:
        if(c == '?'){
          state = PHP;
        }else if(c == '='){ 
          strAddChar(attribute, c);
          return COMPARE_LESS_EQ;         
        }else{
          ungetc(c, source);
          return COMPARE_LESS;
        }
        i++;
      break;
      case COMPARE_MORE:
        if(c == '='){
          strAddChar(attribute, c);
          return COMPARE_MORE_EQ;          
        }else{
          ungetc(c, source);
          return COMPARE_MORE;         
        }
        i++;
      break;
      case COMPARE_IS_NOT:
        if(c == '='){
          strAddChar(attribute, c);
          if(i == 2){
            return COMPARE_IS_NOT;
          }
          i++;
        }
      break;
      case OPERATION_ASSIGN:
        if(c == '='){
          strAddChar(attribute, c);
          if(i == 2){
            return COMPARE_IS;
          }
          i++;                    
        }else{
          ungetc(c, source);
          return OPERATION_ASSIGN;
        }  
      break;
      case IDENTIFIER:
        if(isalnum(c) || (c == '_')){
          strAddChar(attribute, c);
          i++;    
        }else{
          ungetc(c, source);
          return isKeyWord(attribute->str);        
        }
      break; 
      case IDENTIFIER_VARIABLE:
         if(isalnum(c) || (c == '_')){
          strAddChar(attribute, c);
          i++;    
        }else{
          ungetc(c, source);
          return IDENTIFIER_VARIABLE;        
        }       
      break;
      case LITERAL_INETEGER:
        if(isdigit(c)){
          strAddChar(attribute, c);        
        }else if(!isdigit(c) && (c != '.') && (c != 'e') && (c != 'E')){
          ungetc(c, source);
          return LITERAL_INETEGER;         
        }else if(c == '.'){
          state = LITERAL_DOUBLE;
          strAddChar(attribute, c); 
        }else if(c == 'e' || c == 'E'){
          state = LITERAL_DOUBLE;
          strAddChar(attribute, c);
          e++;  
        }
        i++;
      break;
      case OPERATION_DIVIDE:
        if(c == '*' && star == 0){
          star = 1;
        }else if(star > 0){
          if(c == '*' && star == 1){
            star = 2;
          }else if(c == '/' && star == 2){
            star = 0;
		        i = 0;
            state = S_START;
          }
        }else if(c == '/' && linec == 0){
          linec = 1;         
        }else if(linec == 1 && i >= 2){
          if(c == 10){                                 // zalomení řádku
            linec = 0;
		        i = 0;
            state = S_START;
          }
        }else{
          strAddChar(attribute, '/'); 
          return OPERATION_DIVIDE;
        }
        i++;
      break;
      case LITERAL_STRING:
        if(backs == 0 && x == 0 && c != '\\' && c != '"' && c != '$'){
          if(c > 31){
            strAddChar(attribute, c);
          }
        }else if(c == '"' && backs == 0){
          return LITERAL_STRING;
        }else if(c == '\\' && backs == 0){
          backs = 1;      
        }else if(backs == 1){
          if(c == 'n'){
            strAddChar(attribute, 10);
          }else if(c == 't'){
            strAddChar(attribute, 11);
          }else if(c == '\\' || c == '"' || c == '$'){
            strAddChar(attribute, c);
          }else if(c == 'x'){
            x = 1;
            xx = 0;
          }
          backs = 0;
        }else if(x > 0){
          if((xx <= 1) && (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c == ' '))){
            hexa[xx] = c;
	          xx++;
	          if(c == ' '){
              space = 1;
            }
          }
          x++;
          if(xx >= 2){	// už mam načteny dva znaky
            sscanf(hexa, "%x", &s);     
            sprintf(hexa, "%d", s);		
		        while(hexa[j] != '\0'){
              strAddChar(attribute, hexa[j]); 
              j++;     
            }
            if(space == 1){
              strAddChar(attribute, ' ');
            } 		
		        x = 0;
		        xx = 0;
		        j = 0;
		        hexa[0] = 0;
	   	      hexa[1] = 0;
	   	      hexa[2] = 0;
          }           
        }
        i++;
      break;
      
      case LITERAL_DOUBLE:
        if((c == 'e' || c == 'E') && (e == 0)){
          e++;
          strAddChar(attribute, c);
        }else if((c == '+' || c == '-') && (e != 0)){
          strAddChar(attribute, c);
        }else if(isdigit(c)){
          strAddChar(attribute, c);
        }else{
          ungetc(c, source);
          e = 0;
          return LITERAL_DOUBLE;       
        }
        i++;
      break;
      
      case PHP:
        if((c == 'p' || c == 'P') && (i == 2 || i == 4)){
          state = PHP;  
          strAddChar(attribute, c);
          if(i == 4){
            return PHP;          
          }
        }else if((c == 'h' || c == 'H') && i == 3){
          state = PHP;
          strAddChar(attribute, c);
        }
        i++;
      break;
                    
    }
  }
	
	return SUCCESS;
}
