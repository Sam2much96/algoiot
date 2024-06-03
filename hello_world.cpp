// Connet To Pico Serial on Ubuntu : https://www.gibbard.me/using_the_raspberry_pi_pico_on_ubuntu/
// .run command in Terminal :picocom -b 115200 /dev/ttyACM0

#include <stdio.h>
#include <pico/stdlib.h>

// Boolean
#include <stdbool.h>

// Uart
#include <hardware/uart.h>

// Get Chip Temparature
#include <hardware/adc.h>

//  Uart Setup
//  Board B Uses Tx Pin
#define UART_ID uart1
#define BAUD_RATE 9600
#define UART_TX_PIN 4

// LED

#define LED_PIN CYW43_WL_GPIO_LED_PIN

// Wallet Setup
// #define Wallet AlgoIoT// Wallet
#include "include/AlgoIoT.h"

// Wifi and Web Server
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "webserver/WifiHelper.h"
#include "webserver/lwipopts.h"

// web hooks / handlers
#include "webserver/ssi.h"
#include "webserver/cgi.h"

// for API calls and requests
#include "webserver/requests.h"

#define WIFI_SSID "itel A60"     //"Galaxy A0220d1"
#define WIFI_PASSWORD "00000001" //"rxlz8491"
int main()
{
    // Blinky Led
    /*
        To DO:
        (1) Setup Uart As debug (Done)
        (2) Setup SD Card as Logging
        (3) Build Wallet and Send Outupt to Uart debug
        (4) Implement CUstom HTTP CLient CLass
        (5) Implement Port Forwarding
        (6) Data to send should be json
        (7) Implement Webserver for Board A
        (8) Implement HTTP Requests in a second thread using libcurl : https://github.com/JosephP91/curlcpp
        (9) Implement Multi-threading
        (10) Capture notes from the webserver for llm prompting
        (11) Sd card read and capture

    */

    // init all io for serial and usb debugging
    stdio_init_all();

    // init wifi

    if (WifiHelper::init())
    {
        printf("Wifi Controller Initialised\n");
    }
    else
    {
        printf("Failed to initialise controller\n");
        // return 0;
    }

    printf("Connecting to WiFi... %s \n", WIFI_SSID);

    if (WifiHelper::join(WIFI_SSID, WIFI_PASSWORD))
    {
        printf("Connect to Wifi\n");
    }
    else
    {
        printf("Failed to connect to Wifi \n");
    }

    // Print MAC Address
    char macStr[20];
    WifiHelper::getMACAddressStr(macStr);
    printf("MAC ADDRESS: %s\n", macStr);

    // Print IP Address
    char ipStr[20];
    WifiHelper::getIPAddressStr(ipStr);
    printf("IP ADDRESS: %s\n", ipStr);

    WifiHelper::sntpAddServer("0.africa.pool.ntp.org");
    WifiHelper::sntpAddServer("1.africa.pool.ntp.org");
    WifiHelper::sntpAddServer("2.africa.pool.ntp.org");
    WifiHelper::sntpAddServer("3.africa.pool.ntp.org");
    WifiHelper::sntpSetTimezone(0);
    WifiHelper::sntpStartSync();

    // make request to api server
    //
    https_get_request();

    sleep_ms(10000);
    printf("10 second timeout done");
    // Initialise web server
    // httpd_init();
    // printf("Http server initialised\n");

    // Configure SSI and CGI handler
    // ssi_init();
    // printf("SSI Handler initialised\n");
    // cgi_init();
    // printf("CGI Handler initialised\n");

    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);

    const char *mnemonicQr = "tank game arrive train bring taxi tackle popular bacon gasp tell pigeon error step leaf zone suit chest next swim luggage oblige opinion about execute \n";
    // Pass node debug to uart puts
    // AlgoIot Wallet Setup
    const char *app_name = "Whatever nggq\n";
    const char *mnemonic = "tank game arrive train bring taxi tackle popular bacon gasp tell pigeon error step leaf zone suit chest next swim luggage oblige opinion about execute";

    // wallet
    //  create Algoiot object
    AlgoIoT Wallet(app_name, mnemonic);

    while (true)
    {

        // Blink LED
        uart_puts(UART_ID, mnemonicQr); // Send Board data as output via Uart, send as Json Document
        sleep_ms(3000);                 // sleep for 3 secs
    }
}