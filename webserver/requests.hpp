// Custom Requests Class for Pico W using LWIP, Http Client/server
// Documentations, FAQs, Helpers, Templates:
//
// (1) https://www.mail-archive.com/lwip-users@nongnu.org/msg19790.html
// (2) https://stackoverflow.com/questions/76182903/rasberry-pi-pico-w-http-get-request-failing-when-strcpy-line-uncommented
// (3) https://www.i-programmer.info/programming/hardware/15838-the-picow-in-c-simple-web-client.html
// (4) FTP : https://github.com/gezedo/lwftp
// (5) SNTP : https://github.com/jondurrant/RPIPicoWSNTP
// (6) WolfSSL
#include <string>
#include <stdio.h>
// #include <iostream>
#include <cstring>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/http_client.h"

// wifi variables

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Request Class for creatuing generic request things

#define URL_2 "example.com"
#define CONTENT_TYPE "application/x-www-form-urlencoded\r\n"

// class HTTPPost
//{
// public:
//  modify header for custom post request with a functon
//  Uses string concatenation to create customized or default http post requests
//  with altered header types
// static const char *http_post_request "";//= "POST /post HTTP/1.1\r\n"
//                                       "Host: %d\r\n",
//                  "Content-Type: application/x-www-form-urlencoded\r\n"
//                  "Content-Length: 13\r\n"
//                  "Connection: close\r\n\r\n"
//                  "data=value",
//                  URL_2;

//-------------------------------------------------------------------------------------------------------------
//      Class 1
//  ClsHTTPGet
// Allows the user to make a HTTP Get Request by calling the public method HTTP_Request
//-------------------------------------------------------------------------------------------------------------

class HTTPGet
{

    //-------------------------------------------------------------------------------------------------------------
    //      Public Function 1
    //  ClsHTTPGet
    // Constructor method - used to declare and intiliase private attributes
    /**
     * @note - takes no parameters and returns no value (currently)
     * @note - Get Header is already defined in http_client.c
     */
    //-------------------------------------------------------------------------------------------------------------

public:
    int make_http_get_request() // rename to make_http_get_request()
    {
        bodyReceived = false;
        std::strcpy(URL, "");
        myHTTPPORT = 0;
        // strcpy(myTCPIPSERVER , '\0');
    };

    //-------------------------------------------------------------------------------------------------------------
    //      Public Function 2
    //  URLSetter
    // used to set the URL for the specific object
    /**
     * @param newURL - takes the new URL as a char* not string
     * @note max size for URL 48 (maybe need to change)
     */
    //-------------------------------------------------------------------------------------------------------------

    void set_url(char *newURL)
    {
        std::strcpy(URL, newURL);
    }

    //-------------------------------------------------------------------------------------------------------------
    //      Public Function 3
    //  MyTCPIPSEVERSetter
    // used to Set the myTCPIPSEVER attribute for the specific object
    /**
     * @param newTCPIPSERVER - std::string containing new DNS name
     * @note - no return value (void function)
     */
    //-------------------------------------------------------------------------------------------------------------

    void MyTCPIPSERVERSetter(std::string newTCPIPSERVER)
    {
        myTCPIPSERVER = newTCPIPSERVER;
    }

    //-------------------------------------------------------------------------------------------------------------
    //      Public Function 4
    //  MyTCPIPSEVERSetter
    // used to Set the myTCPIPSEVER attribute for the specific object
    /**
     * @param newTCPIPSERVER - std::string containing new DNS name
     * @note - no return value (void function)
     */
    //-------------------------------------------------------------------------------------------------------------

    void MyHTTPPORTSetter(int newHTTPPORT)
    {
        myHTTPPORT = newHTTPPORT;
    }

    //-------------------------------------------------------------------------------------------------------------
    //      Public Function 5
    //  HTTPBufferGetter
    // used to get the HTTPBuffer for the specific object
    /**
     * @return - HTTPBuffer as a char* not string
     * @note max size for HTTPBUffer is 2000
     * @note buffer is null-terminated
     */
    //-------------------------------------------------------------------------------------------------------------

