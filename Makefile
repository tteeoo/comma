CC	= gcc
CFLAGS	= -Wall -I.
OBJFILES	= src/main.o src/errorfuncs.o src/parsefuncs.o
TARGET	= comma

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET)

install:
	cp comma /usr/local/bin/

.phony: all clean install $(TARGET)
