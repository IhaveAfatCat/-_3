#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080          // Θύρα για τη σύνδεση
#define MAX_PRODUCTS 20    // Μέγιστος αριθμός προϊόντων

// Δομή για τα προϊόντα
typedef struct {
    char name[50];  // Όνομα προϊόντος
    int stock;      // Απόθεμα
    float price;    // Τιμή
} Product;

Product inventory[MAX_PRODUCTS];  // Κατάλογος προϊόντων

// Αρχικοποίηση του καταλόγου προϊόντων
void init_inventory() {
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        sprintf(inventory[i].name, "Product %d", i + 1);  // Ονομασία προϊόντος
        inventory[i].stock = 2;                          // Αρχικό απόθεμα: 2 τεμάχια
        inventory[i].price = (i + 1) * 5.0;              // Τιμή: 5 * (i + 1)
    }
}

// Επεξεργασία παραγγελίας
void process_order(int client_sock) {
    int product_id;
    read(client_sock, &product_id, sizeof(int));  // Διαβάζουμε το ID του προϊόντος από τον πελάτη

    if (product_id < 0 || product_id >= MAX_PRODUCTS) {
        char response[] = "Invalid product ID.";  // Μήνυμα για μη έγκυρο ID
        write(client_sock, response, sizeof(response));
    } else if (inventory[product_id].stock > 0) {
        inventory[product_id].stock--;            // Μείωση αποθέματος
        char response[] = "Order successful.";    // Μήνυμα επιτυχίας
        write(client_sock, response, sizeof(response));
    } else {
        char response[] = "Out of stock.";        // Μήνυμα για εξαντλημένο απόθεμα
        write(client_sock, response, sizeof(response));
    }

    close(client_sock);  // Κλείσιμο σύνδεσης με τον πελάτη
}

int main() {
    init_inventory();  // Αρχικοποίηση του καταλόγου προϊόντων

    int server_fd, client_sock;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);

    // Δημιουργία socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Ρύθμιση διεύθυνσης server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Αποδοχή συνδέσεων από οποιαδήποτε διεύθυνση
    address.sin_port = htons(PORT);        // Θύρα 8080

    // Bind το socket στη διεύθυνση
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Ακρόαση για συνδέσεις (μέχρι 5 σε αναμονή)
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Αποδοχή νέας σύνδεσης
        client_sock = accept(server_fd, (struct sockaddr*)&address, &addr_len);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        // Δημιουργία νέας διεργασίας για κάθε πελάτη
        if (fork() == 0) {
            close(server_fd);  // Κλείσιμο server socket στη child διεργασία
            process_order(client_sock);  // Επεξεργασία παραγγελίας
            exit(0);  // Τερματισμός της child διεργασίας
        } else {
            close(client_sock);  // Κλείσιμο client socket στη parent διεργασία
        }
    }

    return 0;
}
