#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

int main() {
    int server_fd, new_socket;
    sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* response = "pong\n";

    // Создание сокета
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        return 1;
    }

    // Опции сокета
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // слушаем на всех интерфейсах, включая Tailscale
    address.sin_port = htons(12345);

    // Привязка
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    // Ожидание подключения
    listen(server_fd, 3);
    std::cout << "Сервер слушает порт 12345..." << std::endl;

    // Принятие подключения
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("accept");
        return 1;
    }

    read(new_socket, buffer, 1024);
    std::cout << "Получено: " << buffer << std::endl;
    send(new_socket, response, strlen(response), 0);
    std::cout << "Ответ отправлен\n";
    close(new_socket);
    return 0;
}
