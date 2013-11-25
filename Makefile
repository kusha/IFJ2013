# IFJ project 2013
# Interpreter of IFJ2013 language
# 4.11.2013 - 15.12.2013

# Team 13 (b/3/I):
# Bank Tomáš			<xbankt00@stud.fit.vutbr.cz>
# Birger Mark			<xbirge00@stud.fit.vutbr.cz>
# Botka Roland		<xbotka00@stud.fit.vutbr.cz>
# Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
# Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>

# Main make file

CC = gcc
CFLAGS = -std=c99 -W -Wall -pedantic
OUTPUT = ifj

all: $(OUTPUT)


# $(OUTPUT): main.o lexer.o parser.o
# 	$(CC) main.o lexer.o parser.o -o $(OUTPUT)

# main.o: main.c
# 	$(CC) $(CFLAGS) main.c

# lexer.o: lexer.c
# 	$(CC) $(CFLAGS) lexer.c

# parser.o: parser.c
# 	$(CC) $(CFLAGS) parser.c

# clean:
# 	rm -rf *.o $(OUTPUT)


$(OUTPUT): str.o lexer.o instructions.o interpreter.o parser.o main.o 
	$(CC) $(CFLAGS) -o $(OUTPUT) str.o lexer.o instructions.o interpreter.o parser.o main.o 

clean:
	rm -rf *.o $(OUTPUT)