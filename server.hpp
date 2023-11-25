#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>

#include "util.h"
#include "http-request.hpp"


class WebServer {
    long bufferSize = 4096;

    std::string parse_filedirectory(std::string str) {
        if ((str[str.length()-1] == '/')) {
            str += "index.html";
        }
        else if (!isFile(str)) {
            str += "/index.html";
        }
        return str.substr(1);
    }

    std::string get_mime(std::string str) {
        std::string mime_buffer, dir;
        bool is_buffering = false;
        for (int x = 0; x < str.length(); x ++) {
            if (str[x] == '/') {
                dir = "";
            }
            else {
                dir += str[x];
            }
        }
        for (int x = 0; x < dir.length(); x ++) {
            if (dir[x] == '.') {
                is_buffering = true;
            }
            else if (is_buffering) {
                mime_buffer += dir[x];
            }
        }
        return get_mime_from_extention(mime_buffer);
    }

    std::string req_file;

    void handle_request(HTTPRequest req) {
        
        if (req.type == HTTP_GET) {
            req_file = parse_filedirectory(req.directory);
            if (!fileExists(req_file)) {
                send(req.fd, "HTTP/1.1 404 Not Found\r\nServer: webserver-c\r\nContent-type: text/html\r\nContent-Length: 109\r\n\r\n<!DOCTYPE html><html><head><title>404 not found</title></head><body><h1>404 Not Found</h1><p>404 Not Found</p></body></html>\r\n", 219, 0);
            }
            else {
                send(req.fd, "HTTP/1.1 200 OK\r\nServer: webserver-c\r\nContent-type: ", 52, 0);
                std::string mime = get_mime(req_file);
                send(req.fd, mime.c_str(), strlen(mime.c_str()), 0);

                char content_length[32] = {0};
                
                send(req.fd, "\r\nContent-Length: ", 18, 0);
                sprintf(content_length, "%ld", fileLength(req_file));
                send(req.fd, content_length, strlen(content_length), 0);
                FILE* file = fopen(req_file.c_str(), "r");
                send(req.fd, "\r\n\r\n", 4, 0);

                bool over = false;
                int size = bufferSize;
                while (1) {
                    char file_buffer[size] = {0};
                    fread(file_buffer, size, 1, file);
                    if (feof(file)) {
                        size = trueSize(file_buffer, bufferSize);
                        over = true;
                    }
                    send(req.fd, (const char*)file_buffer, size, 0);
                    if (over)
                        break;
                }
                send(req.fd, "\r\n\r\n", 4, 0);
            }
        }
        else if (req.type == HTTP_POST) {
            recentPost.directory = req.directory;
            recentPost.content = req.content;
        }
    }

public:
    struct sockaddr_in address;

    int addrlen;
    int newsockfd;
    char buff[2048] = {0};
    int opt = 1;
    int sockfd;
    int recvSize = 0;

    PostData recentPost;

    WebServer(int port, long bufferSize = 4096) {


        addrlen = sizeof(address);
        newsockfd;
        opt = 1;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        recvSize = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        signal(SIGPIPE, SIG_IGN);

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        bind(sockfd, (struct sockaddr*)&address, sizeof(address));
    }

    PostData postData;

    void serve() {
        while (1) {
            listen(sockfd, 500);
            newsockfd = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
            recvSize = read(newsockfd, buff, 2048);
            buff[recvSize] = 0;
            HTTPRequest req;
            req.set(buff, newsockfd);
            handle_request(req);
            close(newsockfd);
        }
        
    }
};