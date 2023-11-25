#include "mimes.h"

bool isFile(std::string s) {
    return s.find('.') != std::string::npos;
}

inline bool fileExists(const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

inline long fileLength(const std::string& name) {
    struct stat st;
    stat(name.c_str(), &st);
    return st.st_size;
}

int trueSize(char str[], int arraySize) {
    int ret = 0;
    for (int x = 0; x < arraySize; x ++) {
        if (str[x] != '\0') {
            ret = x+1;
        }
    }
    return ret;
}

const char* get_mime_from_extention(std::string ex) {
    for (int x = 0; x < 76; x ++) {
        if (ex == mimeExtentions[x]) {
        
            return mimeTypes[x];
        }
    }
    return "text/plain";
}