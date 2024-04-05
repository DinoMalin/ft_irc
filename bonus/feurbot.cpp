#include <netdb.h>
#include "header.hpp"

void send_message(int sockfd, const std::string& channel, const std::string& message) {
    std::string full_message = "PRIVMSG " + channel + " :" + message + CRLF;
    send(sockfd, full_message.c_str(), full_message.size(), 0);
}

bool endsWiths(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (fullString.compare (fullString.length() - ending.length(), ending.length(), ending) == 0);
    } else {
        return false;
    }
}

std::string toLower(std::string str) {
    std::string result;
    
    for (size_t i = 0; i < str.size(); i++) {
        result += tolower(str[i]);
    }
    return result;
}

int main(int argc, char** argv) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <server> <port> <channel> <passsword>\n";
        return 1;
    }

    std::string server = argv[1];
    int port = std::atoi(argv[2]);
    std::string channel = argv[3];
    std::string password = argv[4];
    
    if (channel[0] != '#')
        channel = '#' + channel;

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
    std::string pass_message = "PASS " + password + CRLF;
    std::string user_message = "USER feurbot Feurbot localhost :FeurBot" + std::string(CRLF);
    std::string join_message = "JOIN " + channel + CRLF;
    std::string nick_message = "NICK feurbot" + std::string(CRLF);
    send(sockfd, pass_message.c_str(), pass_message.size(), 0);
    send(sockfd, nick_message.c_str(), nick_message.size(), 0);
    send(sockfd, user_message.c_str(), user_message.size(), 0);
    send(sockfd, join_message.c_str(), join_message.size(), 0);

    char buffer[4096];
    ssize_t bytes_received;

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        std::string message(buffer, bytes_received);
        std::cout << message;

        if (message.find("PRIVMSG " + channel) != std::string::npos) {
            std::string lower = toLower(message);
            if (endsWiths(lower, "quoi\r\n"))
                send_message(sockfd, channel, "feur");
            if (endsWiths(lower, "kwa\r\n"))
                send_message(sockfd, channel, "feur");
            if (endsWiths(lower, "qua\r\n"))
                send_message(sockfd, channel, "feur");
            if (endsWiths(lower, "quoua\r\n"))
                send_message(sockfd, channel, "feur");
            if (endsWiths(lower, "quouoi\r\n"))
                send_message(sockfd, channel, "feur");
            if (endsWiths(lower, "quwoa\r\n"))
                send_message(sockfd, channel, "feur");
        }
    }

    close(sockfd);
    return 0;
}