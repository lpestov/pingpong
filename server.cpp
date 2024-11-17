#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    char buffer[BUFFER_SIZE];
    std::string pong = "pong";

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Привязка сокета к адресу
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Ожидание подключений
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    std::cout << "Сервер ожидает подключения на порту " << PORT << "..." << std::endl;

    // Принятие подключения
    if ((client_socket = accept(server_fd, (struct sockaddr*)&address, &addr_len)) < 0) {
        perror("Accept failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Основной цикл обработки
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            // Создание строки с учётом количества прочитанных байт
            std::string message(buffer, bytes_read);
            std::cout << "Получено: " << message << std::endl;

            if (message == "ping") {
                ssize_t bytes_sent = send(client_socket, pong.c_str(), pong.size(), 0);
                if (bytes_sent < 0) {
                    perror("Send failed");
                    break;
                }
                std::cout << "Отправлено: " << pong << std::endl;
            } else {
                std::cout << "Неизвестное сообщение: " << message << std::endl;
            }
        } else if (bytes_read == 0) {
            std::cout << "Клиент закрыл соединение" << std::endl;
            break;
        } else {
            perror("Read failed");
            break;
        }
    }

    close(client_socket);
    close(server_fd);
    return 0;
}
