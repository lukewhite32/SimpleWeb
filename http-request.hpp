#define HTTP_GET  1
#define HTTP_POST 2

struct PostData {
    std::string directory;
    std::string content;
};

struct HTTPRequest {
    bool isSet = false;
    int fd;
    int type = HTTP_POST;

    std::string content;
    std::string directory;

    void parse_type() {
        std::string buffer;
        for (int x = 0; x < 16; x ++) {
            if (content[x] == ' ') {
                std::cout << "'" << buffer << "'" << std::endl;
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
        content = buff;
        parse_type();
        parse_directory();
    }
};