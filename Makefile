CC	= gcc
CFLAGS	= -Wall -I.
LDFLAGS	=
OBJFILES	= src/main.o src/errorfuncs.o src/parsefuncs.o
TARGET	= comma

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
