#include <netinet/in.h>  // for sockaddr_in
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>  // for socket APIs
#include <sys/types.h>
#include <unistd.h>      // for close()
#include "list.h"

#define PORT 9001
#define ACK "ACK"

int main(int argc, char const* argv[]) 
{ 
    int n, val, idx;
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    // Allow quick reuse of the port after stopping the server
    int opt = 1;
    setsockopt(servSockD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind socket to IP and port
    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(servSockD, 1) < 0) {
        perror("Listen failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }
    printf("✅ Server started on port %d. Waiting for client...\n", PORT);

    int clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket < 0) {
        perror("Accept failed");
        close(servSockD);
        exit(EXIT_FAILURE);
    }
    printf("🎯 Client connected!\n");

    // Initialize linked list
    List *mylist = list_alloc();
    char buf[1024], sbuf[1024];
    char *token;

    while (1) {
        memset(buf, 0, sizeof(buf));
        n = recv(clientSocket, buf, sizeof(buf) - 1, 0);

        if (n <= 0) {
            printf("⚠️ Client disconnected or error.\n");
            break;
        }

        buf[n] = '\0';
        token = strtok(buf, " \n\r");
        if (token == NULL) continue;

        // === COMMAND HANDLING ===
        if (strcmp(token, "exit") == 0) {
            snprintf(sbuf, sizeof(sbuf), "Server shutting down...");
            send(clientSocket, sbuf, strlen(sbuf), 0);
            break;
        } 
        else if (strcmp(token, "get_length") == 0) {
            val = list_length(mylist);
            snprintf(sbuf, sizeof(sbuf), "Length = %d", val);
        } 
        else if (strcmp(token, "add_front") == 0) {
            token = strtok(NULL, " \n\r");
            if (token == NULL) {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing value.");
            } else {
                val = atoi(token);
                list_add_to_front(mylist, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            }
        } 
        else if (strcmp(token, "add_back") == 0) {
            token = strtok(NULL, " \n\r");
            if (token == NULL) {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing value.");
            } else {
                val = atoi(token);
                list_add_to_back(mylist, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            }
        } 
        else if (strcmp(token, "add_position") == 0) {
            char *idxToken = strtok(NULL, " \n\r");
            char *valToken = strtok(NULL, " \n\r");
            if (!idxToken || !valToken) {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing index or value.");
            } else {
                idx = atoi(idxToken);
                val = atoi(valToken);
                list_add_at_index(mylist, idx, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d at %d", ACK, val, idx);
            }
        } 
        else if (strcmp(token, "remove_back") == 0) {
            val = list_remove_from_back(mylist);
            snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
        } 
        else if (strcmp(token, "remove_front") == 0) {
            val = list_remove_from_front(mylist);
            snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
        } 
        else if (strcmp(token, "remove_position") == 0) {
            token = strtok(NULL, " \n\r");
            if (token == NULL) {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing index.");
            } else {
                idx = atoi(token);
                val = list_remove_at_index(mylist, idx);
                snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
            }
        } 
        else if (strcmp(token, "get") == 0) {
            token = strtok(NULL, " \n\r");
            if (token == NULL) {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing index.");
            } else {
                idx = atoi(token);
                val = list_get_elem_at(mylist, idx);
                snprintf(sbuf, sizeof(sbuf), "Value = %d", val);
            }
        } 
        else if (strcmp(token, "print") == 0) {
            snprintf(sbuf, sizeof(sbuf), "List: %s", listToString(mylist));
        } 
        else {
            snprintf(sbuf, sizeof(sbuf), "Unknown command.");
        }

        // Send the response to client
        send(clientSocket, sbuf, strlen(sbuf), 0);
    }

    printf("🛑 Closing server.\n");
    close(clientSocket);
    close(servSockD);
    list_free(mylist);
    return 0;
}
