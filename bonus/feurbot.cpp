#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>

void send_message(int sockfd, const std::string& channel, const std::string& message) {
    std::string full_message = "PRIVMSG " + channel + " :" + message + "\r\n";
    send(sockfd, full_message.c_str(), full_message.size(), 0);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server> <port> <channel>\n";
        return 1;
    }

    const std::string server = argv[1];
    const int port = std::atoi(argv[2]);
    const std::string channel = argv[3];

    struct sockaddr_in serv_addr;
    struct hostent *server_info;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket\n";
        return 1;
    }

    server_info = gethostbyname(server.c_str());
    if (server_info == NULL) {
        std::cerr << "Error: No such host\n";
        return 1;
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server_info->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server_info->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error connecting\n";
        return 1;
    }

    std::string join_message = "JOIN " + channel + "\r\n";
    std::string user_message = "USER FeurBot 0 * :FeurBot\r\n";
    send(sockfd, "NICK FeurBot\r\n", 13, 0);
    send(sockfd, user_message.c_str(), user_message.size(), 0);
    send(sockfd, join_message.c_str(), join_message.size(), 0);

    char buffer[4096];
    ssize_t bytes_received;

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        std::string message(buffer, bytes_received);
        std::cout << message;

        if (message.find("PRIVMSG " + channel) != std::string::npos &&
            message.find_last_of("\n\r") == message.size() - 2 && message.find("quoi\r\n") != std::string::npos) {
            send_message(sockfd, channel, "feur");
        }
    }

    close(sockfd);
    return 0;
}