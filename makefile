SHELL = /bin/sh
CC = g++
RM = rm -f

CPPFLAGS = -I. -std=c++14
CFLAGS = 
LDFLAGS = -Wall -O3

PROGNAME = apb
PACKAGE=$(PROGNAME)
VERSION = 1.0
HEADERS = 
SOURCES = apb.cpp
OBJ = $(SOURCES:.cpp=.o)


all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)

%.o: %.c
	$(CC) -g $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(PROGNAME) $(OBJ)