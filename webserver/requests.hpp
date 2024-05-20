// Custom Requests Class for Pico W using LWIP, Http Client/server
// Documentations, FAQs, Helpers, Templates:
//
// (1) https://www.mail-archive.com/lwip-users@nongnu.org/msg19790.html
// (2) https://stackoverflow.com/questions/76182903/rasberry-pi-pico-w-http-get-request-failing-when-strcpy-line-uncommented
// (3) https://www.i-programmer.info/programming/hardware/15838-the-picow-in-c-simple-web-client.html
// (4) FTP : https://github.com/gezedo/lwftp
// (5) SNTP : https://github.com/jondurrant/RPIPicoWSNTP
// (6) WolfSSL
// (7) CurlCPP : https://github.com/JosephP91/curlcpp
#include <string>
#include <stdio.h>
// #include <iostream>
// #include <cstring>
// #include "pico/stdlib.h"
// #include "pico/cyw43_arch.h"
// #include "lwip/apps/http_client.h"

// curl
// #include "curlcpp/curl_easy.h"

// Request Class for creatuing generic request things

#define URL_2 "example.com"
#define CONTENT_TYPE "application/x-www-form-urlencoded\r\n"

// Documentation : https://github.com/JosephP91/curlcpp
class HTTPReq
{
public:
    /**
     * This example shows how to make a simple request with curl.
     */
    int test()
    {

        return 0;
    }
};