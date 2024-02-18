# SimpleWeb
A very simple, lightweight C++ webserver. Meant for simple use, it supports POST as well as a ton of common MIME types.
# How To Use
## On Linux
Run this command in the same folder as your project:
```git clone https://github.com/lukewhite32/SimpleWeb.git```
To use, just put this header in your project.
```#include "SimpleWeb/server.hpp"```
## On any other system
Download the .zip file, extract it, and copy the extracted folder into your project. Use this header to import it into your project.
```#include "SimpleWeb/server.hpp"```
# Example
```cpp
#include "SimpleWeb/server.hpp"

int main() {
    WebServer server(8080);    // create a new webserver object to run at port 8080

    while (1) {
        server.serve("<some-directory>/");    // run the webserver at a certain directory
    }

    return 0;
}
```
## With POST
```cpp
#include "SimpleWeb/server.hpp"
#include <iostream>

int main() {
    WebServer server(8080);    // create a new webserver object to run at port 8080

    while (1) {
        server.serve("<some-directory>/");    // run the webserver at a certain directory

        // POST data is stored in the recentPost variable
        std::cout << "POST Content: " << server.recentPost.content << std::endl;
        std::cout << "POST Directory: " << server.recentPost.directory << std::endl;
        std::cout << "POST MIME: " << server.recentPost.mime << std::endl;
    }

    return 0;
}
```