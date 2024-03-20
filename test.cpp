#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <poll.h>

#define MAX_CLIENTS 10
#define PORT 6667

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    struct pollfd fds[MAX_CLIENTS + 1];
    char buffer[1024];

    // Création du socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Attachement du socket à l'adresse
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur lors de l'attachement du socket à l'adresse");
        exit(EXIT_FAILURE);
    }

    // Mise en écoute du socket
    if (listen(server_socket, 5) == -1) {
        perror("Erreur lors de la mise en écoute du socket");
        exit(EXIT_FAILURE);
    }

    // Initialisation de la structure de descripteurs de fichiers pour poll()
    memset(fds, 0, sizeof(fds));
    fds[0].fd = server_socket;
    fds[0].events = POLLIN;

    std::cout << "Serveur IRC démarré sur le port " << PORT << std::endl;

    while (true) {
        int poll_result = poll(fds, MAX_CLIENTS + 1, -1);
        if (poll_result == -1) {
            perror("Erreur lors de l'appel à poll()");
            exit(EXIT_FAILURE);
        }

        // Vérification des événements sur les sockets
        for (int i = 0; i <= MAX_CLIENTS; ++i) {
            if (fds[i].revents & POLLIN) {
                // Nouvelle connexion sur le socket serveur
                if (i == 0) {
                    client_len = sizeof(client_addr);
                    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
                    if (client_socket == -1) {
                        perror("Erreur lors de l'acceptation de la connexion");
                        continue;
                    }

                    std::cout << "Nouvelle connexion entrante" << std::endl;

                    // Ajout du socket client aux descripteurs surveillés
                    for (int j = 1; j <= MAX_CLIENTS; ++j) {
                        if (fds[j].fd == 0) {
                            fds[j].fd = client_socket;
                            fds[j].events = POLLIN;
                            break;
                        }
                    }
                } else {
                    // Réception des données du client
                    ssize_t bytes_received = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    if (bytes_received <= 0) {
                        // Erreur ou fin de connexion
                        close(fds[i].fd);
                        fds[i].fd = 0;
                        std::cout << "Client déconnecté" << std::endl;
                    } else {
                        // Traitement des données reçues (ici on répond PONG aux commandes PING)
                        if (strncmp(buffer, "PING", 4) == 0) {
                            // Répondre avec PONG
                            send(fds[i].fd, "PONG\r\n", 6, 0);
                        }
                    }
                }
            }
        }
    }

    // Fermeture du socket serveur
    close(server_socket);

    return 0;
}