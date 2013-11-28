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
LINKER = -c
OUTPUT = ifj

OBJS = str.o lexer.o instructions.o ial.o tables.o stack.o interpreter.o parser.o main.o

all: $(OUTPUT)


$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(OBJS) 

clean:
	rm -rf *.o $(OUTPUT)