CC=g++
CFLAGS=-g -Wall -pedantic -g

TARGET= UDP_server
all: $(TARGET)

$(TARGET): $(TARGET).cc
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cc -lpthread

clean:
	$(RM) $(TARGET)