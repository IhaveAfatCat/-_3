#ifndef ESHOP_H
#define ESHOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080          // Θύρα για τη σύνδεση
#define MAX_PRODUCTS 20    // Μέγιστος αριθμός προϊόντων
#define ORDERS_PER_CLIENT 10  // Αριθμός παραγγελιών ανά πελάτη

// Δομή για τα προϊόντα
typedef struct {
    char name[50];  // Όνομα προϊόντος
    int stock;      // Απόθεμα
    float price;    // Τιμή
} Product;

// Κατάλογος προϊόντων
extern Product inventory[MAX_PRODUCTS];

// Αρχικοποίηση του καταλόγου προϊόντων
void init_inventory();

// Συνάρτηση για την επεξεργασία μιας παραγγελίας (server)
void process_order(int client_sock);

// Συνάρτηση για τον server
void run_server();

// Συνάρτηση για τον client
void run_client();

#endif // ESHOP_H