    char *HTTPBufferGetter()
    {
        return HTTPBuffer;
    }

    //-------------------------------------------------------------------------------------------------------------
    //      Public Function 6
    //  HTTPRequest
    // used to perform a HTTP Get request to the specific URL that is an attribute of the object
    /**
     * @return - 0 if no error occured or 1 if a error occured
     * @note - no parameters taken values can be changed through setter methods
     * @note - max size for recieved data is 2000
     * @note - can be slow so should be performed before performance-critical operation are needing to be executed
     * @note - this method will call private methods within the class in order to complete the HTTP Get Request
     */
    //-------------------------------------------------------------------------------------------------------------
    int HTTP_Request()
    {

        // printf("%s \n" , myTCPIPSERVER);
        printf("%d \n", myHTTPPORT);
        printf("%s \n", URL);
        httpc_connection_t settings;
        settings.result_fn = &result;
        settings.headers_done_fn = headers;

        char *CharMyTCPIPSERVER = new char[myTCPIPSERVER.length() + 1];
        // std::strncpy(CharMyTCPIPSERVER, myTCPIPSERVER.c_str() , 30); //######  breaks the code if enabled idk how or why
        // printf("%s \n" , CharMyTCPIPSERVER);
        char myTCPIPSEVER2[27] = "LAUNCHCONTROL.rocket.local";
        // if(CharMyTCPIPSERVER == myTCPIPSEVER2)
        //{
        //    printf("my mind is broken \n");
        // }
        err_t err = httpc_get_file_dns(
            myTCPIPSEVER2,
            myHTTPPORT,
            URL,
            &settings,
            body,
            this,
            NULL);
        bodyReceived = false;
        printf("status %d \n", err);
        while (!bodyReceived)
        {
            sleep_ms(500);
        }
        sleep_ms(1000);
        free(CharMyTCPIPSERVER);
        return 0;
    }

    //-------------------------------------------------------------------------------------------------------------
    //      Public Method 7
    //   ~ClsHTTPGet
    // used to destruct the object once used
    /**
     * @note - destructor for the class
     * @note - free the memory allocated for the HTTPBuffer
     */
    //-------------------------------------------------------------------------------------------------------------

    ~HTTPGet()
    {
        delete (HTTPBuffer);
    }

    //-------------------------------------------------------------------------------------------------------------
    // End Of Public Methods
    //-------------------------------------------------------------------------------------------------------------

private:
    //-------------------------------------------------------------------------------------------------------------
    //      Private Attributes
    // HTTPBuffer - used to store the data recieved from the HTTP request
    // bodyReceived - used by private methods to verify the data from the HTTP request has been recieved
    // URL - the url that the HTTP request is made on
    // myTCPIPSERVER - holds the DNS name for the TCP/IP Server request will be sent to
    // myHTTPPORT - holds the port number http request will be made over
    //-------------------------------------------------------------------------------------------------------------

    char *HTTPBuffer = (char *)malloc(2000);
    bool bodyReceived;
    char URL[48];
    std::string myTCPIPSERVER = "LAUNCHCONTROL.rocket.local";
    int myHTTPPORT;

    //-------------------------------------------------------------------------------------------------------------
    //      Private Method 1
    //-------------------------------------------------------------------------------------------------------------

