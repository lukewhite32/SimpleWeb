#define HTTP_GET  1
#define HTTP_POST 2

struct PostData {
    std::string directory;
    std::string content;
    std::string mime;
};

struct HTTPRequest {
    bool isSet = false;
    int fd;
    int method = HTTP_POST;

    std::string data;
    std::string headers;
    std::string content;
    std::string directory;
    std::string mime;

    void parse_method() {
        if (content.substr(0, 16).find("GET") != std::string::npos) {
            method = HTTP_GET;
        }
        else if (content.substr(0, 16).find("POST") != std::string::npos) {
            method == HTTP_POST;
        }
        /*for (int x = 0; x < 16; x ++) {
            if (content[x] == ' ') {
                if (buffer == "GET") {
                    this -> type = HTTP_GET;
                    break;
                }
                else if (buffer == "POST") {
                    this -> type == HTTP_POST;
                    break;
                }
            }
            else {
                buffer += content[x];
            }            
        }*/
    }

    void parse_post_mime() {
        std::string buffer;

        size_t pos = headers.find("Content-Type: ");
        if (pos != std::string::npos) {
            pos += 14;
            buffer = "";
            for (int x = pos; x < headers.length(); x ++) {
                buffer += headers[x]; 
            }
            mime = buffer;
        }
    }

    void parse_directory() {
        std::string buffer;
        bool isBuffering = false;

        for (int x = 0; x < content.length(); x ++) {
            if (content[x] == ' ') {
                if (isBuffering) {
                    directory = buffer;
                    return;
                }
                else {
                    isBuffering = true;
                }
            }
            else {
                if (isBuffering) {
                    buffer += content[x];
                }
            }
        }
    }

    void set(std::string buff, int newfd) {
        isSet = true;
        fd = newfd;
        data = buff;
        size_t pos = data.find("\r\n\r\n");
        headers = data.substr(0, pos);
        content = data.substr(pos + 4);

        parse_method();
        parse_directory();
        parse_post_mime();
    }
};