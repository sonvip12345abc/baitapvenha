#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define BUF_SIZE 1024

typedef struct {
    char mssv[10];
    char hoten[50];
    char ngaysinh[20];
    float diemtb;
} SinhVien;

int main(int argc, char *argv[]) {
    time_t now=time(0);
    char* dt=ctime(&now);
    int sockfd, client_socket, portno, clilent;
    char buffer[BUF_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    socklen_t client_address_len;
    // Kiểm tra đầu vào
    if (argc < 2) {
        printf( "Usage: %s <port>\n", argv[0]);
        return 1;
    }

     //Tạo socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
    }

     //Thiết lập thông tin địa chỉ server
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    // Liên kết socket với địa chỉ server
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
    }

    //Lắng nghe kết nối từ client
    listen(sockfd, 5);
    client_address_len = sizeof(cli_addr);

    // Chấp nhận kết nối từ client
    client_socket = accept(sockfd, (struct sockaddr *) &cli_addr, &client_address_len);
    if (client_socket < 0) {
        perror("ERROR on accept");
        return 1;
    }

    // lấy thông tin từ client
    char *client_ip = inet_ntoa(cli_addr.sin_addr);
    int client_port = ntohs(cli_addr.sin_port);
    printf("Connection accepted from %s:%d\n", client_ip, client_port);

    // Nhận thông tin từ client
    SinhVien sv;
    bzero(buffer, BUF_SIZE);
    n = read(client_socket, buffer, BUF_SIZE - 1);
    if (n < 0) {
        perror("ERROR reading from socket");
    }

    memcpy(&sv, buffer, sizeof(SinhVien));
    printf( "%s,%s,%s,%s,%s,%.2f\n",inet_ntoa(cli_addr.sin_addr), dt,sv.mssv,sv.hoten,sv.ngaysinh,sv.diemtb);

    //ghi file
    FILE *fp = fopen("sv_log.txt", "a");
    if (fp == NULL) {
        perror("ERROR opening file");
    }

    fprintf(fp, "%s %s %s %s %s %.2f\n" ,inet_ntoa(cli_addr.sin_addr), dt,sv.mssv,sv.hoten,sv.ngaysinh,sv.diemtb);
    fclose(fp);

    close(client_socket);
    close(sockfd);
}


