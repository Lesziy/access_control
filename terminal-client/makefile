CC = g++
CFLAGS = -c -Wall --std=c++11
SRCDIR = src
INCLUDE = -Iinclude

all: project

project: main.o client.o auth-proto.o conn.o
	$(CC) main.o client.o auth-proto.o conn.o -o build/app.out

main.o: $(SRCDIR)/main.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/main.cpp $(INCLUDE) -o main.o

client.o: $(SRCDIR)/client-application.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/client-application.cpp $(INCLUDE) -o client.o

auth-proto.o: $(SRCDIR)/auth-protocol.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/auth-protocol.cpp $(INCLUDE) -o auth-proto.o

conn.o: $(SRCDIR)/connection.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/connection.cpp -o conn.o

clean:
	rm *o build/app.out