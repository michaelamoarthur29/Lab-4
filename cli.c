#include <netinet/in.h>   // for sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>   // for socket APIs
#include <sys/types.h>
#include <unistd.h>       // for close()

#define PORT 9001
#define MAX_COMMAND_LINE_LEN 1024

// Function to read a command line from stdin safely
char* getCommandLine(char *command_line) {
    do {
        if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
            fprintf(stderr, "fgets error\n");
            exit(EXIT_FAILURE);
        }
    } while (command_line[0] == 0x0A); // ignore if just ENTER pressed

    // Remove newline character
    command_line[strcspn(command_line, "\n")] = '\0';
    return command_line;
}

int main(int argc, char const* argv[]) {
    int sockID = socket(AF_INET, SOCK_STREAM, 0);
    if (sockID < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    // Try to connect to the server
    int connectStatus = connect(sockID, (struct sockaddr*)&servAddr, sizeof(servAddr));
    if (connectStatus == -1) {
        perror("Connection to server failed");
        close(sockID);
        exit(EXIT_FAILURE);
    }

    char buf[MAX_COMMAND_LINE_LEN];
    char responseData[MAX_COMMAND_LINE_LEN];
    char *token, *cp;

    printf("Connected to server on port %d.\n", PORT);
    printf("Type 'menu' to see available commands.\n");

    while (1) {
        printf("\nEnter Command (or 'menu'): ");
        getCommandLine(buf);

        // Send the command to the server
        send(sockID, buf, strlen(buf), 0);

        cp = buf;
        token = strtok(cp, " ");

        // Handle local commands (menu or exit)
        if (strcmp(token, "exit") == 0) {
            printf("Closing connection...\n");
            close(sockID);
            exit(EXIT_SUCCESS);
        } else if (strcmp(token, "menu") == 0) {
            printf("\nCOMMANDS:\n---------\n");
            printf("1. print\n");
            printf("2. get_length\n");
            printf("3. add_back <value>\n");
            printf("4. add_front <value>\n");
            printf("5. add_position <index> <value>\n");
            printf("6. remove_back\n");
            printf("7. remove_front\n");
            printf("8. remove_position <index>\n");
            printf("9. get <index>\n");
            printf("10. exit\n");
            continue;
        }

        // Receive the response from server
        memset(responseData, 0, sizeof(responseData));
        int bytesReceived = recv(sockID, responseData, sizeof(responseData) - 1, 0);
        if (bytesReceived > 0) {
            responseData[bytesReceived] = '\0';
            printf("\nSERVER RESPONSE: %s\n", responseData);
        } else {
            printf("No response or connection closed by server.\n");
            break;
        }

        memset(buf, 0, sizeof(buf));
    }

    close(sockID);
    return 0;
}