    static void result(void *arg, httpc_result_t httpc_result,
                       u32_t rx_content_len, u32_t srv_res, err_t err)
    {
        printf(">>> result_fn >>>\n");
        printf("httpc_result: %s\n",
               httpc_result == HTTPC_RESULT_OK                ? "HTTPC_RESULT_OK"
               : httpc_result == HTTPC_RESULT_ERR_UNKNOWN     ? "HTTPC_RESULT_ERR_UNKNOWN"
               : httpc_result == HTTPC_RESULT_ERR_CONNECT     ? "HTTPC_RESULT_ERR_CONNECT"
               : httpc_result == HTTPC_RESULT_ERR_HOSTNAME    ? "HTTPC_RESULT_ERR_HOSTNAME"
               : httpc_result == HTTPC_RESULT_ERR_CLOSED      ? "HTTPC_RESULT_ERR_CLOSED"
               : httpc_result == HTTPC_RESULT_ERR_TIMEOUT     ? "HTTPC_RESULT_ERR_TIMEOUT"
               : httpc_result == HTTPC_RESULT_ERR_SVR_RESP    ? "HTTPC_RESULT_ERR_SVR_RESP"
               : httpc_result == HTTPC_RESULT_ERR_MEM         ? "HTTPC_RESULT_ERR_MEM"
               : httpc_result == HTTPC_RESULT_LOCAL_ABORT     ? "HTTPC_RESULT_LOCAL_ABORT"
               : httpc_result == HTTPC_RESULT_ERR_CONTENT_LEN ? "HTTPC_RESULT_ERR_CONTENT_LEN"
                                                              : "*UNKNOWN*");
        printf("received %ld bytes\n", rx_content_len);
        printf("server response: %ld\n", srv_res);
        printf("err: %d\n", err);
        printf("<<< result_fn <<<\n");
    }
    //-------------------------------------------------------------------------------------------------------------
    //      Private method 2
    //-------------------------------------------------------------------------------------------------------------

    static err_t headers(httpc_state_t *connection, void *arg,
                         struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
    {
        printf("headers received\n");
        printf("content length=%d\n", content_len);
        printf("header length %d\n", hdr_len);
        pbuf_free(hdr);
        return ERR_OK;
    }

    //-------------------------------------------------------------------------------------------------------------
    //      Private method 3
    //-------------------------------------------------------------------------------------------------------------

    static err_t body(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err)
    {
        HTTPGet *self = (HTTPGet *)arg;
        printf("The body is tot_len: %d\n", p->len);
        printf(">>> body >>>\n");
        pbuf_copy_partial(p, self->HTTPBuffer, p->tot_len, 0);
        printf("%s", self->HTTPBuffer);
        self->bodyReceived = true;
        printf("<<< body <<<\n");
        return ERR_OK;
    }

    //-------------------------------------------------------------------------------------------------------------
    //      End Of private methods
    //-------------------------------------------------------------------------------------------------------------
};

// #endif

// #include "ClsHTTPGet.h"
// #include "WiFiConnect.h"
// #include <iostream>
// #include <cassert>
//     int main(){
//     stdio_init_all();
//     sleep_ms(1000); // allow time to ensure usb or Uart conection established
//     {
//         if (ConnectWifi()){
//             std::cout<< "sucessful connection \n";
//         }
//         else{
//             std::cout << "error \n";
//         }
//    }
//    {
//        ClsHTTPGet InitalCommuication;
//        {
//            char URLForInitalCommunication[48] = "/CheckCommunication.php?MyStatus=ReadyToProceed"; // set the attributes of the object
//            InitalCommuication.URLSetter(URLForInitalCommunication);
//            InitalCommuication.MyHTTPPORTSetter(HTTPPORT);
//            InitalCommuication.MyTCPIPSERVERSetter(std::string(TCPIPSERVER)); // breaks it
//        }
//
//       InitalCommuication.HTTP_Request();
//       printf("%s \n" , InitalCommuication.HTTPBufferGetter());
//       const char expectedValue[7] = "Copy\r\n"; // what should be recieved from the HTTP Get request (note \r\n is from the http get request formatting rather htan the PhP itself)
//      if (strcmp(expectedValue , InitalCommuication.HTTPBufferGetter() ) == 0){
//          printf("\n done \n");

//      }
//      else{
//          printf("failed \n");
//      }
//     InitalCommuication.~ClsHTTPGet(); // destruct the class as no longer neeed
// better to call the destructor rather than reply on the compiler removing the object due to being outside the scope due to dynamic memory allocation
// }
// return 0;
//}
