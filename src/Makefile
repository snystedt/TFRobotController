ODIR=../obj/
LDIR =../libs/
IDIR =../include/
BIN = ../bin/
CC=gcc
CFLAGS=-I$(IDIR) -std=gnu99 -g `pkg-config --cflags --libs gtk+-3.0` -O0 -DXBOX_FIX -DDEBUG

LIBS=-lm -pthread

_DEPS = serial_lib.h myStringLib.h robotJoystick.h \
matrixReader.h arduinoController.h robotGui.h \
controllerScripts.h servo.h controller_thread.h serial_thread.h threads.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = serial_lib.o robotController.o myStringLib.o \
robotJoystick.o matrixReader.o arduinoController.o \
robotGui.o controllerScripts.o servo.o controller_thread.o serial_thread.o threads.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

PROG=robotController
LIST=$(addprefix $(BIN), $(PROG))

all: $(LIST)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIST): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
