#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

#define BUFFER_SIZE (1024 * 500)

int main(int argc, char const* argv[]) {
    int                ret = 0;
    int                client_socket;
    struct sockaddr_in server_address;
    char               server_ip[] = "10.0.0.200"; // 目标服务器的IP地址
    int                server_port = 80;           // 目标服务器的端口号

    // const char request_home[128] = "GET /home HTTP/1.1\r\n"
    //                                "Host: example.com\r\n"
    //                                "Connection: close\r\n"
    //                                "\r\n";

    const char request_cover_on[128] = "GET /set_params?cover=1 HTTP/1.1\r\n"
                                       "Host: example.com\r\n"
                                       "Connection: close\r\n"
                                       "\r\n";

    const char request_cover_off[128] = "GET /set_params?cover=0 HTTP/1.1\r\n"
                                        "Host: example.com\r\n"
                                        "Connection: close\r\n"
                                        "\r\n";

    const char request_get_snap[128] = "GET /1.jpg HTTP/1.1\r\n"
                                       "Host: example.com\r\n"
                                       "Connection: close\r\n"
                                       "\r\n";

    int cnt = 0;

    while (1) {
        // 创建套接字
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            perror("Failed to create socket");
            // exit(EXIT_FAILURE);
            sleep(1);
            continue;
        }

        // 设置服务器地址信息
        server_address.sin_family = AF_INET;
        server_address.sin_port   = htons(server_port);
        if (inet_pton(AF_INET, server_ip, &(server_address.sin_addr)) <= 0) {
            perror("Invalid address/ Address not supported");
            // exit(EXIT_FAILURE);
            close(client_socket);
            sleep(1);
            continue;
        }

        // 连接到服务器
        if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
            perror("Connection failed");
            // exit(EXIT_FAILURE);
            close(client_socket);
            sleep(1);
            continue;
        }

        // 构建HTTP请求
        char request[128];

        // sprintf(request,
        //         "GET %s HTTP/1.1\r\n"
        //         "Host: example.com\r\n"
        //         "Connection: close\r\n"
        //         "\r\n",
        //         argv[1]);

        if (cnt % 3 == 0) {
            sprintf(request, request_get_snap);
        } else if (cnt % 3 == 1) {
            sprintf(request, request_cover_off);
        } else if (cnt % 3 == 2) {
            sprintf(request, request_cover_on);
        }

        // sprintf(request, request_home);

        // 发送请求
        if (send(client_socket, request, strlen(request), 0) < 0) {
            perror("Failed to send request");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        cnt++;
        printf("count: %d\n", cnt);

        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(client_socket, &readset);

        struct timeval timeout;
        timeout.tv_sec  = 1;
        timeout.tv_usec = 0;

        ret = select(client_socket + 1, &readset, NULL, NULL, &timeout);
        if (ret == -1) {
            //除了被信号中断的情形，其他情况都是出错
            if (errno != EINTR) {
                // close(client_socket);
                // break;
            }
        } else if (ret == 0) {
            // select函数超时
            printf("select timeout\n");
            // close(client_socket);
            // break;
        } else {
            if (FD_ISSET(client_socket, &readset)) {
                // 接收响应并输出
                char response_buffer[BUFFER_SIZE];
                int  received_bytes;
                while ((received_bytes = recv(client_socket, response_buffer, BUFFER_SIZE - 1, 0)) > 0) {
                    response_buffer[received_bytes] = '\0';
                    printf("%s\n", response_buffer);
                    printf("====================================\n\n");
                }

                if (received_bytes < 0) {
                    //除了被信号中断的情形，其他情况都是出错
                    if (errno != EINTR) {
                        perror("recv");
                        // close(client_socket);
                        // break;
                    }
                } else if (received_bytes == 0) {
                    // printf("对端关闭了连接\n");
                }
            }
        }

        // 关闭套接字
        close(client_socket);

        // sleep(1);
        usleep(1000 * 500);
    }

    return 0;
}
