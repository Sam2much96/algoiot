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
#include "FreeRTOS.h"
// #include <stdio.h>
//  #include <cstdint>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
// #include "pico/async_context.h"
#include "lwip/dns.h"
#include "lwip/inet.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h" //bugs out cuz of redefinitions between freertos sockets and lwip sockets
// #include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/certs_test.h>

#define URL_2 "example.com"
#define CONTENT_TYPE "application/x-www-form-urlencoded\r\n"

// Replace with your Wi-Fi credentials
#ifndef WIFI_SSID
#define WIFI_SSID "your_ssid"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "your_password"
#endif
// replace with actual variants from wifihelper
// not needed, delete and rewrite later
// #define AF_INET
// #define SOCK_STREAM
#define TCP_PORT 1111
const char *request = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
char response[2048];

class HTTPReq
{
public:
    /**
     * This example shows how to make a simple request with curl.
     */
    int test()
    {
        // initialise wolf ssl
        /* Initialize wolfSSL */
        wolfSSL_Init();
        wolfSSL_Debugging_ON();

        // Perform HTTPS request with DNS resolution
        https_request_with_dns("www.google.com");
        return 0;
    }

    static void dns_found(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
    {
        if (ipaddr != NULL)
        {
            printf("Resolved %s to %s\n", name, ipaddr_ntoa(ipaddr));

            int ret;
            WOLFSSL_CTX *ctx;
            WOLFSSL *ssl;
            int sock;
            // SOCKET_T sock;
            struct sockaddr_in serv_addr;

            // Initialize WolfSSL
            wolfSSL_Init();

            // Create and initialize WOLFSSL_CTX
            if ((ctx = wolfSSL_CTX_new(wolfTLSv1_2_client_method())) == NULL)
            {
                printf("wolfSSL_CTX_new error.\n");
                return;
            }

            // Load CA certificates
            if (wolfSSL_CTX_load_verify_buffer(ctx, ca_cert_der_2048, sizeof(ca_cert_der_2048), SSL_FILETYPE_ASN1) != SSL_SUCCESS)
            {
                printf("Error loading CA certificate.\n");
                wolfSSL_CTX_free(ctx);
                wolfSSL_Cleanup();
                return;
            }

            // Create a WOLFSSL object
            if ((ssl = wolfSSL_new(ctx)) == NULL)
            {
                printf("wolfSSL_new error.\n");
                wolfSSL_CTX_free(ctx);
                wolfSSL_Cleanup();
                return;
            }

            // Create a socket
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (!sock)
            {
                printf("ERROR:wolf_TCPsocke()\n");
                printf("Socket creation error.\n");
                wolfSSL_free(ssl);
                wolfSSL_CTX_free(ctx);
                wolfSSL_Cleanup();
                return;
            }

            // Set up the server address struct
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(443);
            serv_addr.sin_addr.s_addr = ipaddr->addr;

            // Connect to the server
            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            {
                printf("Connection error.\n");
                // free connection to the server
                // free(sock);
                wolfSSL_free(ssl);
                wolfSSL_CTX_free(ctx);
                wolfSSL_Cleanup();
                return;
            }

            // create derefed int object for socket
            // int sock_ = reinterpret_cast<int>(sock);

            // Associate the socket with the WOLFSSL object
            wolfSSL_set_fd(ssl, sock);

            // Perform the SSL/TLS handshake
            if (wolfSSL_connect(ssl) != SSL_SUCCESS)
            {
                printf("wolfSSL_connect error.\n");
                // free(sock);
                wolfSSL_free(ssl);
                wolfSSL_CTX_free(ctx);
                wolfSSL_Cleanup();
                return;
            }

            // Send the HTTP request
            if (wolfSSL_write(ssl, request, strlen(request)) != strlen(request))
            {
                printf("wolfSSL_write error.\n");
                // free(sock);
                wolfSSL_free(ssl);
                wolfSSL_CTX_free(ctx);
                wolfSSL_Cleanup();
                return;
            }

            // Receive the response
            ret = wolfSSL_read(ssl, response, sizeof(response) - 1);
            if (ret < 0)
            {
                printf("wolfSSL_read error.\n");
            }
            else
            {
                response[ret] = '\0'; // Null-terminate the response
                printf("Received:\n%s\n", response);
            }

            // Clean up
            // free(sock);
            wolfSSL_free(ssl);
            wolfSSL_CTX_free(ctx);
            wolfSSL_Cleanup();
        }
        else
        {
            printf("DNS resolution failed for %s\n", name);
        }
    }

    void https_request_with_dns(const char *hostname)
    {
        ip_addr_t addr;
        err_t err = dns_gethostbyname(hostname, &addr, dns_found, NULL);

        if (err == ERR_OK)
        {
            // IP address is already in addr
            dns_found(hostname, &addr, NULL);
        }
        else if (err == ERR_INPROGRESS)
        {
            // DNS request sent, dns_found will be called with the result
            printf("DNS resolution in progress for %s\n", hostname);
        }
        else
        {
            printf("DNS resolution error for %s: %d\n", hostname, err);
        }
    }
};