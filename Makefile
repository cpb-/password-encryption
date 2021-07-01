
EXE = hasher

OBJ = hasher.o

CROSS_COMPILE ?=
CC            ?= $(CROSS_COMPILE)gcc
CFLAGS        += -Wall -W -D_HASHER_VERSION=$(shell git describe --always 2>/dev/null | tr -d '\r\n')
LDFLAGS       += -lcrypt

.PHONY: all

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c  $<

.PHONY: clean

clean:
	rm -f $(EXE) $(OBJ) *~

