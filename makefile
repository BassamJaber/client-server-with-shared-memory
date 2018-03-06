CC=gcc

all: server client

server: server.c
	$(CC) server.c -o server -lpthread
client: client.c
	$(CC) client.c -o client
	
