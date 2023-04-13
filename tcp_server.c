#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    int server_socket, client_socket, port_number, n;
    char buffer[BUFSIZE];
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len;

    // Kiểm tra số lượng tham số dòng lệnh
    if (argc != 4) {
        printf("Usage: %s <port> <hello file> <client file>\n", argv[0]);
        return 1;
    }

    // Lấy thông tin cổng, tên tệp tin chứa câu chào và tên tệp tin để lưu nội dung client gửi đến
    port_number = atoi(argv[1]);
    

    //Tạo socket
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == -1)
    {
        perror("socket() failed");
        return 1;
    }
    //Thiết lập thông tin địa chỉ server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);
    // Liên kết socket với địa chỉ server
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)))
    {
        perror("bind() failed");
        return 1;
    }
    //Lắng nghe kết nối từ client
    if (listen(server_socket, 5))
    {
        perror("listen() failed");
        return 1;
    }
    
    // Chấp nhận kết nối từ client
    client_address_len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket < 0) {
         perror("accept() failed");
        return 1;
    }
    //Gửi xâu chào đến client
    FILE *hello_file = fopen(argv[2], "r");
    if (hello_file == NULL)
        perror("ERROR opening hello message file");

    bzero(buffer, BUFSIZE);
    int len = fread(buffer, 1, BUFSIZE, hello_file);
    n = write(client_socket, buffer, len);
    if (n < 0)
        perror("ERROR writing to socket");
    fclose(hello_file);
    //Nhận dữ liệu từ client và ghi ra vào file
    FILE *client_file = fopen(argv[3], "w");
    if (client_file == NULL)
        perror("ERROR opening output file");
    
    while (1)
    {
        int n = read(client_socket, buffer, BUFSIZE);
        if (n <= 0)
        {
            printf("Ket noi bi dong.\n");
            break;    
        }
        if (n < BUFSIZE)
            buffer[n] = 0;
            printf("data da gui toi file client\n");
        fwrite(buffer, 1, n,client_file);
    }

    
    fclose(client_file);

    //Close sockets 
    close(client_socket);
    close(server_socket);

    return 0;
}


       

