CC = gcc
CFLAGS = -Wall -Wextra -pthread

SERVER = server
CLIENT = client

SRV_SRC = server.c
CLI_SRC = client.c

all: $(SERVER) $(CLIENT)

$(SERVER): $(SRV_SRC)
	$(CC) $(CFLAGS) -o $(SERVER) $(SRV_SRC)

$(CLIENT): $(CLI_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLI_SRC)

run-server:
	./$(SERVER)

run-clients:
	./$(CLIENT) & ./$(CLIENT) & ./$(CLIENT) & ./$(CLIENT) & ./$(CLIENT) &

clean:
	rm -f $(SERVER) $(CLIENT)
