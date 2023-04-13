#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Cấu trúc để lưu thông tin của sinh viên
struct SinhVien {
    char mssv[10];
    char hoTen[50];
    char ngaySinh[20];
    float diemTB;
};

int main(int argc, char *argv[]) {
    // Kiểm tra đầu vào
    if (argc != 3) {
        printf("Usage: %s <server_address> <server_port>\n", argv[0]);
        return 1;
    }

    // Tạo socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return 1;
    }

    // Thiết lập địa chỉ và cổng của server
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // Kết nối đến server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        return 1;
    }

    // Nhập thông tin của sinh viên từ người dùng
    struct SinhVien sv;
    printf("Nhap MSSV: ");
    fgets(sv.mssv, sizeof(sv.mssv), stdin);
    sv.mssv[strcspn(sv.mssv, "\n")] = '\0'; // Xóa ký tự '\n' ở cuối chuỗi

    printf("Nhap ho ten: ");
    fgets(sv.hoTen, sizeof(sv.hoTen), stdin);
    sv.hoTen[strcspn(sv.hoTen, "\n")] = '\0';

    printf("Nhap ngay sinh: ");
    fgets(sv.ngaySinh, sizeof(sv.ngaySinh), stdin);
    sv.ngaySinh[strcspn(sv.ngaySinh, "\n")] = '\0';

    printf("Nhap diem trung binh: ");
    scanf("%f", &sv.diemTB);

    // Đóng gói dữ liệu và gửi sang server
    char buffer[sizeof(sv)];
    memcpy(buffer, &sv, sizeof(sv));
    if (send(sockfd, buffer, sizeof(buffer), 0) < 0) {
        perror("ERROR sending data");
        return 1;
    }

    // Đóng kết nối và thoát chương trình
    close(sockfd);
    return 0;
}