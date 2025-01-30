#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080  // Θύρα για τη σύνδεση στον server

int main() {
    // Δημιουργία socket για τον client
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Ρύθμιση διεύθυνσης server
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;           // Οικογένεια διευθύνσεων (IPv4)
    serv_addr.sin_port = htons(PORT);         // Θύρα 8080
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);  // Διεύθυνση IP του server (localhost)

    // Σύνδεση με τον server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Επιλογή τυχαίου προϊόντος (ID από 0 έως 19)
    int product_id = rand() % 20;
    printf("Client is ordering product with ID: %d\n", product_id);

    // Αποστολή του ID του προϊόντος στον server
    write(sock, &product_id, sizeof(int));

    // Λήψη απάντησης από τον server
    char response[50];
    read(sock, response, sizeof(response));
    printf("Server response: %s\n", response);

    // Κλείσιμο της σύνδεσης
    close(sock);
    return 0;
}
