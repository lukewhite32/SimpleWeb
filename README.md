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
```
#include "SimpleWeb/server.hpp"

int main() {
    WebServer server(8080);    // create a new webserver object to run at port 8080
    server.serve("<some-directory>");    // run the webserver at a certain directory
    
    return 0;
}
```