SDIR=./src/
ODIR=./obj/
LDIR=./libs/
IDIR=./include/
BIN=./bin/
PROG=robotController
CC=gcc
CFLAGS=-I$(IDIR) -std=gnu99 -g `pkg-config --cflags --libs gtk+-3.0` -O0 -DXBOX_FIX -DDEBUG

LIBS=-lm -pthread

FILES = serial_lib myStringLib robotJoystick arduinoController \
	robotGui controller_thread serial_thread threads

_DEPS = $(patsubst %,%.h,$(FILES))
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(patsubst %,%.o,$(FILES))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
OBJ += $(ODIR)/$(PROG).o

_SRC = $(patsubst %,%.c,$(FILES))
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))
SRC += $(SDIR)/$(PROG).c

LIST=$(addprefix $(BIN), $(PROG))
MKDIR_P= mkdir -p

all: directories $(LIST) 

directories: $(ODIR)

$(ODIR):
	$(MKDIR_P) $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIST): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean directories

clean:
	rm -f $(ODIR)/*.o
