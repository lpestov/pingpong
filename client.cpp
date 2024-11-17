#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    std::string ping = "ping";

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразование адреса в двоичный формат
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sock);
        return EXIT_FAILURE;
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return EXIT_FAILURE;
    }

    // Основной цикл отправки и получения сообщений
    while (true) {
        ssize_t bytes_sent = send(sock, ping.c_str(), ping.size(), 0);
        if (bytes_sent < 0) {
            perror("Send failed");
            break;
        }
        std::cout << "Отправлено: " << ping << std::endl;

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(sock, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            // Создание строки с учётом количества прочитанных байт
            std::string message(buffer, bytes_read);
            std::cout << "Получено: " << message << std::endl;
        } else if (bytes_read == 0) {
            std::cout << "Сервер закрыл соединение" << std::endl;
            break;
        } else {
            perror("Read failed");
            break;
        }

        sleep(1); // Задержка перед следующим сообщением
    }

    close(sock);
    return 0;
}
