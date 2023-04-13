#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    //Kiểm tra đầy đủ tham số dòng lệnh
    if (argc != 3) {
        printf("Usage: %s <ip_address> <port>\n", argv[0]);
        return 1;
    }

    char *ip_address = argv[1];
    int port = atoi(argv[2]);

    // Tạo socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Tạo địa chỉ kết nối của server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    // Kết nối đến server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    char buf[BUF_SIZE];
     // Nhận dữ liệu từ server
    int num_bytes = recv(sockfd, buf, BUF_SIZE - 1, 0);
    if (num_bytes == -1) {
        perror("recv");
        close(sockfd);
        return 1;
     }

     // Hiển thị dữ liệu nhận được từ server
    buf[num_bytes] = '\0';
     printf("Received from server: %s\n", buf);
    while (1) {
        // Nhập dữ liệu từ bàn phím
        printf("Enter data to send : ");
        fgets(buf, BUF_SIZE, stdin);

        // Gửi dữ liệu đến server
        if (send(sockfd, buf, strlen(buf), 0) == -1) {
            perror("send");
            close(sockfd);
            return 1;
        }
    }

    // Đóng kết nối
    close(sockfd);
    return 0;
}